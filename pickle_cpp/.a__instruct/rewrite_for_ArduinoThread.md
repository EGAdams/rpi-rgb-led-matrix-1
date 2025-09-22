# Persona
World-class C++ Developer
Expert user of the Arduino IDE and compiler

# Your task
The following code compiles fine on my system, but doesn't compile in the Ardruino IDE because the Arduino IDE doesn't support the std::thread library.  One of the solutions is to use the ArduinoThread library.  Your task is to rewrite the following code to use the ArduinoThread library.  Use your browsing tool to find the ArduinoThread library and read the documentation to understand how to use it.  You will need to install the library and include the necessary header file(s) in your code.  You will also need to change the code to use the ArduinoThread library instead of std::thread.

## Source Code for the ZeroCondition class
```cpp
// ZeroCondition.cpp
#include "ZeroCondition.h"
#include "GameTimer.h"
#define ZERO_CONDITION_BLINK_DELAY 1000

/** class ZeroCondition */
ZeroCondition::ZeroCondition( PinInterface* pinInterface, std::atomic<bool>& noScores )
    : _pinInterface( pinInterface ), _noScores( noScores ) {}

ZeroCondition::~ZeroCondition() { stop( ); }

void ZeroCondition::run() {
    _thread = std::thread([this]() {
        while ( _noScores ) {
            _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, 1 );
            _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, 1 );
            GameTimer::gameDelay( ZERO_CONDITION_BLINK_DELAY  );
            _pinInterface->pinDigitalWrite( TEAM_A_POINT_0, 0 );
            _pinInterface->pinDigitalWrite( TEAM_B_POINT_0, 0 );
            GameTimer::gameDelay( ZERO_CONDITION_BLINK_DELAY  );
        }
    } );
}

void ZeroCondition::stop() { 
    if (_thread.joinable()) { _thread.join( ); }
}
```

## Usage in Scoreboard.cpp
```cpp
void ScoreBoard::startZeroScoreCondition() {
    _zeroCondition->run();
}

void ScoreBoard::stopZeroScoreCondition() {
    _noScores = false;
    _zeroCondition->stop();
}
```

## Source code for Rules object usage
```cpp
/** @class Rules */
#include "Rules.h"
#include "ScoreBoard.h"

Rules::Rules( int freshServes ) : _freshServes( freshServes ) {}
Rules::~Rules(){};

void Rules::checkGameState( Team* scoring_team, Team* opposing_team, ScoreBoard* scoreBoard ) {
    int scoring_team_points = scoring_team->getPoints();
    int opposing_team_points = opposing_team->getPoints();
    if ( scoring_team_points >= _pointsToWin && scoring_team_points - 1 > opposing_team_points ) {
        scoring_team->setGameWin( 1 );           // got points to win and leading by two points
        scoring_team->setSets( scoring_team->getSets() + 1 );        // increment the set count
        if ( scoring_team->getSets() > 1 ) {                          // if it's a match win...
            scoreBoard->showMatchWinner( scoring_team, opposing_team );   // show the match win
            scoring_team->setGameWin( 0 );                           // start reseting the match
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            scoring_team->setSets( 0 );
            opposing_team->setSets( 0 );  // after this, reset the serve to start on the green team
            scoring_team->number()  == 1 ? scoring_team->setServe(  _freshServes ) : opposing_team->setServe( _freshServes );
            scoring_team->number()  == 1 ? opposing_team->setServe( 0            ) : scoring_team->setServe(  0            );
            scoreBoard->update( scoring_team, opposing_team );
        } else {                                                // this is just a normal set win...
            scoreBoard->showSetWinner( scoring_team, opposing_team ); // show the set win
            scoring_team->setGameWin( 0 );                            // start reseting the game
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            this->updateFreshServes( scoring_team, opposing_team );
        }
        scoreBoard->update( scoring_team, opposing_team ); 
        scoreBoard->startZeroScoreCondition();
    } else if( scoring_team_points == _maxPoints ) {
        scoring_team->setGameWin( 1 );      // the scoring team wins by reaching the maximum points
        scoring_team->setSets( scoring_team->getSets() + 1 );
        if ( scoring_team->getSets() > 1 ) {                              // if it's a match win...
            scoreBoard->showMatchWinner( scoring_team, opposing_team );   // show the match win
            scoring_team->setGameWin( 0 );                                // start reseting the match
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            scoring_team->setSets( 0 );
            opposing_team->setSets( 0 );  // after this, reset the serve to start on the green team
            scoring_team->number()  == 1 ? scoring_team->setServe(  _freshServes ) : opposing_team->setServe( _freshServes );
            scoring_team->number()  == 1 ? opposing_team->setServe( 0            ) : scoring_team->setServe(  0            );
            scoreBoard->update( scoring_team, opposing_team );
        } else {
            scoreBoard->showSetWinner( scoring_team, opposing_team );    // show the set win
            scoring_team->setGameWin( 0 );                               // start reseting the game
            scoring_team->setPoints( 0 );
            opposing_team->setPoints( 0 );
            this->updateFreshServes( scoring_team, opposing_team ); }
        scoreBoard->update( scoring_team, opposing_team );
        scoreBoard->startZeroScoreCondition();
    }
}

void Rules::updateFreshServes( Team* scoring_team, Team* opposing_team ) {
    if ( scoring_team->number() == 1 ) {            // is this team a?
        scoring_team->setServe( _freshServes );
        opposing_team->setServe( 0 );
    } else {                                        // if not, the opposing team is...
        opposing_team->setServe( _freshServes );
        scoring_team->setServe( 0 );
    }}

int Rules::getFreshServes() { return _freshServes; }
void Rules::setFreshServes( int freshServes ) { _freshServes = freshServes; }
int Rules::getPointsToWin() { return _pointsToWin; }
void Rules::setPointsToWin( int pointsToWin ) { _pointsToWin = pointsToWin; }
int Rules::getMaxPoints() { return _maxPoints; }
void Rules::setMaxPoints( int points ) { _maxPoints = points; }
```