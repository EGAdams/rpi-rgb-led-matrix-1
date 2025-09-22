# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C++ project for controlling RGB LED matrices on Raspberry Pi, specifically designed for a pickleball scoring system. The project is part of the larger rpi-rgb-led-matrix library ecosystem and implements a state machine-based game control system with support for both hardware (Raspberry Pi) and testing environments.

## Build System

The project uses Make for building. Key build commands:

```bash
# Build the main executable and keyboard version
make

# Build specific targets
make main              # Build run_pickle_listener only
make keyboard          # Build pickle_keyboard only

# Clean build artifacts
make clean

# Clean everything including RGB library
make clean-all

# Build RGB matrix library (from parent directory)
make -C ../lib
```

### Build Configuration

- **Compiler**: g++ with C++17 standard (`-std=c++17`)
- **Main targets**: 
  - `run_pickle_listener` - Main game executable
  - `pickle_keyboard` - Keyboard input version for testing
- **Dependencies**: RGB matrix library (`librgbmatrix.a`) from parent directory
- **Flags**: `-Wall -O0 -g -Wextra -pthread`
- **Linker flags**: `-lrt -lm -lpthread`

### Node.js Integration

The project includes Node.js bindings:

```bash
# Build Node.js addon
npm install

# The addon exposes C++ functionality to Node.js via pickle_cpp.cc
```

## Architecture

### Core State Machine Architecture

The system is built around a state machine pattern that manages different game phases:

1. **StateMachine**: Main controller that transitions between states based on game context
2. **PickleListenerContext**: Shared context containing all game components and resources
3. **IRemoteListenerState**: Interface that all states implement with `handleInput()` method

### Game States

- **PairingModeState**: Remote controller pairing and setup
- **RegularGamePlayBeforeScoreState**: Pre-score game play
- **RegularGamePlayAfterScoreState**: Post-score game play  
- **SleepModeState**: Low power/idle mode
- **AfterMatchWinState**: Match completion handling
- **MatchWinBlinkState**: Victory display animations

### Core Game Components

1. **Game Control**:
   - `GameObject`: Central game controller with team management and game loop
   - `GameState`: Current game state and score tracking
   - `GameTimer`: Timing and timing-related logic
   - `Rules`: Game rules enforcement (supports different player counts)

2. **Team & Player Management**:
   - `Team`: Team objects (TEAM_A, TEAM_B) with serve tracking
   - `Player`: Individual player objects within teams

3. **Input System (Strategy Pattern)**:
   - `IGameInput`: Base interface for all input types
   - `RemoteGameInput`: IR/RF remote control input
   - `KeyboardGameInput`: Keyboard input for testing
   - `TestGameInput`: Automated test input
   - `IInputWithTimer`: Timer-based input handling interface

4. **Display System**:
   - `ScoreBoard`: Main LED matrix display controller
   - `IDisplay`: Display interface (supports both LED matrix and console)
   - `ConsoleDisplay`: Console output for debugging
   - `FontManager`/`FontLoader`: Font handling for display text
   - `ColorManager`: Color management for display elements
   - `CanvasCreator`: RGB matrix canvas management

5. **Visual Effects (Observer Pattern)**:
   - `BlinkController`: Coordinates LED blinking effects
   - `MatchWinBlinker`: Match victory animations
   - `ScoreboardBlinker`: Score display effects
   - `PairingBlinker`: Pairing mode indicators
   - `BlankBlinker`: Blank display state

6. **Hardware Abstraction**:
   - `PinInterface`: GPIO pin control and hardware abstraction
   - `PinState`: GPIO pin state management
   - `DIGI_V6_15`: Hardware-specific configurations

7. **Game Logic Components**:
   - `ScoreCommand`: Command pattern for score operations
   - `History`: Game history tracking
   - `Logger`: Logging system
   - `RemoteCodeTranslator`: Remote control signal translation

### Input Architecture

The system supports three input modes determined at runtime:

1. **Production Mode** (`keyboard_off && !testing`): Uses `RemoteInputWithTimer` for IR/RF remote
2. **Testing Mode** (`testing`): Uses `TestInputWithTimer` for automated testing
3. **Development Mode** (`!keyboard_off`): Uses `KeyboardInputWithTimer` for manual testing

### Key Design Patterns

- **State Pattern**: Game phase management through `IRemoteListenerState`
- **Strategy Pattern**: Different input strategies via `IGameInput` interface
- **Observer Pattern**: `Subject`/`Observer` for LED blink notifications
- **Command Pattern**: Score operations via `ScoreCommand`
- **Factory Pattern**: State creation in `StateMachine::createState()`

## Platform Detection

The system automatically detects Raspberry Pi hardware:

```cpp
bool is_on_raspberry_pi() {
    // Checks /proc/device-tree/model for "Raspberry Pi"
}
```

## Common Development Tasks

### Running the Application

```bash
# Build and run on Raspberry Pi (requires sudo for GPIO access)
make
sudo ./run_pickle_listener

# Run keyboard testing version (no sudo needed)
make keyboard
./pickle_keyboard
```

### Testing

The project includes comprehensive testing infrastructure:

- **TestGameInput**: Simulates game input for automated testing
- **TestInputWithTimer**: Timer-based test input with configurable scenarios
- Test input files in `TestInputWithTimer/` and `TestGameInput/` directories
- Console display mode for debugging without LED hardware

### Adding New States

1. Create new state class inheriting from `IRemoteListenerState`
2. Implement `handleInput(PickleListenerContext& context)` method
3. Add state creation logic in `StateMachine::createState()`
4. Define state transitions and add to appropriate existing states

### Hardware Configuration

- **Target Platform**: Raspberry Pi with RGB LED matrix panels
- **LED Panel Support**: Various sizes via parent directory's RGB matrix library
- **GPIO Requirements**: Root privileges required for hardware GPIO access
- **Connector Standard**: HUB75 connectors for LED panels

## Key File Locations

- **Main executable**: `run_pickle_listener.cpp`
- **Keyboard version**: `pickle_keyboard.cpp` 
- **Build configuration**: `Makefile`
- **Node.js bindings**: `binding.gyp`, `package.json`
- **State machine**: `StateMachine/StateMachine.h`
- **Game context**: `PickleListenerContext/PickleListenerContext.h`
- **Core game logic**: `GameObject/GameObject.h`
- **Display system**: `ScoreBoard/ScoreBoard.h`
- **Hardware abstraction**: `PinInterface/PinInterface.h`
- **Font resources**: `fonts/` directory with `.bdf` and `.fon` files

## Dependencies

- **RGB Matrix Library**: `librgbmatrix.a` from parent directory (`../lib/`)
- **Threading**: pthread (`-lpthread`)
- **System**: rt, m libraries (`-lrt -lm`)
- **Node.js**: node-addon-api for Node.js bindings
- **C++ Standard**: C++17 features and standard library

## Development Notes

- The system uses doubles mode by default (`Rules(2)` = 2 players per team)
- Game initialization includes team setup with serve assignment to Team A
- State machine runs with 100ms delay between state checks
- Memory management uses raw pointers with manual cleanup (pre-smart pointer style)
- Hardware detection enables different code paths for Pi vs development machines