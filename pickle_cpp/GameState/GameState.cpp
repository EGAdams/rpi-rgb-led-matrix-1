#include "GameState.h"
#include "../Arduino/Arduino.h"

GameState::~GameState() {
    std::cout << "*** DEBUG: GameState destructor ENTRY, this=" << this << std::endl;
    try {
        std::cout << "*** DEBUG: GameState destructor - about to call Subject destructor ***" << std::endl;
        // Subject destructor will be called automatically
        std::cout << "*** DEBUG: GameState destructor - after Subject destructor call ***" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "*** CRITICAL ERROR: Exception in GameState destructor: " << e.what() << " ***" << std::endl;
    } catch (...) {
        std::cout << "*** CRITICAL ERROR: Unknown exception in GameState destructor ***" << std::endl;
    }
    std::cout << "*** DEBUG: GameState destructor COMPLETE, this=" << this << std::endl;
}

// Copy constructor with extensive debugging
GameState::GameState(const GameState& other) : Subject(other) {
    std::cout << "*** DEBUG: GameState copy constructor ENTRY ***" << std::endl;
    
    std::cout << "*** DEBUG: Copying basic members ***" << std::endl;
    _match_win_time = other._match_win_time;
    _game_mode = other._game_mode;
    _current_action = other._current_action;
    _state = other._state;
    _now = other._now;
    _game_running = other._game_running;
    _serve = other._serve;
    _player_button = other._player_button;
    _started = other._started;
    _rotary_change = other._rotary_change;
    _previous_time = other._previous_time;
    _rotary_position = other._rotary_position;
    _prev_rotary_position = other._prev_rotary_position;
    _team_a_points = other._team_a_points;
    _team_b_points = other._team_b_points;
    _team_a_serve = other._team_a_serve;
    _team_b_serve = other._team_b_serve;
    _team_a_sets = other._team_a_sets;
    _team_b_sets = other._team_b_sets;
    _undo_flag = other._undo_flag;
    _game_play_state = other._game_play_state;
    _current_set = other._current_set;
    
    std::cout << "*** DEBUG: About to copy std::map members ***" << std::endl;
    _player1_set_history = other._player1_set_history;
    std::cout << "*** DEBUG: Copied _player1_set_history ***" << std::endl;
    _player2_set_history = other._player2_set_history;
    std::cout << "*** DEBUG: Copied _player2_set_history ***" << std::endl;
    
    std::cout << "*** DEBUG: GameState copy constructor COMPLETE ***" << std::endl;
}

// Move constructor
GameState::GameState(GameState&& other) noexcept : Subject(std::move(other)) {
    std::cout << "*** DEBUG: GameState move constructor ENTRY ***" << std::endl;
    
    // Move all members
    _match_win_time = std::move(other._match_win_time);
    _game_mode = other._game_mode;
    _current_action = other._current_action;
    _state = other._state;
    _now = other._now;
    _game_running = other._game_running;
    _serve = other._serve;
    _player_button = other._player_button;
    _started = other._started;
    _rotary_change = other._rotary_change;
    _previous_time = other._previous_time;
    _rotary_position = other._rotary_position;
    _prev_rotary_position = other._prev_rotary_position;
    _team_a_points = other._team_a_points;
    _team_b_points = other._team_b_points;
    _team_a_serve = other._team_a_serve;
    _team_b_serve = other._team_b_serve;
    _team_a_sets = other._team_a_sets;
    _team_b_sets = other._team_b_sets;
    _undo_flag = other._undo_flag;
    _game_play_state = other._game_play_state;
    _current_set = other._current_set;
    
    // Move the maps
    _player1_set_history = std::move(other._player1_set_history);
    _player2_set_history = std::move(other._player2_set_history);
    
    std::cout << "*** DEBUG: GameState move constructor COMPLETE ***" << std::endl;
}

