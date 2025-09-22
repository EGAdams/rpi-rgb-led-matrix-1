#include "TranslateConstant.h"
#include <unordered_map>
#include <string>

TranslateConstant::TranslateConstant() :
  _map_constant_to_string({
    { NO_REMOTE_BUTTONS,         "NO_REMOTE_BUTTONS" },
    { RED_REMOTE_RED_SCORE,      "RED_REMOTE_RED_SCORE" },
    { RED_REMOTE_GREEN_SCORE,    "RED_REMOTE_GREEN_SCORE" },
    { RED_REMOTE_UNDO,           "RED_REMOTE_UNDO" },
    { RED_REMOTE_RESET,          "RED_REMOTE_RESET" },
    { GREEN_REMOTE_GREEN_SCORE,  "GREEN_REMOTE_GREEN_SCORE" },
    { GREEN_REMOTE_RED_SCORE,    "GREEN_REMOTE_RED_SCORE" },
    { GREEN_REMOTE_UNDO,         "GREEN_REMOTE_UNDO" },
    { GREEN_REMOTE_RESET,        "GREEN_REMOTE_RESET" },
    { UNKNOWN_REMOTE_BUTTON,     "UNKNOWN_REMOTE_BUTTON" },
    { UMPIRE_REMOTE_GREEN_SCORE, "UMPIRE_REMOTE_GREEN_SCORE" },
    { UMPIRE_REMOTE_RED_SCORE,   "UMPIRE_REMOTE_RED_SCORE" },
    { UMPIRE_REMOTE_UNDO,        "UMPIRE_REMOTE_UNDO" }
  })
{
}

TranslateConstant::~TranslateConstant() {}

std::string TranslateConstant::get_translated_constant(int the_constant) {
  auto it = _map_constant_to_string.find(the_constant);
  if (it != _map_constant_to_string.end()) {
    return it->second;
  } else {
    return "UNKNOWN_BUTTON(" + std::to_string(the_constant) + ")";
  }
}
