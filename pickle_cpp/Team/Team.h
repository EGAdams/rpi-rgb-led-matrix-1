#ifndef Team_h
#define Team_h

#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../Arduino/Arduino.h"
#include "../Subject/Subject.h"
#include <map>
class GameState;
class Team : public Subject {
 public:
    Team( int team_number );
    ~Team();
    void setPoints( int points );
    int getPoints();
    void setSets( int sets );
    void setSets( GameState* gameState, int sets );
    int getSets(); 
    int number();
    int getServe();
    void setServe( int serve );
    int getGameWin();
    void setGameWin( int gameWin );
    Team* getOpposingTeam();
    void setOpposingTeam( Team* opposingTeam );
    bool getRemotePaired();
    void setRemotePaired( bool remotePaired );
    void setGameState( GameState* gs );

 private:
    int     _team_number;
    int     _points;
    int     _sets;
    int     _serve;
    int     _gameWin;
    Team*   _opposingTeam;
    bool    _remote_paired;
    GameState* _gameState;
    std::map<int, int> _set_history;
    void setSetHistory( std::map<int, int> set_history );
    void setSetHistory( int set, int games );
    std::map< int, int> getSetHistory();
};

#endif
