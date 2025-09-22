#ifndef GAME_INPUT_H
#define GAME_INPUT_H

class IGameInput {
public:
    virtual ~IGameInput() = default;
    virtual int getInput() = 0;       // gets for example "2", GREEN_REMOTE_GREEN_SCORE
};

#endif