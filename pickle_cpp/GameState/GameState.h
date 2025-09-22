#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../Arduino/Arduino.h"
#include "../Team/Team.h"
#include "../Rules/Rules.h"
#include "../Subject/Subject.h"
#include <map>


class GameState : public Subject {
 public:
    enum class GamePlayStates {
        REMOTE_PAIRING = 0,
        REGULAR_GAME_PLAY // New state added
    };

    // GameState();
    GameState();
    GameState(const GameState& other); // Copy constructor
    GameState(GameState&& other) noexcept; // Move constructor
    GameState& operator=(const GameState& other); // Assignment operator
    GameState& operator=(GameState&& other) noexcept; // Move assignment operator
    ~GameState();
    int getCurrentAction();
    void setCurrentAction( int new_action );
    int getState();
    void setState( int new_state );
    void setServe(int serve);
    int getServe();
    void setPlayerButton(int playerButton);
    int getPlayerButton();
    void setStarted(int started);
    int getStarted();
    void setRotaryChange(int rotaryChange);
    int getRotaryChange();
    void setPreviousTime(unsigned long previousTime);
    int getRotaryPosition() const;
    int getRotaryPosition();
    void setRotaryPosition(int rotaryPosition);
    void setPrevRotaryPosition(int prevRotaryPosition);
    int getPrevRotaryPosition() const;
    int getPrevRotaryPosition();
    unsigned long getPreviousTime();
    void setNow(unsigned long now);
    unsigned long getNow();
    int getGamePlayState();
    void decrementServe( Team* team, Rules* rules );
    void setUndoFlag( int undoFlag );
    int getUndoFlag();
    void setTeamAPoints( int points );
    int getTeamAPoints() const;
    int getTeamAPoints();
    void setTeamBPoints( int points );
    int getTeamBPoints() const;
    int getTeamBPoints();
    void setTeamAServe( int serve );
    int getTeamAServe() const;
    int getTeamAServe();
    void setTeamBServe( int serve );
    int getTeamBServe() const;
    int getTeamBServe();
    void setTeamASets( int sets );
    int getTeamASets() const;
    int getTeamASets();
    void setTeamBSets( int sets );
    int getTeamBSets() const;
    int getTeamBSets();
    void setGamePlayState( int game_play_state );
    bool gameRunning();
    void setGameRunning();
    std::map<int, int> getPlayer1SetHistory();
    void setPlayer1SetHistory( std::map<int, int> player1_set_history );
    std::map<int, int> getPlayer2SetHistory();
    void setPlayer2SetHistory( std::map<int, int> player2_set_history );
    int getCurrentSet() const;
    void setCurrentSet( int new_set_value );
    int getGameMode() const;
    void setGameMode( int new_game_mode );

    std::chrono::duration<double> getMatchWinTime();                        // Match Win Time
    void setMatchWinTime( std::chrono::duration<double> match_win_time );
    std::chrono::duration<double> getMatchWinTime() const;
    
    private:
    std::chrono::duration<double> _match_win_time;
    int _game_mode;
    int _current_action;
    int _state;
    unsigned long _now;
    bool _game_running;
    int _serve;
    int _player_button;
    int _started;
    int _rotary_change;
    unsigned long _previous_time;
    int _rotary_position;
    int _prev_rotary_position;
    int _team_a_points;
    int _team_b_points;
    int _team_a_serve;
    int _team_b_serve;
    int _team_a_sets;
    int _team_b_sets;
    int _undo_flag;
    int _game_play_state;
    int _current_set;
    std::map<int, int> _player1_set_history;
    std::map<int, int> _player2_set_history;
};

#endif
