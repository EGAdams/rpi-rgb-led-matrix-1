#ifndef BLINKER_H
#define BLINKER_H

class Blinker {
public:
    virtual ~Blinker() = default;
    
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif
