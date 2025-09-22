// =============================
// File: History.cpp
// =============================
#include "History.h"
// IMPORTANT: Need full GameState definition here (we construct/return it) git check...
#include "../GameState/GameState.h"

#include <iomanip>

History::History()
    : _start_canary(HISTORY_CANARY_START)
    , _logical_size(0)
    , _internal_canary(HISTORY_CANARY_INTERNAL)
    , _logger(nullptr)
    , _end_canary(HISTORY_CANARY_END)
    , _initialized(false)
    , _last_known_vector_size(0)
    , _last_known_vector_capacity(0)
{
    std::cout << "*** PROTECTED_HISTORY: Constructor ENTRY, this=" << this << std::endl;

    try {
        _history.reserve(100); // pre‑allocate to avoid frequent reallocations
        _last_known_vector_capacity = 100;

        if (!_validateCanaries("constructor")) {
            std::cout << "*** PROTECTED_HISTORY: CRITICAL - Canary corruption during construction!" << std::endl;
            return;
        }

        _initialized = true;
        std::cout << "*** PROTECTED_HISTORY: Constructor complete - protection active ***" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "*** PROTECTED_HISTORY: Exception during construction: " << e.what() << std::endl;
        _initialized = false;
    }
    catch (...) {
        std::cout << "*** PROTECTED_HISTORY: Unknown exception during construction" << std::endl;
        _initialized = false;
    }
}

// Ensure GameState is complete in this translation unit
static_assert(sizeof(GameState) > 0, "GameState must be a complete type here");

History::~History() {
    std::cout << "*** PROTECTED_HISTORY: Destructor entry, this=" << this
              << ", final status: " << (isCorrupted() ? "CORRUPTED" : "CLEAN")
              << std::endl;

    if (_initialized && !_validateCanaries("destructor")) {
        std::cout << "*** PROTECTED_HISTORY: CORRUPTION detected in destructor!" << std::endl;
    }

    // Best‑effort tripwire to catch accidental post‑destruction use in debug runs
    _destroyed.store(true, std::memory_order_relaxed);

    std::cout << "*** PROTECTED_HISTORY: Destructor complete - smart pointers will handle cleanup ***" << std::endl;
}

void History::_uafGuard(const char* fn) const {
    if (_destroyed.load(std::memory_order_relaxed)) {
        std::cerr << "*** PROTECTED_HISTORY: UAF guard tripped in "
                  << (fn ? fn : "<unknown>") << ", this=" << this << std::endl;
        std::abort();
    }
}

void History::push(const GameState& state) {
    _uafGuard("push");
    std::cout << "state.matchWinTime" << state.getMatchWinTime().count() << std::endl;
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);

    std::cout << "*** PROTECTED_HISTORY: push() ENTRY" << std::endl;

    if (!validateIntegrity("push-entry")) {
        std::cout << "*** PROTECTED_HISTORY: Integrity validation FAILED at push entry - ABORTING" << std::endl;
        return;
    }

    try {
        auto gameStatePtr = std::make_shared<GameState>(state);
        std::cout << "*** PROTECTED_HISTORY: Created shared_ptr for GameState ***" << std::endl;

        if (_logical_size < _history.size()) {
            std::cout << "*** PROTECTED_HISTORY: Reusing existing slot at index " << _logical_size << std::endl;
            _history[_logical_size] = gameStatePtr;
        } else {
            std::cout << "*** PROTECTED_HISTORY: Adding new shared_ptr to vector ***" << std::endl;
            _history.push_back(std::move(gameStatePtr));
        }

        ++_logical_size;
        _last_known_vector_size = _history.size();
        
        std::cout << "*** PROTECTED_HISTORY: After push - logical_size: " << _logical_size 
                  << ", vector_size: " << _history.size();
        
        // Safety check for runaway growth
        if (_logical_size > 500) {
            std::cout << " (ERROR: EXCESSIVE HISTORY SIZE - CLEARING TO PREVENT CORRUPTION!)";
            _logical_size = 0; // Emergency reset
            _history.clear();
        }
        std::cout << " ***" << std::endl;
        _last_known_vector_capacity = _history.capacity();

        std::cout << "*** PROTECTED_HISTORY: push() completed, logical_size: " << _logical_size << std::endl;

        if (!validateIntegrity("push-exit")) {
            std::cout << "*** PROTECTED_HISTORY: Integrity validation FAILED after push - CORRUPTION OCCURRED DURING PUSH!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "*** PROTECTED_HISTORY: Exception during push: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "*** PROTECTED_HISTORY: Unknown exception during push" << std::endl;
    }
}

