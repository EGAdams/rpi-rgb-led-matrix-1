/*
 * GameStateManager implementation
 */
#include "GameStateManager.h"

GameStateManager::GameStateManager( ScoreBoard* scoreBoard ) : _scoreBoard( scoreBoard ) {}
GameStateManager::~GameStateManager(){}

void GameStateManager::saveGameState( GameState* gameState, Team* scoring_team, Team* opposing_team, History& history ) {
    std::cout << "inside save game state manager save game state." << std::endl;
    GameState saveState;
    std::cout << "setting team a points... " << std::endl;
    saveState.setTeamAPoints( scoring_team->number() == 1 ? scoring_team->getPoints() : opposing_team->getPoints());
    saveState.setTeamBPoints( scoring_team->number() == 2 ? scoring_team->getPoints() : opposing_team->getPoints());
    saveState.setTeamAServe(  scoring_team->number() == 1 ? scoring_team->getServe()  : opposing_team->getServe()  );
    saveState.setTeamBServe(  scoring_team->number() == 2 ? scoring_team->getServe()  : opposing_team->getServe()  );
    saveState.setTeamASets(   scoring_team->number() == 1 ? scoring_team->getSets()   : opposing_team->getSets()   );
    saveState.setTeamBSets(   scoring_team->number() == 2 ? scoring_team->getSets()   : opposing_team->getSets()   );
    saveState.setRotaryChange( gameState->getRotaryChange());
    saveState.setRotaryPosition( gameState->getRotaryPosition());
    saveState.setPrevRotaryPosition( gameState->getPrevRotaryPosition());
    saveState.setMatchWinTime( gameState->getMatchWinTime());
    std::cout << "pushing saveState onto history stack... " << std::endl;
    std::cout << "history size before push: " << history.size() << std::endl;
    history.push( saveState ); 
    std::cout << "done pushing saveState onto history stack." << std::endl;
}

void GameStateManager::setPreviousGameState( GameState* gameState, Team* team_a, Team* team_b, History& history ) {
    std::cout << "*** DEBUG: setPreviousGameState ENTRY ***" << std::endl;
    std::cout << "*** DEBUG: setPreviousGameState received History reference: " << &history << std::endl;
    
    if ( history.size() == 0 ) {
        std::cout << "*** DEBUG: History is empty, returning ***" << std::endl;
        return;
    }
    
    std::cout << "*** DEBUG: About to call history.pop() on address: " << &history << std::endl;
    GameState savedState = history.pop();
    std::cout << "*** DEBUG: Successfully assigned result of history->pop() to savedState ***" << std::endl;
    team_a->setPoints( savedState.getTeamAPoints());
    team_b->setPoints( savedState.getTeamBPoints());
    team_a->setServe( savedState.getTeamAServe());
    team_b->setServe( savedState.getTeamBServe()); 

    // use BOTH overloaded setSets here! // 081625
    std::cout << "*** setting team A sets to: " << savedState.getTeamASets() << std::endl;
    team_a->setSets( gameState, savedState.getTeamASets());
    team_a->setSets( savedState.getTeamASets());
    gameState->setTeamASets( savedState.getTeamASets());
    std::cout << "*** setting team B sets to: " << savedState.getTeamBSets() << std::endl;
    team_b->setSets( gameState, savedState.getTeamBSets());
    team_b->setSets( savedState.getTeamBSets());
    gameState->setTeamBSets( savedState.getTeamBSets());
    gameState->setRotaryChange( savedState.getRotaryChange());
    gameState->setRotaryPosition( savedState.getRotaryPosition());
    gameState->setPrevRotaryPosition( savedState.getPrevRotaryPosition());
    gameState->setMatchWinTime( savedState.getMatchWinTime());
    _scoreBoard->update();
}
