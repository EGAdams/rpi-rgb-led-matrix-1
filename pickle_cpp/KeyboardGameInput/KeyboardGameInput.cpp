#include "KeyboardGameInput.h"

KeyboardGameInput::~KeyboardGameInput() {
    std::cout << "destroying keyboard game input..." << std::endl;
}
KeyboardGameInput::KeyboardGameInput() {
    std::cout << "constructing keyboard game input..." << std::endl;
}

int KeyboardGameInput::getInput() {
    int input_read_from_keyboard = 0;
    std::cin >> input_read_from_keyboard;
    return input_read_from_keyboard;
}