// Move assignment operator
GameState& GameState::operator=(GameState&& other) noexcept {
    std::cout << "*** DEBUG: GameState move assignment operator ENTRY ***" << std::endl;
    
    if (this != &other) {
        Subject::operator=(std::move(other));
        
        // Move all members
        _match_win_time = std::move(other._match_win_time);
        _game_mode = other._game_mode;
        _current_action = other._current_action;
        _state = other._state;
        _now = other._now;
        _game_running = other._game_running;
        _serve = other._serve;
        _player_button = other._player_button;
        _started = other._started;
        _rotary_change = other._rotary_change;
        _previous_time = other._previous_time;
        _rotary_position = other._rotary_position;
        _prev_rotary_position = other._prev_rotary_position;
        _team_a_points = other._team_a_points;
        _team_b_points = other._team_b_points;
        _team_a_serve = other._team_a_serve;
        _team_b_serve = other._team_b_serve;
        _team_a_sets = other._team_a_sets;
        _team_b_sets = other._team_b_sets;
        _undo_flag = other._undo_flag;
        _game_play_state = other._game_play_state;
        _current_set = other._current_set;
        
        // Move the maps
        _player1_set_history = std::move(other._player1_set_history);
        _player2_set_history = std::move(other._player2_set_history);
    }
    
    std::cout << "*** DEBUG: GameState move assignment operator COMPLETE ***" << std::endl;
    return *this;
}

// Assignment operator with extensive debugging
GameState& GameState::operator=(const GameState& other) {
    std::cout << "*** DEBUG: GameState assignment operator ENTRY ***" << std::endl;
    
    if (this != &other) {
        std::cout << "*** DEBUG: Calling Subject assignment operator ***" << std::endl;
        Subject::operator=(other);
        
        std::cout << "*** DEBUG: Assigning basic members ***" << std::endl;
        _match_win_time = other._match_win_time;
        _game_mode = other._game_mode;
        _current_action = other._current_action;
        _state = other._state;
        _now = other._now;
        _game_running = other._game_running;
        _serve = other._serve;
        _player_button = other._player_button;
        _started = other._started;
        _rotary_change = other._rotary_change;
        _previous_time = other._previous_time;
        _rotary_position = other._rotary_position;
        _prev_rotary_position = other._prev_rotary_position;
        _team_a_points = other._team_a_points;
        _team_b_points = other._team_b_points;
        _team_a_serve = other._team_a_serve;
        _team_b_serve = other._team_b_serve;
        _team_a_sets = other._team_a_sets;
        _team_b_sets = other._team_b_sets;
        _undo_flag = other._undo_flag;
        _game_play_state = other._game_play_state;
        _current_set = other._current_set;
        
        std::cout << "*** DEBUG: About to assign std::map members ***" << std::endl;
        _player1_set_history = other._player1_set_history;
        std::cout << "*** DEBUG: Assigned _player1_set_history ***" << std::endl;
        _player2_set_history = other._player2_set_history;
        std::cout << "*** DEBUG: Assigned _player2_set_history ***" << std::endl;
    }
    
    std::cout << "*** DEBUG: GameState assignment operator COMPLETE ***" << std::endl;
    return *this;
}

GameState::GameState(){
    _match_win_time = std::chrono::duration<double>( 0.0 );
    _game_mode            = SINGLES_MODE;              // default game mode 080925
    _current_action       = 0;
    _state                = 0;
    _now                  = 0;
    _game_running         = false;
    _serve                = 0;
    _player_button        = 0;
    _started              = 0;
    _rotary_change        = 0;
    _previous_time        = 0;
    _rotary_position      = 0;
    _prev_rotary_position = 0;
    _team_a_points        = 0;
    _team_b_points        = 0;
    _team_a_serve         = 0;
    _team_b_serve         = 0;
    _team_a_sets          = 0;
    _team_b_sets          = 0;
    _undo_flag            = 0;
    _game_play_state      = 5; // start in regular game play after score // 060225
    _current_set          = 0;
};

void GameState::setMatchWinTime( std::chrono::duration<double> match_win_time ) { _match_win_time = match_win_time; }
std::chrono::duration<double> GameState::getMatchWinTime() { return _match_win_time; }
std::chrono::duration<double> GameState::getMatchWinTime() const { return _match_win_time; }

int GameState::getGameMode() const { return _game_mode; }
void GameState::setGameMode( int new_game_mode ) { _game_mode = new_game_mode; }

void GameState::setCurrentAction( int new_action ) { _current_action = new_action; }
int  GameState::getCurrentAction() { return _current_action; }

