#include "Team.h"
#include "../Arduino/Arduino.h"
#include "../GameState/GameState.h"

Team::Team( int team_number ) : _team_number( team_number ) {
    _points = 0;
    _sets   = 0;
    _gameWin = 0; 
    _remote_paired = false; }
Team::~Team() {}

void  Team::setPoints(         int points ) {         _points = points             ;}
int   Team::getPoints(            ) { return          _points                      ;}
void Team::setSetHistory( std::map<int, int> set_history ) { _set_history = set_history; }

void Team::setSetHistory( int set, int points ) {
    // std::cout << "setting set history for player " << number() << " set " << set << " to " << points << std::endl;
    // if ( set > SETS_TO_WIN_MATCH ) {
    //     std::cerr << "*** ERROR: in Team: set > SETS_TO_WIN_MATCH in Team::setSetHistory(). ***" << std::endl;
    //     exit( 1 );
    // }
    _set_history[ set ] = points;
    number() == 1 ?                      // if this is team number 1...
    _gameState->setPlayer1SetHistory( _set_history ):   // then set player 1's set  history
    _gameState->setPlayer2SetHistory( _set_history ); } // else set player 2's set history
    std::map< int, int> Team::getSetHistory() { return _set_history; }
    
void  Team::setSets(           int sets   ) {                 _sets = sets               ;}
void Team::setSets( GameState* gameState, int sets ) {                                       // sets this player's sets
    this->setSetHistory(          gameState->getCurrentSet(), _points                   );   // and set history for both
    _opposingTeam->setSetHistory( gameState->getCurrentSet(), _opposingTeam->getPoints());   // players.
    if( number() == 1 ) { 
        if ( gameState ) {
            gameState->setPlayer1SetHistory( this->getSetHistory());
            gameState->setPlayer2SetHistory( _opposingTeam->getSetHistory());
        }
    } else {
        if ( gameState ) {
            gameState->setPlayer1SetHistory( _opposingTeam->getSetHistory());
            gameState->setPlayer2SetHistory( this->getSetHistory());
        }
    }
    _sets = sets;
}  
int   Team::getSets(              ) { return          _sets                        ;}
int   Team::number(               ) { return          _team_number                 ;}
int   Team::getServe(             ) { return          _serve                       ;}
void  Team::setServe(          int serve   ) {        _serve = serve               ;}
int   Team::getGameWin(           ) { return          _gameWin                     ;}
void  Team::setGameWin(        int gameWin ) {        _gameWin = gameWin           ;}
Team* Team::getOpposingTeam(      ) { return          _opposingTeam                ;}
void  Team::setOpposingTeam(   Team* opposingTeam ) { _opposingTeam = opposingTeam ;}
void  Team::setRemotePaired( bool remotePaired ) { _remote_paired = remotePaired   ;}
bool  Team::getRemotePaired(      ) { return          _remote_paired               ;}

void  Team::setGameState( GameState* gs ) { _gameState = gs; }
