# Persona
World-class C++ Developer

# Makefile being used
```makefile
# Makefile for pickle_cpp_cli project

# Compiler and linker settings
CXX = g++
CXXFLAGS = -Wall -Wextra $(INCLUDES)
LDFLAGS =

# Include directories
INCLUDES = -IUNDO -IDIGI_V6_15 -IGAME_LEDs -IGAME_MODES -IArduino -IPinState \
           -IPinInterface -IPlayer -IGameState -IGameTimer -IINPUTS -IRESET \
           -IPOINT_LEDs -IMODE_1_FUNCTIONS -IMODE_1_SCORE -ISERVE_LEDs \
           -IMODE_1_TIEBREAKER -IMODE_1_WIN_SEQUENCES -ISET_LEDs \
           -IMODE_2_FUNCTIONS -IGameObject -IScoreBoard -ITranslateConstant \
           -ITeam -IRules -IScoreCommand -ICommand -ILogger -IHistory \
           -IGameStateManager -IRemoteCodeTranslator -IRemoteLocker \
           -IShowMatchWin -IZeroCondition -IObserver -ISubject -IBlinkController

# Source files
SOURCES = pickle_cpp_cli.cc GameTimer/GameTimer.cpp \
          TranslateConstant/TranslateConstant.cpp GameState/GameState.cpp \
          RESET/RESET.cpp INPUTS/INPUTS.cpp GAME_MODES/GAME_MODES.cpp \
          ScoreBoard/ScoreBoard.cpp GameObject/GameObject.cpp \
          PinState/PinState.cpp PinInterface/PinInterface.cpp \
          Player/Player.cpp Serial.cpp Team/Team.cpp Rules/Rules.cpp \
          ScoreCommand/ScoreCommand.cpp Logger/Logger.cpp \
          History/History.cpp GameStateManager/GameStateManager.cpp \
          RemoteCodeTranslator/RemoteCodeTranslator.cpp RemoteLocker/RemoteLocker.cpp \
          ShowMatchWin/ShowMatchWin.cpp ZeroCondition/ZeroCondition.cpp \
          BlinkController/BlinkController.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = pickle_cpp_cli

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Generic rule for compiling objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
```

# tree -L 2 output of the project
```bash
adamsl@DESKTOP-BKHEBT0:/mnt/c/Users/EG/Desktop/2022/may/2nd_week/pickle_cpp$ tree -L 2
.
├── Arduino
│   ├── Arduino.cpp
│   └── Arduino.h
├── BlinkController
│   ├── BlinkController.cpp
│   └── BlinkController.h
├── Command
│   └── Command.h
├── CppInterface.cpp
├── CppInterface.h
├── DIGI_V6_15
│   ├── DIGI_V6_15.cpp
│   ├── DIGI_V6_15.h
│   ├── jewelry_link.txt
│   └── scratchpad.js
├── GAME_MODES
│   ├── GAME_MODES.cpp
│   ├── GAME_MODES.h
│   └── log.txt
├── GameObject
│   ├── GameObject.cpp
│   └── GameObject.h
├── GameState
│   ├── GameState.cpp
│   └── GameState.h
├── GameStateManager
│   ├── GameStateManager.cpp
│   └── GameStateManager.h
├── GameTimer
│   ├── GameTimer.cpp
│   └── GameTimer.h
├── History
│   ├── History.cpp
│   └── History.h
├── INPUTS
│   ├── INPUTS.cpp
│   └── INPUTS.h
├── LICENSE
├── Logger
│   ├── Logger.cpp
│   └── Logger.h
├── Loop.cpp
├── Makefile
├── Morse
│   ├── Morse.cpp
│   ├── Morse.h
│   ├── examples
│   └── keywords.txt
├── Observer
│   └── Observer.h
├── PinInterface
│   ├── PinInterface.cpp
│   └── PinInterface.h
├── PinState
│   ├── PinState.cpp
│   └── PinState.h
├── Player
│   ├── Player.cpp
│   └── Player.h
├── RESET
│   ├── RESET.cpp
│   └── RESET.h
├── RemoteCodeTranslator
│   ├── RemoteCodeTranslator.cpp
│   └── RemoteCodeTranslator.h
├── RemoteLocker
│   ├── RemoteLocker.cpp
│   └── RemoteLocker.h
├── Rules
│   ├── Rules.cpp
│   └── Rules.h
├── ScoreBoard
│   ├── ScoreBoard.cpp
│   └── ScoreBoard.h
├── ScoreCommand
│   ├── ScoreCommand.cpp
│   └── ScoreCommand.h
├── Serial.cpp
├── Serial.h
├── ShowMatchWin
│   ├── ShowMatchWin.cpp
│   └── ShowMatchWin.h
├── Subject
│   └── Subject.h
├── Team
│   ├── Team.cpp
│   └── Team.h
├── TranslateConstant
│   ├── TranslateConstant.cpp
│   └── TranslateConstant.h
├── ZeroCondition
│   ├── ZeroCondition.cpp
│   └── ZeroCondition.h
├── addon.js
├── binding.gyp
├── build
│   ├── Release
│   ├── binding.sln
│   ├── config.gypi
│   ├── pickle_cpp.vcxproj
│   └── pickle_cpp.vcxproj.filters
├── compile.bat
├── index.html
├── notes.cpp
├── package-lock.json
├── package.json
├── pickle_cpp.cc
├── pickle_cpp_cli.cc
└── rename_ino_cpp.sh
```

# make error to debug
```bash
...
In file included from pickle_cpp_cli.cc:1:
./GameObject/GameObject.h:9:10: fatal error: GAME_MODES.h: No such file or directory
    9 | #include "GAME_MODES.h"
      |          ^~~~~~~~~~~~~~
compilation terminated.
make: *** [Makefile:41: pickle_cpp_cli] Error 1
adamsl@DESKTOP-BKHEBT0:/mnt/c/Users/EG/Desktop/2022/may/2nd_week/pickle_cpp$
```

# partial GameObject.h
```cpp
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#if defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __WIN64__ || defined __linux__ || defined WIN32 || defined WIN64
    #include <iostream>
#else
    #include "Arduino.h"
#endif    

#include "GAME_MODES.h"
#include "GameState.h"
#include "GameTimer.h"
...
```

# Your Task
Analyze the project structure and find out why make is failing.
