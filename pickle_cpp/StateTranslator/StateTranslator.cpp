#include "StateTranslator.h"

std::string StateTranslator::translateState(int stateValue) const {
    switch (stateValue) {
        case REGULAR_PLAY_NO_SCORE_STATE:
            return "REGULAR_PLAY_NO_SCORE_STATE";
        case NO_SCORE_SLEEP_STATE:
            return "NO_SCORE_SLEEP_STATE";
        case AFTER_SLEEP_MODE:
            return "AFTER_SLEEP_MODE";
        case REGULAR_PLAY_SLEEP_STATE:
            return "REGULAR_PLAY_SLEEP_STATE";
        case REGULAR_PLAY_AFTER_SCORE_STATE:
            return "REGULAR_PLAY_AFTER_SCORE_STATE";
        case PAIRING_SLEEP_MODE_STATE:
            return "PAIRING_SLEEP_MODE_STATE";
        case PAIRING_MODE_STATE:
            return "PAIRING_MODE_STATE";
        case SLEEP_MODE:
            return "SLEEP_MODE";
        case MATCH_WIN_BLINK_STATE:
            return "MATCH_WIN_BLINK_STATE";
        case AFTER_MATCH_WIN_STATE:
            return "AFTER_MATCH_WIN_STATE";
        case UNDO_STATE:
            return "UNDO_STATE";
        default:
            return "UNKNOWN_STATE(" + std::to_string(stateValue) + ")";
    }
}