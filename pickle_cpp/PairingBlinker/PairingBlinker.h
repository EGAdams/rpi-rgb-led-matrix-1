#ifndef PAIRING_BLINKER_H
#define PAIRING_BLINKER_H

#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "../ScoreBoard/ScoreBoard.h"
#include "../Blinker/Blinker.h"
#include "../TennisConstants/TennisConstants.h"
#include "../ScoreboardBlinker/ScoreboardBlinker.h"
#include "../IInputWithTimer/IInputWithTimer.h"
#include <thread>

class PairingBlinker : public Blinker {
public:
    PairingBlinker(ScoreBoard* scoreBoard);
    ~PairingBlinker();
    
    void start() override;
    void stop() override;
    void setGreenPlayerPaired(bool paired);  // New method to track Green player status
    void setRedPlayerPaired(bool paired);    // New method to track Red player status
    void enable();
    void sleepModeOn();
    void sleepModeOff();
    bool awake();
    bool areBothPlayersPaired();

private:
    void blinkLoop();         // Alternates between Green and Red instructions
    void showGreenInstructions();
    void showPlayerPressYourRemoteText();
    void showRedInstructions();
    
    ScoreBoard* _scoreboard;
    bool        _should_stop;
    std::thread blink_thread;

    // New state tracking
    bool _green_player_paired;
    bool _red_player_paired;
    bool _show_green = true;
    bool _sleep_mode = false;
    std::string _original_font;
};

#endif
