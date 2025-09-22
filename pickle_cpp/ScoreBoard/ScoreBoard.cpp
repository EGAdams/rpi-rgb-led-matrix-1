/*
 * if you where still wondering where we could have used an interface, here's a good place for one.
 */
#include "ScoreBoard.h"
#include "../Arduino/Arduino.h"
#include <iostream>
#include <stdexcept>

ScoreBoard::ScoreBoard( PinInterface* pinInterface, Team* player1, Team* player2, Rules* rules, ClockTimer* clockTimer,  GameState* gameState, IDisplay* display,
                       FontManager* fontManager, ColorManager* colorManager )
    : _pinInterface( pinInterface ), _team_a( player1 ), _team_b( player2 ), _rules( rules ), _clockTimer( clockTimer ),
    _gameState( gameState ), _display( display ), _fontManager( fontManager ), _colorManager( colorManager ) {
    
    std::cout << "=== SCOREBOARD CONSTRUCTOR START ===" << std::endl;
    std::cout << "Validating input parameters..." << std::endl;
    
    if (!player1) throw std::runtime_error("player1 is null");
    if (!player2) throw std::runtime_error("player2 is null"); 
    if (!gameState) throw std::runtime_error("gameState is null");
    if (!display) throw std::runtime_error("display is null");
    if (!rules) throw std::runtime_error("rules is null");
    
    std::cout << "All input parameters validated successfully" << std::endl;
    if ( onRaspberryPi() == false ) {
        std::cout << "*** NON-RASPBERRY PI MODE ***" << std::endl;
        std::cout << "constructing scoreboard without matrix..." << std::endl;
        try {
            std::cout << "Creating SetDrawer with nullptr canvas for non-Pi mode" << std::endl;
            _setDrawer = std::make_unique<SetDrawer>( nullptr, _gameState, _clockTimer );
            std::cout << "SetDrawer created successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "*** EXCEPTION in non-Pi SetDrawer creation: " << e.what() << std::endl;
            throw;
        }
        std::cout << "=== SCOREBOARD CONSTRUCTOR END (NON-PI) ===" << std::endl;
    } else {
        std::cout << "*** RASPBERRY PI MODE DETECTED ***" << std::endl;
        // const rgb_matrix::Font& defaultFont = _fontManager->getFont( "fonts/fgm_27_ee.bdf" );
        // rgb_matrix::Color defaultColor = _colorManager->getColor( "WHITE" );
        printf( "setting up matrix...\n" );
        // std::cout << "Creating Color objects..." << std::endl;
        Color pipe_color( 255, 255, 0 ); // yellow
        // std::cout << "pipe_color created" << std::endl;
        Color background_color( 0, 0, 0 );
        // std::cout << "background_color created" << std::endl;
        Color player_one_score_color( 0, 255, 0 ); // green
        // std::cout << "player_one_score_color created" << std::endl;
        Color player_two_score_color( 255, 0, 0 ); // red
        // std::cout << "player_two_score_color created" << std::endl;
        Color outline_color( 0, 0, 0 );
        // std::cout << "outline_color created" << std::endl;
        
        // std::cout << "Setting up matrix options..." << std::endl;
        RGBMatrix::Options matrix_options; // seems like the only logical place to create the canvas
        // std::cout << "matrix_options created" << std::endl;
        matrix_options.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
        matrix_options.rows = 64;
        matrix_options.chain_length = 1;
        matrix_options.parallel = 2;
        matrix_options.show_refresh_rate = false;
        matrix_options.disable_hardware_pulsing = true; // --led-no-hardware-pulse    
        
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            print("Current working directory: " + std::string(cwd));
        }
        print( "reading brightness file...\n" );
        std::ifstream brightness_file( "brightness.txt" );
        std::string brightness_string;
        bool brightness_file_read = false;
        if ( brightness_file.is_open()) {
            print( "brightness file is_open().. reading brightness value...\n" );
            std::getline( brightness_file, brightness_string );
            print( "brightness value read: " + brightness_string + " close brightness file...\n" );
            brightness_file.close();
            print( "setting brightness file read to true...\n" );
            brightness_file_read = true;
        } else {
            print( "*** Warning: brightness_file.is_open() must have returned false, using default value... ***" );
        }
        int brightness_value = 35;
        if ( brightness_file_read ) {
            print( "brightness file read, setting brightness value to: " + brightness_string + "\n" );
            brightness_value = std::stoi( brightness_string );
        } else {
            print( "*** Warning: brightness_file_read is false, using default value... ***" );
        }
        print( "brightness value set to: " + std::to_string( brightness_value ));
        matrix_options.brightness = brightness_value; // 35; // 85; // inc jan 22, 22024 // 35 is best for demo videos in largo
        matrix_options.pwm_bits = 11;
        rgb_matrix::RuntimeOptions runtime_opt;
        
        // MARK: Panel Settings
        /* Hot Electronics setting */
        // matrix_options.multiplexing = 1;
        // runtime_opt.gpio_slowdown = 2;
        // matrix_options.pixel_mapper_config = "Rotate:180;264-Mapper"; // or e.g. "U-mapper" 
        // matrix_options.cols = 64;
        
        /* Kerun setting */
        matrix_options.multiplexing     = 0; // from 1
        runtime_opt.gpio_slowdown       = 3; // from 2
        matrix_options.cols             = 128;
        matrix_options.row_address_type = 5;
        // took out pixel mapper
        
        runtime_opt.drop_privileges = 0;
        runtime_opt.daemon = 0;
        runtime_opt.do_gpio_init = 1;
        // std::cout << "*** CREATING CANVAS - CRITICAL SECTION ***" << std::endl;
        try {
            // std::cout << "Creating CanvasCreator..." << std::endl;
            CanvasCreator canvasCreator( matrix_options, runtime_opt );
            // std::cout << "CanvasCreator created successfully" << std::endl;
            
            // std::cout << "Creating canvas via canvasCreator.createCanvas()..." << std::endl;
            _canvas = std::unique_ptr<rgb_matrix::RGBMatrix>( canvasCreator.createCanvas());
            // std::cout << "Canvas created successfully: " << _canvas.get() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "*** EXCEPTION during canvas creation: " << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "*** UNKNOWN EXCEPTION during canvas creation ***" << std::endl;
            throw;
        }

        // std::cout << "*** LOADING FONTS - CRITICAL SECTION ***" << std::endl;
        // start loading fonts...
        // FontLoader smallNumberFontLoader( "fonts/mspgothic_042623.bdf" );       // little numbers
        // rgb_matrix::Font small_number_font;
        // smallNumberFontLoader.LoadFont( small_number_font );
        // # define FONT_FILE "fonts/8x13B.bdf"
        // std::cout << "Loading little number font: " << PB_LITTLE_NUMBER_FONT << std::endl;
        try {
            if ( !_little_number_font.LoadFont( PB_LITTLE_NUMBER_FONT )) { // massacre aniversary 090724
                std::cerr << "*** ERROR: Could not load font '" << PB_LITTLE_NUMBER_FONT << "' ***" << std::endl;
                throw std::runtime_error("Failed to load little number font");
            }
            // std::cout << "Little number font loaded successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "*** EXCEPTION during little font loading: " << e.what() << std::endl;
            throw;
        }
        // if ( !_little_number_font.LoadFont( LITTLE_NUMBER_FONT )) {
        //     fprintf( stderr, "*** ERROR: Could not load font '%s' ***\n", LITTLE_NUMBER_FONT ); exit( 1 );
        // }

        // std::cout << "Loading big number font: " << PB_BIG_NUMBER_FONT << std::endl;
        try {
            // FontLoader bigNumberFontLoader( "fonts/fgm_27_ee.bdf" );     // big numbers
            // FontLoader bigNumberFontLoader( "fonts/jan_8_bar.bdf" );     // big numbers
            FontLoader bigNumberFontLoader( PB_BIG_NUMBER_FONT    );
            // std::cout << "FontLoader created for big number font" << std::endl;
            rgb_matrix::Font bigNumberFont;
            // std::cout << "Loading font via FontLoader..." << std::endl;
            bigNumberFontLoader.LoadFont( bigNumberFont );
            // std::cout << "FontLoader completed, now loading into _big_number_font..." << std::endl;
            if ( !_big_number_font.LoadFont( PB_BIG_NUMBER_FONT )) {
                std::cerr << "Couldn't load font '" << PB_BIG_NUMBER_FONT << "'" << std::endl;
                throw std::runtime_error("Failed to load big number font");
            }
            // std::cout << "Big number font loaded successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "*** EXCEPTION during big font loading: " << e.what() << std::endl;
            throw;
        }
        // if ( !_big_number_font.LoadFont( BIG_NUMBER_FONT )) {
        //     fprintf( stderr, "Couldn't load font '%s'\n", BIG_NUMBER_FONT ); exit( 1 );
        // }

        FontLoader periodFontLoader( "fonts/mspgothic_030623.bdf" ); // that period
        periodFontLoader.LoadFont( _period_font );
        if ( !_period_font.LoadFont( "fonts/mspgothic_030623.bdf" )) {
            fprintf( stderr, "*** ERROR: Couldn't load font '%s'\n", "fonts/mspgothic_030623.bdf ***" ); exit( 1 );
        }
        else {
            // std::cout << "loaded period font." << std::endl;
        }

        // std::cout << "Loading pairing font: fonts/8x13B.bdf" << std::endl;
        try {
            // Load pairing font
            if ( !_pairing_font.LoadFont( "fonts/8x13B.bdf" )) {
                std::cerr << "*** ERROR: Couldn't load pairing font 'fonts/8x13B.bdf' ***" << std::endl;
                throw std::runtime_error("Failed to load pairing font");
            }
            // std::cout << "Pairing font loaded successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "*** EXCEPTION during pairing font loading: " << e.what() << std::endl;
            throw;
        }
        // end loading fonts

        // std::cout << "*** CREATING DRAWER OBJECTS - CRITICAL SECTION ***" << std::endl;
        try {
            // std::cout << "Creating color objects for drawers..." << std::endl;
            Color color( 255, 255, 0 );
            // std::cout << "color created" << std::endl;
            Color bg_color( 0, 0, 0 );
            // std::cout << "bg_color created" << std::endl;
            Color blue_color( 0, 0, 255 );
            // std::cout << "blue_color created" << std::endl;
            Color red_color( 255, 0, 0 );
            // std::cout << "red_color created" << std::endl;
            Color green_color( 0, 255, 0 );
            // std::cout << "green_color created" << std::endl;
            Color yellow_color( 255, 255, 0 );
            // std::cout << "yellow_color created" << std::endl;
            Color black_color( 0, 0, 0 );
            // std::cout << "black_color created" << std::endl;
            
            // std::cout << "Creating drawer objects..." << std::endl;
            _playerOneScoreDrawer = std::make_unique<Drawer>(
                _canvas.get(), &_big_number_font, Drawer::BIG, player_one_score_color, bg_color );
            _playerTwoScoreDrawer = std::make_unique<Drawer>(
                _canvas.get(), &_big_number_font, Drawer::BIG, player_two_score_color, bg_color );

            _drawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::SMALL, color, bg_color );
            _new_drawer = std::make_unique<Drawer>( _canvas.get(), &_little_number_font, Drawer::BIG, color, bg_color );
            _pipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, color, bg_color );
            _bluePipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, blue_color, bg_color );
            _redPipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, red_color, bg_color );
            _greenPipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, green_color, bg_color );
            _blankPipeDrawer = std::make_unique<Drawer>( _canvas.get(), &_big_number_font, Drawer::BIG, black_color, bg_color );
            _redPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, red_color, bg_color );
            _greenPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, green_color, bg_color );
            _yellowPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, yellow_color, bg_color );
            _blankPeriodDrawer = std::make_unique<Drawer>( _canvas.get(), &_period_font, Drawer::BIG, black_color, bg_color );
            // std::cout << "Creating _text_drawer..." << std::endl;
            // std::cout << "Canvas: " << _canvas.get() << ", Font: " << &_little_number_font << std::endl;
            _text_drawer = std::make_unique<Drawer>( _canvas.get(), &_little_number_font, Drawer::SMALL, yellow_color, bg_color );
            // std::cout << "_text_drawer created successfully" << std::endl;
            
            // std::cout << "Creating _pairing_text_drawer (CRITICAL)..." << std::endl;
            // std::cout << "Canvas: " << _canvas.get() << ", Pairing Font: " << &_pairing_font << std::endl;
            _pairing_text_drawer = std::make_unique<Drawer>( _canvas.get(), &_pairing_font, Drawer::SMALL, yellow_color, bg_color );
            // std::cout << "_pairing_text_drawer created successfully" << std::endl;
            // std::cout << "Creating _setDrawer..." << std::endl;
            _setDrawer = std::make_unique<SetDrawer>( _canvas.get(), _gameState, _clockTimer );
            // std::cout << "_setDrawer created successfully" << std::endl;
            
            // print( "done constructing unique pointers.  updating scoreboard..." );
            // std::cout << "=== SCOREBOARD CONSTRUCTOR END (RASPBERRY PI) ===" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "*** EXCEPTION during drawer creation: " << e.what() << std::endl;
            throw;
        } catch (...) {
            std::cerr << "*** UNKNOWN EXCEPTION during drawer creation ***" << std::endl;
            throw;
        }
    } // fi onRaspberryPi
}

