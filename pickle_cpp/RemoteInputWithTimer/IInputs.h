#ifndef IINPUTS_H
#define IINPUTS_H

class IInputs {
public:
    virtual ~IInputs() = default;
    virtual int read_mcp23017_value() = 0;
};

#endif // IINPUTS_H