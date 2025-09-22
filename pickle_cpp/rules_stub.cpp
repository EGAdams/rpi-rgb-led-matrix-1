// Minimal Rules stub for testing
#include "Rules/Rules.h"

int Rules::getFreshServes() { 
    return _freshServes;  // Default value for testing
}

// Define any other essential Rules methods if needed
Rules::Rules(int freshServes) : _freshServes(freshServes) {}
Rules::~Rules() {}