ScoreBoard::~ScoreBoard() {
    // std::cout << "destroying ScoreBoard..." << std::endl;
    if ( _canvas != NULL ) {
        // std::cout << "NOT deleting _canvas..." << std::endl;
        delete _display;
    }
    else { /* std::cout << "*** WARNING: _canvas == NULL, not deleting. ***" << std::endl; */ }
}

void ScoreBoard::setDrawerBackgroundColor( const Color& color ) { _drawer->setBackgroundColor( color ); }
void ScoreBoard::setDrawerForegroundColor( const Color& color ) { _text_drawer->setForegroundColor( color ); }
void ScoreBoard::setPairingDrawerForegroundColor( const Color& color ) { 
    if (_pairing_text_drawer) { 
        _pairing_text_drawer->setForegroundColor( color ); 
    } 
}

void ScoreBoard::drawYellowPeriod() {
    // std::cout << "ScoreBoard::drawYellowPeriod() called... " << std::endl;
    if ( !onRaspberryPi()) {
        print( "inside drawYellowPeriod()" );
        return; 
    }
    int period_lr_offset = 53;
    int period_ud_offset = 129;
    _yellowPeriodDrawer->drawNumber( ".", period_lr_offset, period_ud_offset - 20 );
}

void ScoreBoard::clearScreen() {
    if ( hasCanvas() == false ) {
        std::cout << "\033[2J\033[H";   // Clear screen and move cursor to the top-left 
        // system( "clear" );           // execute a system `clear` command  
    } else {
        if ( !hasCanvas()) { std::cout << "*** ERROR: canvas == NULL.  exiting... ***" << std::endl; exit( 1 ); }
        Color flood_color( 0, 0, 0 ); _canvas->Fill( flood_color.r, flood_color.g, flood_color.b );
    }
}
void ScoreBoard::drawNewText( const std::string& text, int x, int y ) {
    // std::cout << "drawing new text: " << text << " at x: " << x << " and y: " << y << std::endl;
    if (_text_drawer) {
        _text_drawer->drawText(text, x, y);
    }
}