void GameState::setState(                   int new_state ) { _state = new_state; }
int  GameState::getState()                                  { return _state; }
void GameState::setServe(                    int serve ) { _serve = serve;                                }
int GameState::getServe() {                  return _serve;                                               }
void GameState::setPlayerButton(             int playerButton ) { _player_button = playerButton;           }
int GameState::getPlayerButton() {           return _player_button;                                        }
void GameState::setStarted(                  int started ) {  _started = started;                         }
int GameState::getStarted() {                return _started;                                             }
void GameState::setRotaryChange(             int rotaryChange) { _rotary_change = rotaryChange;            }
int GameState::getRotaryChange() {           return _rotary_change;                                        }
int GameState::getRotaryPosition() const {         return _rotary_position;                                      }
int GameState::getRotaryPosition() {         return _rotary_position;                                      }
void GameState::setRotaryPosition(           int rotaryPosition ) { _rotary_position = rotaryPosition;     }
void GameState::setPrevRotaryPosition(       int prevRotaryPosition ) { 
                                                 _prev_rotary_position = prevRotaryPosition;                }
int GameState::getPrevRotaryPosition() const {     return _prev_rotary_position;                                  }
int GameState::getPrevRotaryPosition() {     return _prev_rotary_position;                                  }
void GameState::setPreviousTime(             unsigned long previous_time_arg ) { _previous_time = previous_time_arg; }
unsigned long GameState::getPreviousTime() { return _previous_time;                                        }
void GameState::setNow(                      unsigned long now ) {  _now = now;                           }
unsigned long GameState::getNow() {          return   _now;                                               }
void GameState::setUndoFlag(                 int undoFlag ) { _undo_flag = undoFlag;                      }
int GameState::getUndoFlag() {               return _undo_flag;                                           }
void GameState::setTeamAPoints(              int points ) { _team_a_points = points;                      }
int GameState::getTeamAPoints() const {            return _team_a_points;                                       }
int GameState::getTeamAPoints() {            return _team_a_points;                                       }
void GameState::setTeamBPoints(              int points ) { _team_b_points = points;                      }
int GameState::getTeamBPoints() const {            return _team_b_points;                                       }
int GameState::getTeamBPoints() {            return _team_b_points;                                       }
void GameState::setTeamAServe(               int serve ) { _team_a_serve = serve;                         }
int GameState::getTeamAServe() const {             return _team_a_serve;                                        }
int GameState::getTeamAServe() {             return _team_a_serve;                                        }
void GameState::setTeamBServe(               int serve ) { _team_b_serve = serve;                         }
int GameState::getTeamBServe() const {             return _team_b_serve;                                        }
int GameState::getTeamBServe() {             return _team_b_serve;                                        }
void GameState::setTeamASets(                int sets ) { _team_a_sets = sets;                            }
int GameState::getTeamASets() const {              return _team_a_sets;                                         }
int GameState::getTeamASets() {              return _team_a_sets;                                         }
void GameState::setTeamBSets(                int sets ) { _team_b_sets = sets;                            }
int GameState::getTeamBSets() const {              return _team_b_sets;                                         }
int GameState::getTeamBSets() {              return _team_b_sets;                                         }
void GameState::setGamePlayState( int new_state ) {
    _game_play_state = new_state;
}

void GameState::setCurrentSet( int new_set_value ) { _current_set = new_set_value; }
int GameState::getCurrentSet() const { return _current_set; }

int GameState::getGamePlayState() {
    return _game_play_state;
}
                                

bool GameState::gameRunning() {
    return _game_running; // Assuming _game_running is set appropriately elsewhere in the class
}

void GameState::setGameRunning() { _game_running = true; }

void GameState::decrementServe( Team* team, Rules* rules ) { 
    if( team->getServe() > 1 ) {
        team->setServe( team->getServe() - 1 );
    } else {
        team->getOpposingTeam()->setServe( rules->getFreshServes() );
        team->setServe( 0 ); }}

std::map<int, int> GameState::getPlayer1SetHistory() { return _player1_set_history; }
void GameState::setPlayer1SetHistory( std::map<int, int> player1_set_history ) {
    _player1_set_history = player1_set_history;
}
std::map<int, int> GameState::getPlayer2SetHistory() {
    return _player2_set_history; }
void GameState::setPlayer2SetHistory( std::map<int, int> player2_set_history ) {
    _player2_set_history = player2_set_history;}
