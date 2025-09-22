#ifndef Player_h
#define Player_h

#include "../Arduino/Arduino.h"
#include "../GameState/GameState.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include <map>

class Player {
 public:
  Player(int player_number);
  ~Player();
  void setPoints(int points);
  int getPoints();
  void setGames(int games);
  int getGames();
  void setSets( GameState* gameState, int sets );
  int getSets();
  void setMatches(int matches);
  int getMatches();
  void setMode(int mode);
  int getMode();
  void setSetting(int setting);
  int getSetting();
  int incrementSetting();
  int number();
  void setSetHistory( int set, int games );
  void setSetHistory( std::map<int, int> set_history );
  std::map<int,int> getSetHistory();

 private:
  int _playerNumber;
  int _points;
  int _games;
  int _sets;
  int _matches;
  int _mode;
  int _setting;
  std::map<int, int> _set_history;
  GameState* _gameState;
  Player* _opponent;
};

#endif
