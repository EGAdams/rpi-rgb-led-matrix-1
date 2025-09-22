#include <iostream>
#include "GAME_MODES/GAME_MODES.h"
#include "GameObject/GameObject.h"
#include "../tennis-game/TestInputWithTimer/TestInputWithTimer.h"
#include "../tennis-game/ColorManager/ColorManager.h"
#include "../IDisplay/IDisplay.h"
#include "../TestDisplay/TestDisplay.h"

int main() {
    // Create core game objects
    IDisplay*       display         = static_cast< IDisplay* >( new TestDisplay() );
    
    // Create GameObject instance
    GameObject gameObject;

    // Create TestInputWithTimer instance with a timeout of 5000 ms (5 seconds)
    TestInputWithTimer testInput( 5000 );

    std::cout << "Press SPACE (then Enter) to get next score input and run game loop. Press 'q' (then Enter) to quit." << std::endl;

    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "q" || input == "Q") {
            std::cout << "Quitting..." << std::endl;
            break;
        }
        if (input == " ") {
            int score = testInput.getInput();
            std::cout << "Received score input: " << score << std::endl;
            gameObject.loopGame();
        }
    }

    return 0;
}