#include "../ShowMatchWin/ShowMatchWin.h"
#include "../ScoreBoard/ScoreBoard.h"

ShowMatchWin::ShowMatchWin( Team* scoringTeam, 
                            Team* opposingTeam, 
                            ScoreBoard* scoreBoard ) : _scoringTeam(  scoringTeam  ), 
                                                       _opposingTeam( opposingTeam ), 
                                                       _scoreBoard(   scoreBoard   ) {}
ShowMatchWin::~ShowMatchWin() {}

void ShowMatchWin::execute() {
    for ( int i = 0; i < MATCH_WINNER_SET_FLASH_COUNT; i++ ) {
        _scoreBoard->setTeamASets( _scoringTeam->number()   == TEAM_A ? _scoringTeam->getSets()    : _opposingTeam->getSets() );
        _scoreBoard->setTeamBSets( _opposingTeam->number()  == TEAM_B ? _opposingTeam->getSets()   : _scoringTeam->getSets()  );
        GameTimer::gameDelay( MATCH_WINNER_SET_FLASH_DELAY );
        _scoreBoard->setTeamASets( _scoringTeam->number()   == TEAM_A ? 0    : _opposingTeam->getSets() );
        _scoreBoard->setTeamBSets( _opposingTeam->number()  == TEAM_B ? _opposingTeam->getSets()   : 0  );
        GameTimer::gameDelay( MATCH_WINNER_SET_FLASH_DELAY ); }
    
    _scoreBoard->setTeamAPoints( 100 ); _scoreBoard->setTeamASets( 0 );
    _scoreBoard->setTeamBPoints( 100 ); _scoreBoard->setTeamBSets( 0 );
    for ( int i = 0; i < MATCH_WINNER_FLASH_COUNT; i++ ) {
        for ( int game_lamp_count = 1; game_lamp_count < GAME_LAMP_COUNT; game_lamp_count++ ) {
            _scoringTeam->number() == TEAM_A ? _scoreBoard->setTeamAPoints( game_lamp_count ) :
                                               _scoreBoard->setTeamBPoints( game_lamp_count );
            GameTimer::gameDelay( MATCH_WINNER_FLASH_DELAY ); }
        _scoreBoard->setTeamAPoints( 100 );
        _scoreBoard->setTeamBPoints( 100 );
        _scoringTeam->number() == TEAM_A ? _scoreBoard->setTeamASets( 1 ) : _scoreBoard->setTeamBSets( 1 );
        GameTimer::gameDelay( MATCH_WINNER_FLASH_DELAY );
        _scoreBoard->setTeamASets( 0 );
        _scoreBoard->setTeamBSets( 0 );
        GameTimer::gameDelay( MATCH_WINNER_FLASH_DELAY );
        _scoringTeam->number() == TEAM_A ? _scoreBoard->setTeamASets( SET_2_ONLY ) : _scoreBoard->setTeamBSets( SET_2_ONLY );
        GameTimer::gameDelay( MATCH_WINNER_FLASH_DELAY );
        _scoreBoard->setTeamASets( 0 );
        _scoreBoard->setTeamBSets( 0 );
        GameTimer::gameDelay( MATCH_WINNER_FLASH_DELAY ); }}
