#include "Player.h"
#include "../Arduino/Arduino.h"

Player::Player(int playerNumber) : _playerNumber(playerNumber) {
  _points = 0;
  _games = 0;
  _sets = 0;
  _matches = 0;
  _mode = 0;
  _setting = 0;
}
Player::~Player() {}

void Player::setPoints(int points) {
  _points = points;
}
int Player::getPoints() {
  return _points;
}

void Player::setGames(int games) {
  _games = games;
}
int Player::getGames() {
  return _games;
}

void Player::setSetHistory( std::map<int, int> set_history ) { _set_history = set_history; }

void Player::setSetHistory( int set, int games ) {
    // std::cout << "setting set history for player " << number() << " set " << set << " to " << games << std::endl;
    if ( set > SETS_TO_WIN_MATCH ) {
        std::cerr << "*** ERROR: in Player: set > SETS_TO_WIN_MATCH in Player::setSetHistory(). ***" << std::endl;
        exit( 1 );
    }
    _set_history[ set ] = games; 
    number() == 1 ?                      // if this is player 1
        _gameState->setPlayer1SetHistory( _set_history ):   // then set player 1's set  history
        _gameState->setPlayer2SetHistory( _set_history ); } // else set player 2's set history

std::map< int, int> Player::getSetHistory() { return _set_history; }

void Player::setSets( GameState* gameState, int sets ) {                          // sets this player's sets
    this->setSetHistory(      gameState->getCurrentSet(), _games               ); // and set history for both
    _opponent->setSetHistory( gameState->getCurrentSet(), _opponent->getGames()); // players
    if( _playerNumber == 1 ) { 
        if (gameState) {
            gameState->setPlayer1SetHistory(this->getSetHistory());
            gameState->setPlayer2SetHistory(_opponent->getSetHistory());
        }
    } else {
        if (gameState) {
            gameState->setPlayer1SetHistory(_opponent->getSetHistory());
            gameState->setPlayer2SetHistory(this->getSetHistory());
        }
    }
    _sets = sets;
}  

int Player::getSets() {
  return _sets;
}

void Player::setMatches(int matches) {
  _matches = matches;
}
int Player::getMatches() {
  return _matches;
}

void Player::setMode(int mode) {
  _mode = mode;
}
int Player::getMode() {
  return _mode;
}

void Player::setSetting(int setting) {
  _setting = setting;
}
int Player::getSetting() {
  return _setting;
}

int Player::incrementSetting() {
  return ++_setting;
}

int Player::number() {
  return _playerNumber;
}
