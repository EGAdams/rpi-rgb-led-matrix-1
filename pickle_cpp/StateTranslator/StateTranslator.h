#ifndef STATETRANSLATOR_H
#define STATETRANSLATOR_H

#include <string>
#include "../DIGI_V6_15/DIGI_V6_15.h"

class StateTranslator {
public:
    StateTranslator() = default;
    ~StateTranslator() = default;
    
    std::string translateState(int stateValue) const;
};

#endif // STATETRANSLATOR_H