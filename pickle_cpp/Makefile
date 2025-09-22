# Compiler settings
CXX = g++ -std=c++17 -pthread
CFLAGS = -Wall -O0 -g -Wextra -Wno-unused-parameter
CXXFLAGS = $(CFLAGS) -I$(RGB_INCDIR)

# RGB matrix library
RGB_LIB_DISTRIBUTION = $(shell dirname $(PWD))
RGB_INCDIR = $(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR = $(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME = rgbmatrix
RGB_LIBRARY = $(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

# Linker flags
LDFLAGS = -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# Targets
TARGET = run_pickle_listener
KEYBOARD_TARGET = pickle_keyboard
REMOTE_TARGET = pickle_cpp_remote

# Source files
SOURCES = \
  run_pickle_listener.cpp \
  ColorManager/ColorManager.cpp \
  ConsoleDisplay/ConsoleDisplay.cpp \
  GameTimer/GameTimer.cpp \
  MatchWinSequence/MatchWinSequence.cpp \
  TranslateConstant/TranslateConstant.cpp \
  StateTranslator/StateTranslator.cpp \
  GameState/GameState.cpp \
  RESET/RESET.cpp \
  INPUTS/INPUTS.cpp \
  GAME_MODES/GAME_MODES.cpp \
  ScoreBoard/ScoreBoard.cpp \
  PinState/PinState.cpp \
  PinInterface/PinInterface.cpp \
  Player/Player.cpp \
  Serial.cpp \
  Team/Team.cpp \
  Rules/Rules.cpp \
  ScoreCommand/ScoreCommand.cpp \
  SetDrawer/SetDrawer.cpp \
  ClockDrawer/ClockDrawer.cpp \
  ClockTimer/ClockTimer.cpp \
  ClockUpdater/ClockUpdater.cpp \
  Logger/Logger.cpp \
  History/History.cpp \
  HistoryMonitor/HistoryMonitor.cpp \
  GameStateManager/GameStateManager.cpp \
  RemoteCodeTranslator/RemoteCodeTranslator.cpp \
  RemoteLocker/RemoteLocker.cpp \
  ShowMatchWin/ShowMatchWin.cpp \
  BlinkController/BlinkController.cpp \
  TestInputWithTimer/TestInputWithTimer.cpp \
  MatchWinBlinker/MatchWinBlinker.cpp \
  PickleListenerContext/PickleListenerContext.cpp \
  RemotePairingScreen/RemotePairingScreen.cpp \
  RemoteInputWithTimer/RemoteInputWithTimer.cpp \
  RemoteGameInput/RemoteGameInput.cpp \
  TestGameInput/TestGameInput.cpp \
  KeyboardInputWithTimer/KeyboardInputWithTimer.cpp \
  KeyboardGameInput/KeyboardGameInput.cpp \
  StateMachine/StateMachine.cpp \
  PairingBlinker/PairingBlinker.cpp \
  ScoreboardBlinker/ScoreboardBlinker.cpp \
  IInputWithTimer/IInputWithTimer.cpp \
  RegularGamePlayAfterScoreState/RegularGamePlayAfterScoreState.cpp \
  RegularGamePlayBeforeScoreState/RegularGamePlayBeforeScoreState.cpp \
  PairingModeState/PairingModeState.cpp \
  SleepModeState/SleepModeState.cpp \
  AfterMatchWinState/AfterMatchWinState.cpp \
  MatchWinBlinkState/MatchWinBlinkState.cpp \
  GameObject/GameObject.cpp \
  CanvasCreator/CanvasCreator.cpp \
  FontLoader/FontLoader.cpp \
  Drawer/Drawer.cpp \
  FontManager/FontManager.cpp \
  SetHistoryText/SetHistoryText.cpp

# Shared source files (used by both targets)
SHARED_SOURCES = \
  ColorManager/ColorManager.cpp \
  ConsoleDisplay/ConsoleDisplay.cpp \
  GameTimer/GameTimer.cpp \
  MatchWinSequence/MatchWinSequence.cpp \
  TranslateConstant/TranslateConstant.cpp \
  StateTranslator/StateTranslator.cpp \
  GameState/GameState.cpp \
  RESET/RESET.cpp \
  INPUTS/INPUTS.cpp \
  GAME_MODES/GAME_MODES.cpp \
  ScoreBoard/ScoreBoard.cpp \
  PinState/PinState.cpp \
  PinInterface/PinInterface.cpp \
  Player/Player.cpp \
  Serial.cpp \
  Team/Team.cpp \
  Rules/Rules.cpp \
  ScoreCommand/ScoreCommand.cpp \
  SetDrawer/SetDrawer.cpp \
  ClockDrawer/ClockDrawer.cpp \
  ClockTimer/ClockTimer.cpp \
  ClockUpdater/ClockUpdater.cpp \
  Logger/Logger.cpp \
  History/History.cpp \
  HistoryMonitor/HistoryMonitor.cpp \
  GameStateManager/GameStateManager.cpp \
  RemoteCodeTranslator/RemoteCodeTranslator.cpp \
  RemoteLocker/RemoteLocker.cpp \
  ShowMatchWin/ShowMatchWin.cpp \
  BlinkController/BlinkController.cpp \
  TestInputWithTimer/TestInputWithTimer.cpp \
  MatchWinBlinker/MatchWinBlinker.cpp \
  MatchWinBlinkState/MatchWinBlinkState.cpp \
  PickleListenerContext/PickleListenerContext.cpp \
  RemotePairingScreen/RemotePairingScreen.cpp \
  RemoteInputWithTimer/RemoteInputWithTimer.cpp \
  RemoteGameInput/RemoteGameInput.cpp \
  TestGameInput/TestGameInput.cpp \
  KeyboardInputWithTimer/KeyboardInputWithTimer.cpp \
  KeyboardGameInput/KeyboardGameInput.cpp \
  StateMachine/StateMachine.cpp \
  PairingModeState/PairingModeState.cpp \
  SleepModeState/SleepModeState.cpp \
  AfterMatchWinState/AfterMatchWinState.cpp \
  PairingBlinker/PairingBlinker.cpp \
  ScoreboardBlinker/ScoreboardBlinker.cpp \
  IInputWithTimer/IInputWithTimer.cpp \
  RegularGamePlayAfterScoreState/RegularGamePlayAfterScoreState.cpp \
  RegularGamePlayBeforeScoreState/RegularGamePlayBeforeScoreState.cpp \
  GameObject/GameObject.cpp \
  CanvasCreator/CanvasCreator.cpp \
  FontLoader/FontLoader.cpp \
  Drawer/Drawer.cpp \
  FontManager/FontManager.cpp \
  SetHistoryText/SetHistoryText.cpp

# Keyboard-specific source files
KEYBOARD_SOURCES = pickle_keyboard.cpp $(SHARED_SOURCES)

# Remote-specific source files  
REMOTE_SOURCES = pickle_cpp_remote.cpp $(SHARED_SOURCES)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
KEYBOARD_OBJECTS = $(KEYBOARD_SOURCES:.cpp=.o)
REMOTE_OBJECTS = $(REMOTE_SOURCES:.cpp=.o)

# Default target
all: $(TARGET) $(KEYBOARD_TARGET) $(REMOTE_TARGET)

# Build RGB matrix library if it doesn't exist
$(RGB_LIBRARY):
	@echo "Building RGB matrix library..."
	$(MAKE) -C $(RGB_LIB_DISTRIBUTION)/lib

# Linking rule for main target
$(TARGET): $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Linking rule for keyboard target
$(KEYBOARD_TARGET): $(KEYBOARD_OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ $(KEYBOARD_OBJECTS) $(LDFLAGS)

# Linking rule for remote target
$(REMOTE_TARGET): $(REMOTE_OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ $(REMOTE_OBJECTS) $(LDFLAGS)

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(KEYBOARD_OBJECTS) $(REMOTE_OBJECTS) $(TARGET) $(KEYBOARD_TARGET) $(REMOTE_TARGET)

# Clean everything including RGB library
clean-all: clean
	$(MAKE) -C $(RGB_LIB_DISTRIBUTION)/lib clean

# Individual targets
main: $(TARGET)
keyboard: $(KEYBOARD_TARGET)
remote: $(REMOTE_TARGET)
rgblib: $(RGB_LIBRARY)

.PHONY: all clean clean-all main keyboard remote rgblib
