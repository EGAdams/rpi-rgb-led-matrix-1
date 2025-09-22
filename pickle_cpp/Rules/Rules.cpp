/** @class Rules */
#include "../Rules/Rules.h"
#include "../ScoreBoard/ScoreBoard.h"

Rules::Rules( int freshServes ) : _freshServes( freshServes ) {}
Rules::~Rules(){};

void Rules::checkGameState( GameState* gameState, Team* scoring_team, Team* opposing_team, ScoreBoard* scoreBoard ) {
    int scoring_team_points = scoring_team->getPoints();
    int opposing_team_points = opposing_team->getPoints();
    // if ( scoring_team->getSets() > 0 && opposing_team->getSets() > 0 ) {
    //     _pointsToWin = GAME_3_POINTS_TO_WIN;  // change points to win in Game 3.
    // }
    if ( scoring_team_points >= _pointsToWin && scoring_team_points - 1 > opposing_team_points ) { // Game win scenario
        scoring_team->setGameWin( 1 );
        int new_set_value = scoring_team->getSets() + 1;
        scoring_team->setSets( gameState, scoring_team->getSets() + 1 );
        scoring_team->setSets( new_set_value );
        scoring_team->number()  == GREEN_TEAM ? gameState->setTeamASets( new_set_value ) : gameState->setTeamBSets( new_set_value );
        gameState->setCurrentSet( gameState->getCurrentSet() + 1 );      // set game state's current set for display
        if (( scoring_team->getSets() == SETS_TO_WIN_MATCH )) {              // it's a Match winner
            _turnOffServeBars( scoring_team, opposing_team, scoreBoard );
            ClockTimer* clockTimer = scoreBoard->getClockTimer();
            if (clockTimer != nullptr && clockTimer->isRunning()) {
                // match timer saved in Score Command
                // Stop the ClockTimer immediately
                clockTimer->stop();
                std::cout << "*** ClockTimer stopped immediately at match win ***" << std::endl;
            } else if (clockTimer == nullptr) {
                std::cout << "*** WARNING: ClockTimer is null in MatchWinSequence ***" << std::endl;
            } else {
                std::cout << "*** INFO: ClockTimer was not running when match won ***" << std::endl;
            }
            scoreBoard->showSetWinner( scoring_team, opposing_team );
            scoreBoard->showMatchWinner( scoring_team, opposing_team );     // show Match Win!
            scoring_team->setGameWin( 0 );
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            scoring_team->setSets( 0 );
            opposing_team->setSets( 0 );
            scoring_team->setSets( gameState, 0 );
            opposing_team->setSets( gameState, 0 );  // set Green Team serve to START_SERVE_NUMBER.  Set Red Team serve to 0.
            scoring_team->number()  == GREEN_TEAM ? scoring_team->setServe(  START_SERVE_NUMBER )  : scoring_team->setServe(  0 );
            scoring_team->number()  == RED_TEAM   ? opposing_team->setServe( START_SERVE_NUMBER )  : opposing_team->setServe( 0 );
            gameState->setCurrentSet( 1 );
            gameState->setTeamASets( 0 );
            gameState->setTeamBSets( 0 );
            // scoreBoard->update();   // we are going to sleep mode next, do not update.  screen should freeze 082625
        } else {
            scoreBoard->showSetWinner( scoring_team, opposing_team );
            scoring_team->setGameWin( 0 );
            scoring_team->setPoints(  0 );
            opposing_team->setPoints( 0 );
            scoring_team->setSets(  gameState, scoring_team->getSets());
            opposing_team->setSets( gameState, opposing_team->getSets());
            this->updateFreshServes( scoring_team, opposing_team ); 
            scoreBoard->update();
        }
    } else if( scoring_team_points == _maxPoints ) {
        scoring_team->setGameWin( 1 );
        int new_set_value = scoring_team->getSets() + 1;
        scoring_team->setSets( gameState, new_set_value );
        scoring_team->setSets( new_set_value );
        scoring_team->number()  == GREEN_TEAM ? gameState->setTeamASets( new_set_value ) : gameState->setTeamBSets( new_set_value );
        gameState->setCurrentSet( gameState->getCurrentSet() + 1 );
        if ( scoring_team->getSets() > 1 ) {
            _turnOffServeBars( scoring_team, opposing_team, scoreBoard );
            scoreBoard->showMatchWinner( scoring_team, opposing_team );
            scoring_team->setGameWin( 0 );
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            scoring_team->setSets( 0 );
            opposing_team->setSets( 0 );
            scoring_team->setSets( gameState, 0 );
            opposing_team->setSets( gameState, 0 );  // set Green Team serve to START_SERVE_NUMBER.  Set Red Team serve to 0.
            scoring_team->number()  == GREEN_TEAM ? scoring_team->setServe(  START_SERVE_NUMBER )  : scoring_team->setServe(  0 );
            scoring_team->number()  == RED_TEAM   ? opposing_team->setServe( START_SERVE_NUMBER )  : opposing_team->setServe( 0 );
            gameState->setCurrentSet( 1 );
            gameState->setTeamASets( 0 );
            gameState->setTeamBSets( 0 );
            scoreBoard->update();
        } else {
            scoreBoard->showSetWinner( scoring_team, opposing_team );
            scoring_team->setGameWin( 0 );
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            scoring_team->setSets( gameState, scoring_team->getSets() );
            opposing_team->setSets( gameState, opposing_team->getSets() );
            this->updateFreshServes( scoring_team, opposing_team ); }
        scoreBoard->update();
    }}

void Rules::updateFreshServes( Team* scoring_team, Team* opposing_team ) {
    if ( scoring_team->number() == 1 ) {            // is this team a?
        scoring_team->setServe( _freshServes );
        opposing_team->setServe( 0 );
    } else {                                        // if not, the opposing team is...
        opposing_team->setServe( _freshServes );
        scoring_team->setServe( 0 );
    }}

void Rules::_turnOffServeBars( Team* green, Team* red, ScoreBoard* scoreBoard ) {
    green->setServe( 0 ); red->setServe( 0 );
    scoreBoard->update();
}
int Rules::getFreshServes() { return _freshServes; }
void Rules::setFreshServes( int freshServes ) { _freshServes = freshServes; }
int Rules::getPointsToWin() { return _pointsToWin; }
void Rules::setPointsToWin( int pointsToWin ) { _pointsToWin = pointsToWin; }
int Rules::getMaxPoints() { return _maxPoints; }
void Rules::decrementServe( Team* team ) {
    std::cout << "decrementing serve for team: " << team->number() << std::endl;
    if( team->getServe() > 1 ) {
        team->setServe( team->getServe() - 1 );
    } else {
        team->getOpposingTeam()->setServe( this->getFreshServes());
        team->setServe( 0 );
    }
}
void Rules::setMaxPoints( int points ) { _maxPoints = points; }
