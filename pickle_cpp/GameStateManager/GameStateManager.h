/*
 * GameStateManager h 
 */
#ifndef GameStateManager_h
#define GameStateManager_h
#include "../GameState/GameState.h"
#include "../Logger/Logger.h"
#include "../History/History.h"
#include "../ScoreBoard/ScoreBoard.h"
#if defined _WIN32
#include <string>
#endif

class GameStateManager {
 public:
    GameStateManager( ScoreBoard* scoreBoard );
    ~GameStateManager(); 
    void saveGameState( GameState* gameState, Team* team_a, Team* team_b, History& history );
    void setPreviousGameState( GameState* gameState, Team* team_a, Team* team_b, History& history );

 private:
    Logger* _logger;
    ScoreBoard* _scoreBoard;
};

#endif
