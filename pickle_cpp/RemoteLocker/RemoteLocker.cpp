#include "RemoteLocker.h"

RemoteLocker::RemoteLocker( Team* team_a ) { _team_a = team_a; }
RemoteLocker::~RemoteLocker(){}

bool RemoteLocker::playerNotServing( int button ) {
    bool green_serve = _team_a->getServe() == 0 ? false : true;

    switch ( button ) {
    case RED_REMOTE_RED_SCORE:   
    case RED_REMOTE_GREEN_SCORE: 
    // case RED_REMOTE_UNDO:
    // case RED_REMOTE_RESET:
        return green_serve ? true : false;
        break;

    case GREEN_REMOTE_GREEN_SCORE:
    case GREEN_REMOTE_RED_SCORE:
    // case GREEN_REMOTE_UNDO:
    // case GREEN_REMOTE_RESET:
        return green_serve ? false : true;
        break;
    
    case RED_REMOTE_UNDO:
    case GREEN_REMOTE_UNDO:
        return false;  // Never lock out UNDO commands
        break;
    
    default:
        return false;
    }
}    
