#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../Arduino/Arduino.h"
#include "../LedBlinker/LedBlinker.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../Logger/Logger.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../ShowMatchWin/ShowMatchWin.h"
#include "../Team/Team.h"
#include "../Rules/Rules.h"

class ScoreBoard {
 public:
    ScoreBoard( PinInterface* pinInterface, Rules* rules );
    ~ScoreBoard();
    void update( Team* team_a, Team* team_b );
    void showSetWinner( Team* team_a, Team* team_b );
    void showMatchWinner( Team* team_a, Team* team_b );
    void toggleServe( Team* team_a, Team* team_b );
    void setTeamAPointsOn();
    void setTeamBPointsOn();
    void setTeamAPoints( int points );
    void setTeamBPoints( int points );
    void setTeamASets(   int sets   );
    void setTeamBSets(   int sets   );
    void setTeamAServe(  int serve  );
    void setTeamBServe(  int serve  );

 private:
    PinInterface*     _pinInterface;
    Rules*            _rules;
};

#endif
