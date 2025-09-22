#ifndef TEST_GAME_INPUT_H
#define TEST_GAME_INPUT_H
#include "../IGameInput/IGameInput.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class TestGameInput : public IGameInput {
public:
    TestGameInput();
    ~TestGameInput();
    int getInput() override;
private:
    std::vector<int> _test_inputs;
    size_t _current_index = 0;
    std::unordered_map<std::string, int> _input_map;
    void _loadTestInputs(const std::string& filename);
    bool _onRaspberryPi();
};



#endif // TEST_GAME_INPUT_H