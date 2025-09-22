#ifndef Rules_h
#define Rules_h
/*
 * Rules
 */
#include "../Team/Team.h"
class ScoreBoard;

class Rules {
  public:
    Rules( int freshServes );
    ~Rules();
    void checkGameState( GameState* gameState, Team* team_a, Team* team_b, ScoreBoard* scoreBoard );
    int getFreshServes();
    void setFreshServes( int newServes );
    int getPointsToWin();
    void setPointsToWin( int pointsToWin );
    int getMaxPoints();
    void decrementServe( Team* team ); // Add this line
    void setMaxPoints( int points );
    void updateFreshServes( Team* scoringTeam, Team* opposing_team );


  private:
    void _turnOffServeBars( Team* scoringTeam, Team* opposing_team, ScoreBoard* scoreBoard );
    int _freshServes;
    int _pointsToWin;
    int _maxPoints;
    Team* _scoring_team;
};

#endif // Rules_h
