/*
 * class SetHistoryText
 */
#include "../DIGI_V6_15/DIGI_V6_15.h"
#include "SetHistoryText.h"

SetHistoryText::SetHistoryText( GameState* gameState ) : _gameState( gameState ) {}
SetHistoryText::~SetHistoryText() {}

std::string SetHistoryText::getSetHistoryText( int player ) {
    std::string setHistoryText = _buildString( player );
    return setHistoryText;
}

std::string SetHistoryText::_buildString( int player ) { // std::map<int, int> >>---> string
    if ( _gameState == nullptr ) {  return ""; }

    // set history comes from game state as a std::map<int, int> AKA array of sets
    std::map<int, int> setHistory = player == PLAYER_ONE_SET_INDEX ?
        _gameState->getPlayer1SetHistory() : _gameState->getPlayer2SetHistory();

    std::string setHistoryText = "";
    int currentSet = _gameState->getCurrentSet();

    for ( int i = 1; i <= currentSet; i++ ) { // draw all sets up to current set
        if ( setHistory.find( i ) == setHistory.end() ) {
            print( "*** Warning: Set history does not contain key: " << i << " ***" ); }
        else { setHistoryText += std::to_string( setHistory[ i ]);}
        setHistoryText += " ";
    }
    return setHistoryText;
}
