// History/History.h
#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <sstream>

class GameState; // forward decl

class History : public std::enable_shared_from_this<History> {
public:
    using Ptr = std::shared_ptr<History>;
    using WeakPtr = std::weak_ptr<History>;
    static Ptr create() { return std::make_shared<History>(); }

    History();
    ~History();

    History(const History&) = delete;
    History& operator=(const History&) = delete;
    History(History&&) = delete;
    History& operator=(History&&) = delete;

    void      push(const GameState& state);
    GameState pop();                 // returns default GameState on error/empty
    int       size();                // kept as int for back-compat
    bool      empty();
    void      clear();               // logical clear, keeps capacity

    bool validateIntegrity(const char* context);
    bool isCorrupted() const;

private:
    // canaries
    static constexpr std::uint32_t HISTORY_CANARY_START    = 0xDEADBEEF;
    static constexpr std::uint32_t HISTORY_CANARY_INTERNAL = 0xFEEDFACE;
    static constexpr std::uint32_t HISTORY_CANARY_END      = 0xCAFEBABE;

    void _uafGuard(const char* fn) const;
    bool _validateCanaries(const char* context);
    bool _validateVectorIntegrity(const char* context);
    void _reportCorruption(const char* context, const std::string& details) const;

    // layout
    std::uint32_t _start_canary;
    std::size_t   _logical_size;
    std::uint32_t _internal_canary;
    void*         _logger;
    std::uint32_t _end_canary;
    bool          _initialized;

    std::vector<std::shared_ptr<GameState>> _history;
    std::size_t _last_known_vector_size;
    std::size_t _last_known_vector_capacity;

    mutable std::recursive_mutex _protection_mutex; // <-- needed
    std::atomic<bool> _destroyed{false};            // <-- needed
};
