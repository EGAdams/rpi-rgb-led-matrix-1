# Pairing Mode Implementation and Debug Notes

## Overview

This document captures the implementation of pairing mode in `run_pickle_listener.cpp` with TestInputWithTimer integration, including the critical segmentation fault bug discovery and fix.

## Implementation Request

User requested implementing pairing mode in `run_pickle_listener.cpp` with TestInputWithTimer integration to enable testing before connecting keyboard or remote input modes.

### Referenced Files
- `PairingModeStateTest.cpp` - Test framework with extensive debugging
- `run_pickle_listener.cpp` - Main startup file 
- `TestInputWithTimer.cpp` - Test input simulation

## Implementation Changes

### 1. StateMachine Updates
**File: `StateMachine/StateMachine.h`**
```cpp
// Uncommented:
#include "../PairingModeState/PairingModeState.h"
```

**File: `StateMachine/StateMachine.cpp`**
```cpp
// Uncommented case statement:
case PAIRING_MODE_STATE:
    return std::make_unique<PairingModeState>();

// Modified constructor:
StateMachine::StateMachine( PickleListenerContext& context )
    : _context( context ), _currentStateId( PAIRING_MODE_STATE ) {
    setState( PAIRING_MODE_STATE );  // Start in pairing mode instead of hardcoded 5
}
```

### 2. Test Input Configuration
**File: `TestInputWithTimer/test_input.txt`**
```
// Uncommented pairing commands:
GREEN_REMOTE_GREEN_SCORE
RED_REMOTE_RED_SCORE
```

### 3. Build System
**File: `Makefile`**
```makefile
# Added to SOURCES list:
PairingModeState/PairingModeState.cpp
```

## Critical Bug Discovery and Fix

### Segmentation Fault Symptoms
- Occurred immediately after "Waiting for pairing input..." message
- User requested extensive debugging with `[DEBUG_PAIR_SEG_FAULT]` tags
- Required liberal exception handling and thorough logging

### Root Cause Analysis
**File: `PickleListenerContext/PickleListenerContext.cpp`**

**BEFORE (BUGGY):**
```cpp
PickleListenerContext::PickleListenerContext(
    // ... other parameters ...
    IInputWithTimer* pairingBlinkerWithTimer,  // L Wrong parameter name
    // ... rest of parameters ...
)   : // ... other initializations ...
    // L MISSING: pairingInputWithTimer assignment
```

**AFTER (FIXED):**
```cpp
PickleListenerContext::PickleListenerContext(
    // ... other parameters ...
    IInputWithTimer* pairingInputWithTimer_arg,  //  Correct parameter name
    // ... rest of parameters ...
)   : // ... other initializations ...
    pairingInputWithTimer(pairingInputWithTimer_arg),  //  Proper assignment
    // ... rest of initializations ...
```

### Bug Impact
- `pairingInputWithTimer` member variable was uninitialized (nullptr)
- Segfault occurred when `PairingModeState::handleInput()` called `getInput()` on nullptr
- Constructor parameter existed but wasn't assigned to member variable

## Debugging Enhancements Added

### 1. Constructor Debugging
**File: `PickleListenerContext/PickleListenerContext.cpp`**
```cpp
PickleListenerContext::PickleListenerContext(/* ... */) {
    print("[DEBUG_PAIR_SEG_FAULT] PickleListenerContext constructor called");
    print("[DEBUG_PAIR_SEG_FAULT] pairingInputWithTimer_arg: " << pairingInputWithTimer_arg);
    print("[DEBUG_PAIR_SEG_FAULT] this->pairingInputWithTimer: " << this->pairingInputWithTimer);
    print("[DEBUG_PAIR_SEG_FAULT] pairingBlinker: " << pairingBlinker);
    print("[DEBUG_PAIR_SEG_FAULT] remotePairingScreen: " << remotePairingScreen);
}
```

