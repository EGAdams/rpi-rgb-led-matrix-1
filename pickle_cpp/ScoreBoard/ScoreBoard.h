#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "../FontLoader/FontLoader.h"
#include "../SetDrawer/SetDrawer.h"
#include "../Drawer/Drawer.h"
#include "../FontManager/FontManager.h"
#include "../ColorManager/ColorManager.h"
#include "../IDisplay/IDisplay.h"
#include "../CanvasCreator/CanvasCreator.h"
#include "../Arduino/Arduino.h"
#include "../LedBlinker/LedBlinker.h"
#include "../GameState/GameState.h"
#include "../GameTimer/GameTimer.h"
#include "../PinInterface/PinInterface.h"
#include "../Logger/Logger.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../ShowMatchWin/ShowMatchWin.h"
#include "../Team/Team.h"
#include "../Rules/Rules.h"
#include "../MatchWinSequence/MatchWinSequence.h"
#include "../ClockTimer/ClockTimer.h"
#include <iostream>

// for getting the current dir
#include <unistd.h>
#include <limits.h>

#define BIG_NUMBER_FONT     "fonts/fgm_27_ee.bdf"
#define LITTLE_NUMBER_FONT "fonts/the_sets_font.bdf"
#define PLAYER_1_INITIALIZED 1 // what _team_a begins with.  maybe 0?
#define PLAYER_2_INITIALIZED 2

#define RED    2
#define GREEN  5
#define BLUE   6
#define YELLOW 4
#define ORANGE 3
#define PURPLE 7
#define WHITE  9
#define BLACK  0

class ScoreBoard {
 public:
    ScoreBoard( PinInterface* pinInterface, Team* player1, Team* player2, Rules* rules, ClockTimer* clockTimer, GameState* gameState, IDisplay* display,
               FontManager* fontManager, ColorManager* colorManager );

    ~ScoreBoard();
    bool onRaspberryPi();
    void update();
    void showSetWinner( Team* team_a, Team* team_b );
    void showMatchWinner( Team* team_a, Team* team_b );
    void toggleServe( Team* team_a, Team* team_b );
    void setTeamAPointsOn();
    void setTeamBPointsOn();
    void setTeamAPoints( int points );
    void setTeamBPoints( int points );
    void setTeamASets(   int sets   );
    void setTeamBSets(   int sets   );
    void setTeamAServe(  int serve  );
    void setTeamBServe(  int serve  );
    void clearScreen();
    void setDrawerBackgroundColor(const Color& color);
    void setDrawerForegroundColor(const Color& color);
    void setPairingDrawerForegroundColor(const Color& color);
    void drawText(    const std::string& text, int x, int y );
    void drawNewText( const std::string& text, int x, int y );
    void drawPairingText( const std::string& text, int x, int y );
    void drawYellowPeriod();
    std::string drawPlayerScore( Team* scoring_team );
    bool hasCanvas();
    void setLittleDrawerFont( const std::string& font_file );
    void drawSets();
    void setClockTimer(ClockTimer* clockTimer);
    ClockTimer* getClockTimer() const;
    
    private:
    PinInterface* _pinInterface;
    Team*         _team_a;
    Team*         _team_b;
    Rules*        _rules;
    ClockTimer*   _clockTimer;
    rgb_matrix::Font             _big_number_font;
    rgb_matrix::Font             _little_number_font;
    rgb_matrix::Font             _period_font;
    rgb_matrix::Font             _pairing_font;
    std::unique_ptr<Drawer>      _drawer;
    std::unique_ptr<Drawer>      _smallDrawer;
    std::unique_ptr<Drawer>      _playerOneScoreDrawer;
    std::unique_ptr<Drawer>      _playerTwoScoreDrawer;
    std::unique_ptr<Drawer>      _pipeDrawer;
    std::unique_ptr<Drawer>      _bluePipeDrawer;
    std::unique_ptr<Drawer>      _redPipeDrawer;
    std::unique_ptr<Drawer>      _greenPipeDrawer;
    std::unique_ptr<Drawer>      _blankPipeDrawer;
    std::unique_ptr<Drawer>      _redPeriodDrawer;
    std::unique_ptr<Drawer>      _greenPeriodDrawer;
    std::unique_ptr<Drawer>      _yellowPeriodDrawer;
    std::unique_ptr<Drawer>      _blankPeriodDrawer;
    std::unique_ptr<SetDrawer>   _setDrawer;
    std::unique_ptr<RGBMatrix>   _canvas;
    std::unique_ptr<Drawer>      _new_drawer;
    std::unique_ptr<Drawer>      _text_drawer;
    std::unique_ptr<Drawer>      _pairing_text_drawer;
    const char*                  _font_file;
    
    void _drawTieBreakScore(    Player* player        );
    void _drawMatchWinDisplay(  /* void */            );
    int  _characterOffset(      std::string character );
    int  _firstCharacterOffset( std::string character );
    std::string _translate(     int raw_score         );
    GameState*                  _gameState;
    IDisplay*                   _display;
    FontManager*                _fontManager;
    ColorManager*               _colorManager;
};

#endif
