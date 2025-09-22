/*  test_setbox_simple.cpp  – simple test without complex dependencies
 *
 *  Modified to use actual SetDrawer with minimal dependencies.
 */
#include <iostream>
#include <memory>
#include <fstream>
#include <unistd.h>
#include <climits>
#include "../include/led-matrix.h"
#include "../include/graphics.h"

using namespace rgb_matrix;  // This makes RGBMatrix and Color types available

/* ---------------- Minimal GameState for SetDrawer testing ---------------- */
class GameState {
public:
    GameState() : _team_a_sets(2), _team_b_sets(1) {}
    void setTeamASets(int sets) { _team_a_sets = sets; }
    void setTeamBSets(int sets) { _team_b_sets = sets; }
    int getTeamASets() { return _team_a_sets; }
    int getTeamBSets() { return _team_b_sets; }
private:
    int _team_a_sets;
    int _team_b_sets;
};

/* ---------------- Simplified SetDrawer for testing ---------------- */
class SimpleSetDrawer {
public:
    SimpleSetDrawer(RGBMatrix *canvas, GameState *gameState)
        : _canvas(canvas), _gameState(gameState),
          _lastBoxX(0), _lastBoxY(0), _lastBoxWidth(0), _lastBoxHeight(0) {

        if (!_little_font.LoadFont("fonts/the_sets_font.bdf") &&
            !_little_font.LoadFont("../fonts/the_sets_font.bdf")) {
            std::cerr << "Unable to load the_sets_font.bdf" << std::endl;
            exit(1);
        }
    }

    /* public entry point ------------------------------------------------ */
    void drawSetBox() {
        _calculateBoxDimensions();

        /*  1.  Draw the borders first     */
        _drawBoxBorders();

        /*  2.  Draw the center divider.  */
        _drawCenterDivider();
        
        /*  3.  Draw the text numbers.    */
        _drawSetNumbers();
    }

private:
    /* -------------- constants / fields -------------------------------- */
    static constexpr int START_ROW = 89;        // same as before

    RGBMatrix *_canvas;
    GameState *_gameState;
    Font       _little_font;

    int _lastBoxX, _lastBoxY, _lastBoxWidth, _lastBoxHeight;

    /* -------------- helpers ------------------------------------------- */
    void _calculateBoxDimensions() {
        constexpr int BOX_PADDING = 2;      // space inside border
        constexpr int BOX_WIDTH   = 32;     // fixed width for the two-set box
        constexpr int DEBUG_OFFSET = 10;    // your existing "-10 for debugging"
        constexpr int SHIFT_UP     = 20;     // <-- raise the whole thing 5 px

        _lastBoxWidth  = BOX_WIDTH;
        _lastBoxHeight = _little_font.height() + (BOX_PADDING * 2);

        // centre horizontally
        const int canvas_w = _canvas->width();
        _lastBoxX = (canvas_w - _lastBoxWidth) / 2;

        // vertical position (clamp at top if we would go negative)
        _lastBoxY = START_ROW - BOX_PADDING - DEBUG_OFFSET - SHIFT_UP;
        if (_lastBoxY < 0) _lastBoxY = 0;
    }

    void _drawBoxBorders() {
        const Color BLUE(0, 0, 255);
        const int right_x  = _lastBoxX + _lastBoxWidth  - 1;
        const int bottom_y = _lastBoxY + _lastBoxHeight - 1;

        /* top & bottom */
        for (int x = _lastBoxX; x <= right_x; ++x) {
            _canvas->SetPixel(x, _lastBoxY, BLUE.r, BLUE.g, BLUE.b);
            _canvas->SetPixel(x, bottom_y , BLUE.r, BLUE.g, BLUE.b);
        }
        /* left & right */
        for (int y = _lastBoxY; y <= bottom_y; ++y) {
            _canvas->SetPixel(_lastBoxX, y, BLUE.r, BLUE.g, BLUE.b);
            _canvas->SetPixel(right_x  , y, BLUE.r, BLUE.g, BLUE.b);
        }
    }

    void _drawCenterDivider() {
        const Color BLUE(0, 0, 255);
        const int mid_x     = _lastBoxX + (_lastBoxWidth / 2) - 1;
        const int bottom_y  = _lastBoxY + _lastBoxHeight - 1;

        for (int y = _lastBoxY; y <= bottom_y; ++y) {
            _canvas->SetPixel(mid_x    , y, BLUE.r, BLUE.g, BLUE.b);
            _canvas->SetPixel(mid_x + 1, y, BLUE.r, BLUE.g, BLUE.b);
        }
    }

    void _drawSetNumbers() {
        const std::string text_left  = std::to_string(_gameState->getTeamASets());
        const std::string text_right = std::to_string(_gameState->getTeamBSets());

        const Color GREEN(  0, 255,   0);
        const Color RED  (255,   0,   0);

        const int half_w  = _lastBoxWidth / 2;
        const int pad_x   = 1;                       // 1-pixel breathing room
        const int base_y  = _lastBoxY + _little_font.baseline() + 2;

        /* left side */
        int left_x  = _lastBoxX + pad_x +
                      (half_w - (pad_x * 2) - _getStringWidth(text_left)) / 2;
        /* right side */
        int right_x = _lastBoxX + half_w + pad_x +
                      (half_w - (pad_x * 2) - _getStringWidth(text_right)) / 2;

        DrawText(_canvas, _little_font, left_x , base_y,
                             GREEN, nullptr, text_left .c_str());
        DrawText(_canvas, _little_font, right_x, base_y,
                             RED  , nullptr, text_right.c_str());
    }

    int _getStringWidth(const std::string &s) const {
        int w = 0;
        for (char c : s) {
            int cw = _little_font.CharacterWidth(static_cast<uint32_t>(c));
            if (cw > 0) w += cw;
        }
        return w;
    }
};

/* ------------------------- main ------------------------------------- */
int main() {
    RGBMatrix::Options opt;
    opt.hardware_mapping       = "regular";
    opt.pixel_mapper_config    = "Rotate:180;264-Mapper";
    opt.rows                   = 64;
    opt.cols                   = 64;
    opt.chain_length           = 1;
    opt.parallel               = 2;
    opt.disable_hardware_pulsing = true;
    opt.brightness             = 35;               // or read from file, as before
    opt.pwm_bits               = 11;
    opt.multiplexing           = 1;

    RuntimeOptions rt;
    rt.gpio_slowdown = 2;

    std::unique_ptr<RGBMatrix> matrix(
        RGBMatrix::CreateFromOptions(opt, rt));
    if (!matrix) {
        std::cerr << "Failed to open matrix." << std::endl;
        return 1;
    }
    matrix->Clear();

    // Create a real GameState object and set some test values
    GameState gameState;
    gameState.setTeamASets(2);  // Set team A to 2 sets
    gameState.setTeamBSets(1);  // Set team B to 1 set
    
    // Create the simplified SetDrawer object (this works like the original but without complex dependencies)
    SimpleSetDrawer drawer(matrix.get(), &gameState);
    
    // Draw the set box
    drawer.drawSetBox();

    std::cout << "Press <Enter> when done…";
    std::cin.get();

    matrix->Clear();
    return 0;
}