int History::size() {
    _uafGuard("size");
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);

    if (!validateIntegrity("size-check")) {
        std::cout << "*** PROTECTED_HISTORY: Integrity validation FAILED at size check - returning 0" << std::endl;
        return 0;
    }

    std::cout << "*** PROTECTED_HISTORY: size() returning logical_size: " << _logical_size;
    if (_logical_size > 200) {
        std::cout << " (WARNING: EXCESSIVE SIZE!)";
    }
    std::cout << std::endl;
    return static_cast<int>(_logical_size);
}

bool History::empty() {
    _uafGuard("empty");
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);
    return _logical_size == 0;
}

GameState History::pop() {
    _uafGuard("pop");
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);
    std::cout << "\n\n\n**********************************************" << std::endl;
    std::cout << "History::pop() called..." << std::endl;
    std::cout << "************************************************\n\n\n" << std::endl;

    std::cout << "*** PROTECTED_HISTORY: pop() ENTRY" << std::endl;

    if (!validateIntegrity("pop-entry")) {
        std::cout << "*** PROTECTED_HISTORY: Integrity validation FAILED at pop entry - ABORTING, returning default GameState" << std::endl;
        return GameState();
    }

    if (_logical_size == 0) {
        std::cout << "*** PROTECTED_HISTORY: Attempting to pop from empty logical history stack!" << std::endl;
        return GameState();
    }

    try {
        const std::size_t topIndex = _logical_size - 1;
        std::cout << "*** PROTECTED_HISTORY: Getting element at logical top index: " << topIndex << std::endl;

        if (topIndex >= _history.size()) {
            std::cout << "*** PROTECTED_HISTORY: CRITICAL ERROR: topIndex " << topIndex
                      << " >= vector size " << _history.size() << " ***" << std::endl;
            _reportCorruption("pop-bounds-check", "topIndex >= vector.size()");
            return GameState();
        }

        // at() gives extra debug checking in libstdc++ debug mode
        auto& gameStatePtr = _history.at(topIndex);
        if (!gameStatePtr) {
            std::cout << "*** PROTECTED_HISTORY: CRITICAL ERROR: shared_ptr at index " << topIndex << " is null!" << std::endl;
            return GameState();
        }

        GameState result(*gameStatePtr); // copy out
        std::cout << "*** PROTECTED_HISTORY: Successfully created copy from shared_ptr at index " << topIndex << std::endl;

        // Logical pop (do not erase capacity; keep slot for reuse)
        --_logical_size;
        std::cout << "*** PROTECTED_HISTORY: Decremented logical_size to: " << _logical_size << std::endl;

        if (!validateIntegrity("pop-exit")) {
            std::cout << "*** PROTECTED_HISTORY: Integrity validation FAILED after pop - CORRUPTION OCCURRED DURING POP!" << std::endl;
        }

        std::cout << "*** PROTECTED_HISTORY: pop() returning successfully ***" << std::endl;
        return result;
    }
    catch (const std::exception& e) {
        std::cout << "*** PROTECTED_HISTORY: Exception in pop(): " << e.what() << std::endl;
        return GameState();
    }
    catch (...) {
        std::cout << "*** PROTECTED_HISTORY: Unknown exception in pop()" << std::endl;
        return GameState();
    }
}

