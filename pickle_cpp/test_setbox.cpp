/*  test_setbox.cpp  – clean vertical lines
 *
 *  Modified to use actual SetDrawer object for easier testing.
 */
#include <iostream>
#include <memory>
#include <fstream>
#include <unistd.h>
#include <climits>
#include "../include/led-matrix.h"
#include "../include/graphics.h"
#include "SetDrawer/SetDrawer.h"
#include "GameState/GameState.h"

using namespace rgb_matrix;  // This makes RGBMatrix and Color types available

/* ---------------- Test setup using actual SetDrawer and GameState ---------------- */

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
    
    // Create the actual SetDrawer object
    SetDrawer drawer(matrix.get(), &gameState);
    
    // Use the real SetDrawer method
    drawer.drawSetBox();
    drawer.drawCenterDivider();

    std::cout << "Press <Enter> when done…";
    std::cin.get();

    matrix->Clear();
    return 0;
}
