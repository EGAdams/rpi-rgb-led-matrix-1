#include "RemotePairingScreen.h"
#include <iostream>

RemotePairingScreen::RemotePairingScreen( ScoreBoard* scoreboard, Team* team_a, Team* team_b )
    : _green_player_paired( false ), _red_player_paired( false ), _scoreboard( scoreboard ), _team_a(team_a), _team_b(team_b) {}

RemotePairingScreen::~RemotePairingScreen(){}

void RemotePairingScreen::draw() {
    if (!_green_player_paired) {
        if ( _scoreboard->onRaspberryPi()) {
            #define LEFT_MARGIN 8
            _scoreboard->clearScreen();
            _scoreboard->drawNewText( "Green",  LEFT_MARGIN + 1, 17  );
            _scoreboard->drawNewText( "Player", LEFT_MARGIN + 0, 34  );
            _scoreboard->drawNewText( "Press",  LEFT_MARGIN + 1, 51  );
            _scoreboard->drawNewText( "Your",   LEFT_MARGIN + 2, 68  );
            _scoreboard->drawNewText( "Remote", LEFT_MARGIN + -8, 85 );
            _scoreboard->drawNewText( "Green",  LEFT_MARGIN + 1, 102 );
            _scoreboard->drawNewText( "Button", LEFT_MARGIN + 0, 119 );
        } else {
            std::cout << "     Green Player\n";
            std::cout << "     Player\n";
            std::cout << "Press your Remote\n";
            std::cout << "     Green button\n\n";  
        }
    }
    GameTimer::gameDelay( 1000 );
    if (!_red_player_paired) {
        if( _scoreboard->onRaspberryPi()) {
            #define LEFT_MARGIN 8
            _scoreboard->clearScreen();
            _scoreboard->drawNewText( "Red",    LEFT_MARGIN + 4, 17  );
            _scoreboard->drawNewText( "Player", LEFT_MARGIN + 0, 34  );
            _scoreboard->drawNewText( "Press",  LEFT_MARGIN + 1, 51  );
            _scoreboard->drawNewText( "Your",   LEFT_MARGIN + 2, 68  );
            _scoreboard->drawNewText( "Remote", LEFT_MARGIN + -8, 85 );
            _scoreboard->drawNewText( "Red",    LEFT_MARGIN + 4, 102 );
            _scoreboard->drawNewText( "Button", LEFT_MARGIN + 0, 119 );
        } else {
            std::cout << "     Red Player\n";
            std::cout << "Press your Remote\n";
            std::cout << "     Red button\n\n";
        }
    }
    GameTimer::gameDelay( 1000 );
}

void RemotePairingScreen::greenPlayerPressed() {
    _green_player_paired = true;
    clearGreenPlayerText();
    if (_red_player_paired) {
        clearAllText();
    }
}

void RemotePairingScreen::redPlayerPressed() {
    _red_player_paired = true;
    clearRedPlayerText();
    if (_green_player_paired) {
        clearAllText();
    }
}

bool RemotePairingScreen::inPairingMode() const {
    return !(_green_player_paired && _red_player_paired);
}

void RemotePairingScreen::clearGreenPlayerText() {
    std::cout << "Green Player paired, clearing green player text...\n";
}

void RemotePairingScreen::clearRedPlayerText() {
    std::cout << "Red Player paired, clearing red player text...\n";
}

void RemotePairingScreen::clearAllText() {
    std::cout << "Both players paired, starting the game...\n";
    // check _scoreboad for null pointer
    if (_scoreboard != nullptr) {
        rgb_matrix::Color yellow_color( 255, 255, 0 );
        print( "setting drawer foreground to yellow... " );
        _scoreboard->setPairingDrawerForegroundColor( yellow_color );
        print( "scoreboard is not null here.  clearing screen. " );
        _scoreboard->clearScreen();
        print( "done clearing screen.  drawing new text... " );
        _scoreboard->drawPairingText( "Paired", 9, 65 );
        print( "done drawing new text." );
        GameTimer::gameDelay( 3000 );
        _scoreboard->clearScreen();
        _scoreboard->update();
    } else {
        print( "*** ERROR: the scoreboard is null here! ***" );
    }
}

void RemotePairingScreen::enablePairingMode() {
    std::cout << "Enabling pairing mode...\n";
    _green_player_paired = false;
    _red_player_paired = false;
}