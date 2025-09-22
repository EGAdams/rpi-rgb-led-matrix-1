#ifndef REMOTE_PAIRING_SCREEN_H
#define REMOTE_PAIRING_SCREEN_H

#include <string>
#include "../ScoreBoard/ScoreBoard.h"
#include "../../include/graphics.h"

class Team; // Forward declaration


class RemotePairingScreen {
public:
    RemotePairingScreen( ScoreBoard* scoreboard, Team* team_a, Team* team_b );
    ~RemotePairingScreen();
    void draw();                // Method to display the instructions on the screen
    void greenPlayerPressed();  // Called when the Green Player presses their button
    void redPlayerPressed();    // Called when the Red Player presses their button
    bool inPairingMode() const; // Check if still in pairing mode
    void enablePairingMode();  // Enable pairing mode

private:
    bool        _green_player_paired;
    bool        _red_player_paired;
    ScoreBoard* _scoreboard;

    void clearGreenPlayerText(); // Helper to clear the screen or specific text area
    void clearRedPlayerText();
    void clearAllText();
private:
    Team* _team_a;
    Team* _team_b;
};
#endif // REMOTE_PAIRING_SCREEN_H