void ScoreBoard::drawPairingText( const std::string& text, int x, int y ) {
    if (_pairing_text_drawer) {
        _pairing_text_drawer->drawText(text, x, y);
    }
}

void ScoreBoard::drawText( const std::string& text, int x, int y ) {}

bool ScoreBoard::onRaspberryPi() {
    std::ifstream file( "/proc/device-tree/model" );
    std::string line;
    if ( file.is_open()) {
        std::getline( file, line );
        file.close();
        if ( line.find( "Raspberry Pi" ) != std::string::npos ) { return true; }
    }
    return false;
}

void ScoreBoard::showSetWinner( Team* scoring_team, Team* opposing_team ) {
    std::cout << "ScoreBoard::showSetWinner() called for team: " << scoring_team->number() << " and team: " << opposing_team->number() << "... " << std::endl;
    std::cout << "Showing set winner." << std::endl;
    if ( onRaspberryPi()) {
        _setDrawer->blinkPickleSets( scoring_team->number());
    }
}

void ScoreBoard::showMatchWinner( Team* scoring_team, Team* opposing_team ) {
    std::cout << "ScoreBoard::showMatchWinner() called for team: " << scoring_team->number() << " and team: " << opposing_team->number() << "... " << std::endl;
    std::cout << "Showing match winner." << std::endl;
    MatchWinSequence matchWinSequence; matchWinSequence.run( _gameState, this );
}

