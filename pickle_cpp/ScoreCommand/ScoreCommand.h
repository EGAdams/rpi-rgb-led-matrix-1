#ifndef ScoreCommand_h
#define ScoreCommand_h

/*
 * ScoreCommand
 */
#include "../Command/Command.h"

#include "../Rules/Rules.h"
#include "../Arduino/Arduino.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../History/History.h"
#include "../GameStateManager/GameStateManager.h"


class ScoreCommand: Command {
    public:
        ScoreCommand( GameState* gameState, Team* scoringTeam, Rules* rules, ScoreBoard* scoreBoard, History& history );
        ~ScoreCommand();
        void execute();
        void undo();
    private:
        GameState*  _game_state;
        Team*       _scoringTeam;
        Rules*      _rules;
        ScoreBoard* _scoreBoard;
        History*    _history;
        GameStateManager* _gameStateManager;
};

#endif // ScoreCommand_h
