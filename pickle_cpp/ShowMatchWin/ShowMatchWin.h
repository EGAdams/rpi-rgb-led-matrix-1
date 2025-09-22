#ifndef ShowMatchWin_h
#define ShowMatchWin_h

#include <stack>
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../GameState/GameState.h"
#include "../Logger/Logger.h"
#include "../Player/Player.h"
#include "../Team/Team.h"
#include "../ScoreBoard/ScoreBoard.h"

#if defined _WIN32
#include <string>
#endif

class ShowMatchWin {
 public:
  ShowMatchWin( Team* scoring_team, Team* opposing_team, ScoreBoard* scoreboard );
  ~ShowMatchWin();
  void execute();

 private:
    Team*       _scoringTeam;
    Team*       _opposingTeam;
    ScoreBoard* _scoreBoard;
};

#endif