void ScoreBoard::setLittleDrawerFont( const std::string& font_file ) {
    Color color( 255, 255, 0 );
    Color bg_color( 0, 0, 0 );
    print( "loading little number font: " + font_file );
    FontLoader fontLoader( font_file.c_str());
    fontLoader.LoadFont( _little_number_font );
    _text_drawer = std::make_unique<Drawer>( _canvas.get(), &_little_number_font, Drawer::BIG, color, bg_color );
    print( "little number font loaded" );
    // print( "set little number font disabled on october massacre. " );
}

int ScoreBoard::_characterOffset( std::string character ) {
    int char_offset = 0;
    if ( character == "A" ) {
        return -3;
    } else if ( character == "d" ) {
        return 2;
    } else if ( character == "4" ) {
        return 1;
    } else { 
        char_offset = std::stoi( character );
    }

    switch ( char_offset ) {
        case 3: return -1;
        case 4: return -4;
        case 5: return -1;
        default: return 0;
    }
}

int ScoreBoard::_firstCharacterOffset( std::string character ) {
    int char_offset = 0;
    #define FIRST_ZERO_OFFSET -2
    if ( character == "A" ) {
        return -3;
    } else if ( character == "d" ) {
        return 2;
    } else if ( character == "0" ) {
        return FIRST_ZERO_OFFSET;
    } else if ( character == "3" ) {
        return -3;
    } else if ( character == "1" ) {
        return 1;
    } else { char_offset = std::stoi( character ); }

    switch ( char_offset ) {
        case 3: return -1;
        case 4: return -4;
        case 5: return -1;
        default: return 0;
    }
}

