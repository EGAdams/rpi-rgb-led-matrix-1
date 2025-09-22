#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "../Arduino/Arduino.h"
#include "../GAME_MODES/GAME_MODES.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../INPUTS/INPUTS.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"
#include "../Team/Team.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../Logger/Logger.h"
#include "../History/History.h"
#include "../BlinkController/BlinkController.h"
#include "../ConsoleDisplay/ConsoleDisplay.h"
#include <memory>

class GameObject {
public:
    GameObject(
        Team* team_a,
        Team* team_b,
        PinState* pinState,
        PinInterface* pinInterface,
        GameState* gameState,
        GameTimer* gameTimer,
        Inputs* gameInputs,
        GameModes* gameModes,
        ScoreBoard* scoreBoard );
    GameObject();
    ~GameObject();

    bool gameRunning();

    void start();
    PinInterface* getPinInterface();
    void loopGame();
    GameState* getGameState();

    // New methods to fix errors
    void playerScore(int player);
    void undo();
    void resetMatch();

    // Getters
    Team* getTeamA();
    Team* getTeamB();
    PinState* getPinState();
    GameTimer* getGameTimer();
    Inputs* getGameInputs();
    GameModes* getGameModes();
    ScoreBoard* getScoreBoard();
    Rules* getRules();
    Reset* getReset();
    std::shared_ptr<History> history() const;
    Logger* getLogger();
    BlinkController* getBlinkController();

    // Setters
    void setTeamA(Team* team);
    void setTeamB(Team* team);
    void setPinState(PinState* pinState);
    void setPinInterface(PinInterface* pinInterface);
    void setGameState(GameState* gameState);
    void setGameTimer(GameTimer* gameTimer);
    void setGameInputs(Inputs* inputs);
    void setGameModes(GameModes* gameModes);
    void setScoreBoard(ScoreBoard* scoreBoard);
    void setRules(Rules* rules);
    void setReset(Reset* reset);
    void setHistory(std::shared_ptr<History> history);
    void setLogger(Logger* logger);
    void setBlinkController(BlinkController* blinkController);

private:
    Team* _team_a;
    Team* _team_b;
    PinState* _pinState;
    PinInterface* _pinInterface;
    GameState* _gameState;
    GameTimer* _gameTimer;
    Inputs* _gameInputs;
    GameModes* _gameModes;
    ScoreBoard* _scoreBoard;
    Rules* _rules;
    Reset* _reset;
    std::shared_ptr<History> _history;
    Logger* _logger;
    BlinkController* _blinkController;
};

#endif
