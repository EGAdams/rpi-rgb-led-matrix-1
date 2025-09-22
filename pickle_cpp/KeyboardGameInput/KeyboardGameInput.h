#ifndef KEYBOARD_GAME_INPUT_H
#define KEYBOARD_GAME_INPUT_H
#include "../IGameInput/IGameInput.h"
#include <iostream>

class KeyboardGameInput : public IGameInput {
public:
    KeyboardGameInput();
    ~KeyboardGameInput();
    int getInput() override;
};

#endif // KEYBOARD_GAME_INPUT_H