std::string ScoreBoard::_translate( int raw_score ) {
    switch ( raw_score ) {
        case 0:               return "0";
        case 1:               return "1";
        case 2:               return "2";
        case 3:               return "3";
        case 4:               return "4";
        case 5:               return "5";
        case 6:               return "6";
        case 7:               return "7";
        case 8:               return "8";
        case 9:               return "9";
        case 10:              return "10";
        case 11:              return "11";
        case 12:              return "12";
        case 13:              return "13";
        case 14:              return "14";
        case 15:              return "15";
        case 16:              return "16";
        case 17:              return "17";
        case 18:              return "18";
        case 19:              return "19";
        case 20:              return "20";
        case 21:              return "21";
        default:              return "*** ERROR: can not translate. ***";
    }
}

std::string ScoreBoard::drawPlayerScore( Team* player ) {
    std::string serve_bar_text = hasCanvas() == true ? "I" : "\033[34m|";    // filled in "I" 
    // std::string serve_bar_text = hasCanvas() == true ? "i" : "\033[34m|"; // hollow "i"
    // std::string serve_bar = player->getServe() == player->number() ? serve_bar_text : " ";
    std::string serve_bar = serve_bar_text;
    // std::string other_serve_bar = _gameState->getServe() == player->getOpposingTeam()->number() ? serve_bar_text : " ";
    std::string score = _translate( player->getPoints());
    if ( hasCanvas() == false ) {
        std::cout << "==========================" << std::endl;
        player->number() == 1 ?  // type player 1 score, else type player 2 score
            std::cout << "| \033[92mPLAYER 1: //// " << serve_bar << "\033[92m " << score << " //// " << std::endl :
            // std::cout << "| \033[31mPLAYER 2: //// " << serve_bar << "\033[31m " << score << " //// \033[35m" << std::endl;
            std::cout << "| \033[31mPLAYER 2: //// " << serve_bar << "\033[31m " << score << " //// \033[0m" << std::endl;

    } else {
        #define BIG_NUMBER_VERTICAL_OFFSET      2
        #define BIG_NUMBER_HORIZONTAL_OFFSET    1
        #define BIG_NUMBER_HORIZ_SERVE_2_OFFSET 58
        #define GREEN_YELLOW_SERVE_OFFSET       34
        #define RED_YELLOW_SERVE_OFFSET         34
        int vertical_offset   = BIG_NUMBER_VERTICAL_OFFSET;

        // Draw team member number on left if player is serving (serve > 0)
        if (player->getServe() > 0) {
            int team_member_number;
            
            // Check if we're in singles mode (fresh serves = 1) or doubles mode (fresh serves = 2)
            if (_rules->getFreshServes() == 1) {
                // Singles mode: always show "1" for the serving player
                team_member_number = 1;
            } else {
                // Doubles mode: convert remaining serves to team member number (2 serves left = member 1, 1 serve left = member 2)
                team_member_number = (player->getServe() == 2) ? 1 : 2;
            }
            
            // Use the same offset as the old left serve bar
            int serve_x = BIG_NUMBER_HORIZONTAL_OFFSET;
            // Align vertically with the big number score
            int baseline = _big_number_font.baseline();
            int serve_y;
            if (_setDrawer) {
                int set_font_height = _setDrawer->getSetFontHeight();
                int big_font_height = _big_number_font.height();

                if (player->number() == PLAYER_1_INITIALIZED) { // Green Team
                    // Player 1 ( Green Team ): center on big number, shift up by 20 pixels total
                    serve_y = baseline + vertical_offset + (big_font_height / 2) - (set_font_height / 2) - GREEN_YELLOW_SERVE_OFFSET;
                } else {    // Red Team
                    // Player 2 ( Red Team ): center on big number, one big number height down, shift up by 20 pixels total
                    serve_y = baseline + vertical_offset + big_font_height + (big_font_height / 2) - (set_font_height / 2) - RED_YELLOW_SERVE_OFFSET;
                }
            } else {
                std::cout << "*** WARNING: something is wrong.  There is no _seDrawer! *** " << std::endl;
                // fallback to previous logic if _setDrawer is not available
                if (player->number() == PLAYER_1_INITIALIZED) {
                    serve_y = baseline + GREEN_YELLOW_SERVE_OFFSET;  // subtract 2 to move up 2
                } else {
                    serve_y = ( baseline - RED_YELLOW_SERVE_OFFSET ) + _big_number_font.height();
                }
            }
            // Draw the team member number using the little number font, yellow, small (same as set scores)
            if ( _setDrawer ) {
                Color yellow_color(255, 255, 0);
                // std::cout << "calling setDrawer->drawTextOnCanvas() " << "team_member_number: " << team_member_number << std::endl;
                if ( team_member_number == 1 ) {
                    _setDrawer->drawTextOnCanvas( serve_x + 1, serve_y, yellow_color, std::to_string( team_member_number ));
                } else {
                    _setDrawer->drawTextOnCanvas( serve_x, serve_y, yellow_color, std::to_string( team_member_number ));
                }
            }
        }
        // (No serve indicator if serves == 0)

        // Adjust vertical offset based on player number
        if ( player->number() == PLAYER_1_INITIALIZED ) {
            vertical_offset = BIG_NUMBER_VERTICAL_OFFSET;
        } else {
            vertical_offset = _big_number_font.height() + BIG_NUMBER_VERTICAL_OFFSET;
        }

        int baseline = _big_number_font.baseline();                  // set the coordinates for the text
        int first_offset = _firstCharacterOffset(score.substr( 0, 1 ));
        int second_offset = ( score.length() > 1 ) ? _characterOffset(score.substr(1, 1)) : 0;

        // std::cout << "vertical offset: " << vertical_offset << std::endl;
        // std::cout << "score: " << score << std::endl;
        if ( score == "0" ) {
            first_offset += 8; // move 8 clicks to the right for "0"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }

        if ( score == "1" ) {
            first_offset += 7; // move 8 clicks to the right for "1"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "2" ) {
            first_offset += 6; // move 5 clicks to the right for "2"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "3" ) {
            first_offset += 9; // move 5 clicks to the right for "3"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "4" ) {
            first_offset += 9; // move 5 clicks to the right for "4"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "5" ) {
            first_offset += 7; // move 5 clicks to the right for "5"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "6" ) {
            first_offset += 5; // move 5 clicks to the right for "6"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "7" ) {
            first_offset += 6; // move 8 clicks to the right for "7"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "8" ) {
            first_offset += 6; // move 5 clicks to the right for "8"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "9" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // move 5 clicks to the right for "9"
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "10" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "10" Good - no change
            second_offset += 3; // "10" Good - no change
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "11" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // Left "1" - Move 3 clicks left (was 8, now 5)
            second_offset += 4; // Right "1" - Move 2 clicks left (was 6, now 4)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "12" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks to right (5 + 5 = 10)
            second_offset += 3;  // "2" - Move 5 clicks to right (3 + 5 = 8)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "13" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks to right (5 + 5 = 10)
            second_offset += 3;  // "3" - Move 3 clicks to right (3 + 3 = 6)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "14" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks to right (5 + 5 = 10)
            second_offset += 0;  // "4" - Good (no change from base)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "15" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks right (5 + 5 = 10)
            second_offset += 4;  // "5" - Move 4 clicks right (3 + 4 = 7)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "16" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks right (5 + 5 = 10)
            second_offset += 2;  // "6" - Move 2 clicks right (3 + 2 = 5)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "17" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks right (5 + 5 = 10)
            second_offset += 4;  // "7" - Move 4 clicks right (3 + 4 = 7)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "18" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks to right (5 + 5 = 10)
            second_offset += 3;  // "8" - Move 3 clicks to right (3 + 3 = 6)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "19" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 5; // "1" - Move 5 clicks to right (5 + 5 = 10)
            second_offset += 2;  // "9" - Move 2 clicks to right (3 + 2 = 5)
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }
        
        if ( score == "20" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 3; // change 5 to 13, moving 8 clicks to the right 070525
            second_offset += 4;
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }

        if ( score == "21" ) {
            // std::cout << "score is zero, adding 8 click offset... " << std::endl;
            first_offset += 3; // change 5 to 13, moving 8 clicks to the right 070525
            second_offset += 7;
        } else {
            // std::cout << "score is not zero, NOT adding 8 click offset... " << std::endl;
        }

        // std::cout << "first offset: " << first_offset << std::endl;

        // Draw the player score as before
        if (player->number() == PLAYER_1_INITIALIZED ) { // then draw text depending on player
            vertical_offset = BIG_NUMBER_VERTICAL_OFFSET;
            _playerOneScoreDrawer->drawNumber( score.substr( 0, 1 ), first_offset + 16, baseline + vertical_offset );
            if ( score.length() > 1 ) {
                _playerOneScoreDrawer->drawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset );
            }
        } else {
            vertical_offset = _big_number_font.height() + BIG_NUMBER_VERTICAL_OFFSET;
            _playerTwoScoreDrawer->drawNumber( score.substr( 0, 1 ), first_offset + 16, baseline + vertical_offset );
            if ( score.length() > 1 ) {
                _playerTwoScoreDrawer->drawNumber( score.substr( 1, 1 ), second_offset + 38, baseline + vertical_offset );
            }
        }
    }
    // created a concatenated string with "PLAYER 1: ////// " + serve_bar
    std::string returnString = "*** WARNING: return string is not set. this is not normal ***";
    std::string player1ScoreString = "PLAYER 1: ////// " + serve_bar + " " + score + " //////";
    std::string player2ScoreString = "PLAYER 2: ////// " + serve_bar + " " + score + " //////";
    player->number() == PLAYER_1_INITIALIZED ?
        returnString = player1ScoreString : returnString = player2ScoreString;
    // std::cout << "returning return string: " << returnString << std::endl;
    return returnString;
}