void History::clear() {
    _uafGuard("clear");
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);

    std::cout << "\n\n\n**********************************************" << std::endl;
    std::cout << "clearing history..." << std::endl;
    std::cout << "************************************************\n\n\n" << std::endl;

    // Keep capacity for reuse; just logically clear content
    for (auto& sp : _history) sp.reset();
    _history.clear();

    _logical_size = 0;
    _last_known_vector_size = 0;
    // keep _last_known_vector_capacity as capacity() (unchanged)

    std::cout << "*** PROTECTED_HISTORY: clear() completed (capacity kept for reuse) ***" << std::endl;
    
    while ( !_history.empty()) { 
        std::cout << "clearing history some more..." << std::endl;
        pop(); 
    }
}

bool History::validateIntegrity(const char* context) {
    _uafGuard("validateIntegrity");
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);

    if (!_initialized) {
        _reportCorruption(context, "History not initialized");
        return false;
    }

    bool ok = true;
    ok = _validateCanaries(context) && ok;
    ok = _validateVectorIntegrity(context) && ok;

    _last_known_vector_size     = _history.size();
    _last_known_vector_capacity = _history.capacity();

    return ok;
}

bool History::isCorrupted() const {
    // Note: this method is used in destructor; keep it read‑only and tolerant
    std::lock_guard<std::recursive_mutex> lock(_protection_mutex);

    if (_start_canary    != HISTORY_CANARY_START)    return true;
    if (_internal_canary != HISTORY_CANARY_INTERNAL) return true;
    if (_end_canary      != HISTORY_CANARY_END)      return true;

    if (!_initialized) return true;

    if (_logical_size > _history.size())     return true;
    if (_logical_size > _history.capacity()) return true;

    for (std::size_t i = 0; i < _logical_size; ++i) {
        if (!_history[i]) return true;
    }

    return false;
}

bool History::_validateCanaries(const char* context) {
    bool ok = true;

    if (_start_canary != HISTORY_CANARY_START) {
        _reportCorruption(context, "Start canary mismatch (DEADBEEF expected)");
        ok = false;
    }
    if (_internal_canary != HISTORY_CANARY_INTERNAL) {
        _reportCorruption(context, "Internal canary mismatch (FEEDFACE expected)");
        ok = false;
    }
    if (_end_canary != HISTORY_CANARY_END) {
        _reportCorruption(context, "End canary mismatch (CAFEBABE expected)");
        ok = false;
    }

    return ok;
}

bool History::_validateVectorIntegrity(const char* context) {
    bool ok = true;

    if (_logical_size > _history.size()) {
        std::ostringstream ss;
        ss << "logical_size (" << _logical_size << ") > vector.size()(" << _history.size() << ")";
        _reportCorruption(context, ss.str());
        ok = false;
    }

    if (_history.capacity() < _history.size()) {
        std::ostringstream ss;
        ss << "vector.capacity()(" << _history.capacity() << ") < vector.size()(" << _history.size() << ")";
        _reportCorruption(context, ss.str());
        ok = false;
    }

    for (std::size_t i = 0; i < _logical_size; ++i) {
        if (!_history[i]) {
            std::ostringstream ss;
            ss << "null shared_ptr at logical index " << i;
            _reportCorruption(context, ss.str());
            ok = false;
            break;
        }
    }

    return ok;
}

void History::_reportCorruption(const char* context, const std::string& details) const {
    std::cout << std::hex << std::showbase;
    std::cout << "*** PROTECTED_HISTORY: CORRUPTION at [" << (context ? context : "unknown")
              << "] this=" << this << "\n"
              << "    start_canary=" << _start_canary
              << " expected=" << HISTORY_CANARY_START << "\n"
              << "    internal_canary=" << _internal_canary
              << " expected=" << HISTORY_CANARY_INTERNAL << "\n"
              << "    end_canary=" << _end_canary
              << " expected=" << HISTORY_CANARY_END << "\n"
              << std::dec << std::noshowbase
              << "    logical_size=" << _logical_size
              << " vector.size()=" << _history.size()
              << " capacity=" << _history.capacity() << "\n"
              << "    last_known.size=" << _last_known_vector_size
              << " last_known.capacity=" << _last_known_vector_capacity << "\n"
              << "    details: " << details << std::endl;
    // Mirror to external logger if you wire one through _logger
}
