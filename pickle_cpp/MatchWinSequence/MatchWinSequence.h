#ifndef MatchWinSequence_h
#define MatchWinSequence_h

#include "../Arduino/Arduino.h"
#include "../Player/Player.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../../include/led-matrix.h"

class MatchWinSequence {

 public:
  MatchWinSequence();
  ~MatchWinSequence();
  void run( GameState* gameState, ScoreBoard* scoreBoard );
};
#endif