bool ScoreBoard::hasCanvas() {
    if ( _canvas != NULL ) { return true;
    } else { /* std::cout << "*** WARNING: canvas is NULL ***" << std::endl; */ return false; }}

void ScoreBoard::update() {
    clearScreen();
    drawPlayerScore(_team_a); // green player (player 1) always on top
    drawPlayerScore(_team_b); // red player (player 2) always on bottom
    if ( hasCanvas() == false ) { std::cout << "==========================" << std::endl; } // only for terminal
    _setDrawer->drawSets();                                   // if the clock timer is not running, start it...
    if ( !_clockTimer->isRunning()) { _clockTimer->start(); }
}

void ScoreBoard::drawSets() { _setDrawer->drawSets(); }

void ScoreBoard::setTeamAPoints( int points ) {}

void ScoreBoard::setTeamBPoints( int points ) {}

void ScoreBoard::setTeamASets( int sets ) {}

void ScoreBoard::setTeamBSets( int sets ) {}

void ScoreBoard::setTeamAServe( int serve ) {}

void ScoreBoard::setTeamBServe( int serve ) {}

void ScoreBoard::toggleServe( Team* scoring_team, Team* opposing_team ) {
    std::cout << "ScoreBoard::toggleServe() called for team: " << scoring_team->number() << " and team: " << opposing_team->number() << "... " << std::endl;
}

void ScoreBoard::setTeamAPointsOn() {}

void ScoreBoard::setTeamBPointsOn() {}

void ScoreBoard::setClockTimer(ClockTimer* clockTimer) {
    _clockTimer = clockTimer;
    std::cout << "ClockTimer set in ScoreBoard: " << clockTimer << std::endl;
    
    if (_setDrawer != nullptr) {
        _setDrawer->setClockTimer(clockTimer);
        std::cout << "ClockTimer set in SetDrawer" << std::endl;
    }
}

ClockTimer* ScoreBoard::getClockTimer() const { return _clockTimer; }