### 2. PairingModeState Extensive Debugging
**File: `PairingModeState/PairingModeState.cpp`**
```cpp
#include <cassert>
#include <stdexcept>

void PairingModeState::handleInput( PickleListenerContext& context ) {
    print("[DEBUG_PAIR_SEG_FAULT] Entering PairingModeState::handleInput()");
    
    try {
        // Extensive null pointer validation
        auto pairingInputWithTimer = context.getPairingInputWithTimer();
        assert( pairingInputWithTimer != nullptr );
        
        // Protected getInput() call with exception handling
        int selection = -1;
        try {
            selection = pairingInputWithTimer->getInput();
            print("[DEBUG_PAIR_SEG_FAULT] getInput() returned: " << selection);
        } catch (const std::exception& e) {
            print("[DEBUG_PAIR_SEG_FAULT] Exception in getInput(): " << e.what());
            context.unlock();
            throw;
        }
        
        // Process pairing logic with extensive logging...
        
    } catch (const std::exception& e) {
        print("[DEBUG_PAIR_SEG_FAULT] Exception: " << e.what());
        context.unlock();
        throw;
    }
}
```

## Testing Results

### After Fix - Successful Output
```
[DEBUG_PAIR_SEG_FAULT] getInput() returned: 5
[DEBUG_PAIR_SEG_FAULT] Green player pairing...
Green player paired.
[DEBUG_PAIR_SEG_FAULT] getInput() returned: 1  
[DEBUG_PAIR_SEG_FAULT] Red player pairing...
Red player paired.
Both players paired, exiting pairing mode.
[StateMachine] Transitioned to state: 5
```

### Key Test Input Mappings
- `GREEN_REMOTE_GREEN_SCORE` ’ Code 5 ’ Green player pairing
- `RED_REMOTE_RED_SCORE` ’ Code 1 ’ Red player pairing

## Architecture Notes

### State Machine Pattern
- `PairingModeState` implements `IRemoteListenerState` interface
- Uses `PickleListenerContext` for shared state management
- Thread-safe with explicit lock()/unlock() methods

### Input Abstraction
- `TestInputWithTimer` implements `IInputWithTimer` interface
- Reads sequential commands from `test_input.txt`
- Maps string commands to integer codes via `_input_map`

### Pairing Logic Flow
1. Start in `PAIRING_MODE_STATE` (7)
2. Display pairing screen with blinking
3. Process player inputs sequentially
4. Mark players as paired individually
5. Transition to `REGULAR_PLAY_NO_SCORE_STATE` (5) when both paired

## Files Modified

| File | Change Type | Description |
|------|-------------|-------------|
| `StateMachine/StateMachine.h` | Enable | Uncommented PairingModeState include |
| `StateMachine/StateMachine.cpp` | Logic | Added case statement, changed initial state |
| `TestInputWithTimer/test_input.txt` | Config | Uncommented pairing commands |
| `Makefile` | Build | Added PairingModeState.cpp to sources |
| `PickleListenerContext/PickleListenerContext.cpp` | **Critical Fix** | Fixed constructor parameter assignment |
| `PairingModeState/PairingModeState.cpp` | Debug | Added extensive debugging and exception handling |

## Production Considerations

### Optional Cleanup
- Remove extensive `[DEBUG_PAIR_SEG_FAULT]` output after production verification
- Keep essential error handling and assertions

### Hardware Testing
- Current testing on development machine
- Verify on actual Raspberry Pi hardware before deployment

## Key Learnings

1. **Constructor Parameter Bugs**: Verify parameter names match member variable assignments
2. **Null Pointer Safety**: Always validate pointers before dereferencing in embedded systems
3. **Exception Handling**: Critical for debugging segfaults in multi-threaded state machines
4. **Test Input Integration**: TestInputWithTimer provides effective simulation for hardware testing

## Status:  COMPLETED

Pairing mode implementation with TestInputWithTimer integration is fully functional. The critical segmentation fault has been resolved, and the system successfully pairs both players and transitions to regular gameplay.