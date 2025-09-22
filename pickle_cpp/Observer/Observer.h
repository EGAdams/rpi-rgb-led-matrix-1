// Observer.h
#ifndef OBSERVER_H
#define OBSERVER_H

class Subject; // Forward declaration

class Observer {
public:
    virtual ~Observer() {}
    virtual void update( Subject* subject ) = 0; // This method is called when the subject's state changes
};

#endif // OBSERVER_H

