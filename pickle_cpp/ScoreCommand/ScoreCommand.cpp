#include "ScoreCommand.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../Arduino/Arduino.h"
#include "../GameState/GameState.h"
#include "../ScoreBoard/ScoreBoard.h"

ScoreCommand::ScoreCommand( GameState* gameState, Team* scoringTeam,
    Rules* rules, ScoreBoard* scoreBoard,
    History& history ) : _game_state( gameState ),
    _scoringTeam( scoringTeam ),
    _rules( rules ),
    _scoreBoard( scoreBoard ),
    _history( &history ) {
    _gameStateManager = new GameStateManager( scoreBoard );
}
ScoreCommand::~ScoreCommand() {};
void ScoreCommand::undo() {}

void ScoreCommand::execute() {
    std::cout << "entered ScoreCommand::execute()... " << std::endl;
    std::cout << "using game state manager to save game state..." << std::endl;

    std::cout << "save final match duration to GameState ( in seconds ) in case of Match win. " << std::endl;
    ClockTimer* clockTimer = _scoreBoard->getClockTimer();
    int finalMatchTimeSeconds = clockTimer->getElapsedSeconds();
    _game_state->setMatchWinTime( static_cast<std::chrono::duration<double>>( finalMatchTimeSeconds ));
    std::cout << "*** Match time saved: " << finalMatchTimeSeconds << " seconds ***" << std::endl;

    _gameStateManager->saveGameState( _game_state, _scoringTeam, _scoringTeam->getOpposingTeam(), *_history );
    std::cout << "inside score command again getting scoring teams serve status... " << std::endl;
    if ( _scoringTeam->getServe() > 0 ) {  // if this team is serving...
        std::string color = "GREEN"; if ( _scoringTeam->number() == 2 ) { color = "RED"; }
        std::cout << "incrementing points for the " << color << " team... " << std::endl;
        std::cout << "incrementing scoring team points..." << std::endl;
        _scoringTeam->setPoints( _scoringTeam->getPoints() + 1 );
        std::cout << "updating scoreboard... " << std::endl;
        _scoreBoard->update();
        std::cout << "done updating scoreboard.  checking game state..." << std::endl;
        _rules->checkGameState( _game_state, _scoringTeam, _scoringTeam->getOpposingTeam(), _scoreBoard );
    } else {  // serve is zero.  no score, but decrementing opponents serves...
        std::string color = "GREEN"; if ( _scoringTeam->getOpposingTeam()->number() == 2 ) { color = "RED"; }
        std::cout << "decrementing serve for the " << color << " team... " << std::endl;
        _rules->decrementServe( _scoringTeam->getOpposingTeam());
        if ( _scoringTeam->number() == 1 ) {
            std::cout << "green serves: " << _scoringTeam->getServe()                    << std::endl;
            std::cout << "red serves  : " << _scoringTeam->getOpposingTeam()->getServe() << std::endl;
        } else {
            std::cout << "green serves: " << _scoringTeam->getOpposingTeam()->getServe() << std::endl;
            std::cout << "red serves  : " << _scoringTeam->getServe()                    << std::endl;
        }
        _scoreBoard->update();
    }
}
