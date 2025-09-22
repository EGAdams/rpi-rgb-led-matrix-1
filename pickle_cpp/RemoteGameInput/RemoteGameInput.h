#ifndef REMOTE_GAME_INPUT_H
#define REMOTE_GAME_INPUT_H
#include "../IGameInput/IGameInput.h"
#include "../RemoteInputWithTimer/IInputs.h"
#include "../INPUTS/INPUTS.h"
#include <iostream>

class RemoteGameInput : public IGameInput {
public:
    RemoteGameInput( IInputs* inputs );
    ~RemoteGameInput();
    int getInput() override;
private:
    IInputs* _inputs;
};

#endif
