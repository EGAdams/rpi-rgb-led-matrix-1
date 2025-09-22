#ifndef SUBJECT_H
#define SUBJECT_H
#include "../Observer/Observer.h"
#include <vector>
#include <list>
#include <iostream>

class Subject {
public:
    virtual ~Subject() = 0; // Making the destructor pure virtual
    
    // Copy constructor - creates a new Subject with empty observers list
    Subject(const Subject& other) {
        std::cout << "*** DEBUG: Subject copy constructor ENTRY ***" << std::endl;
        // Don't copy observers - new object starts with empty observers
        std::cout << "*** DEBUG: Subject copy constructor COMPLETE ***" << std::endl;
    }
    
    // Move constructor - creates a new Subject with moved observers list
    Subject(Subject&& other) noexcept {
        std::cout << "*** DEBUG: Subject move constructor ENTRY ***" << std::endl;
        observers = std::move(other.observers);
        std::cout << "*** DEBUG: Subject move constructor COMPLETE ***" << std::endl;
    }
    
    // Assignment operator - don't copy observers
    Subject& operator=(const Subject& other) {
        std::cout << "*** DEBUG: Subject assignment operator ENTRY ***" << std::endl;
        if (this != &other) {
            std::cout << "*** DEBUG: Subject assignment operator - not self assignment ***" << std::endl;
            // Don't copy observers - keep current observers
        }
        std::cout << "*** DEBUG: Subject assignment operator COMPLETE ***" << std::endl;
        return *this;
    }
    
    // Move assignment operator - move observers list
    Subject& operator=(Subject&& other) noexcept {
        std::cout << "*** DEBUG: Subject move assignment operator ENTRY ***" << std::endl;
        if (this != &other) {
            observers = std::move(other.observers);
        }
        std::cout << "*** DEBUG: Subject move assignment operator COMPLETE ***" << std::endl;
        return *this;
    }
    
    // Default constructor
    Subject() = default;
    
    void attach( Observer* observer ) { observers.push_back( observer );}
    void detach( Observer* observer ) { observers.remove( observer );}

    void notify() { // Notify all observers about a state change.
        // Create a copy of the observers list to avoid crashing.  github copilot
        // says that it may be because the observer is detaching itself from the
        // list while the list is being iterated.  I'm not sure if that's the 
        // case, but making a copy of the list here, fixes the crash.
        std::list< Observer* > observersCopy = observers; /*** copy observers 1st! ***/
        for ( Observer* observer : observersCopy ) {
            if ( !observer ) { continue; }
            observer->update( this ); }}
private:
    std::list< Observer* > observers; // List of observers
};
inline Subject::~Subject() {
    std::cout << "*** DEBUG: Subject destructor ENTRY, this=" << this << ", observers.size()=" << observers.size() << std::endl;
    try {
        std::cout << "*** DEBUG: Subject destructor - about to clear observers list ***" << std::endl;
        observers.clear(); // Explicitly clear the observers list
        std::cout << "*** DEBUG: Subject destructor - observers list cleared ***" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "*** CRITICAL ERROR: Exception in Subject destructor: " << e.what() << " ***" << std::endl;
    } catch (...) {
        std::cout << "*** CRITICAL ERROR: Unknown exception in Subject destructor ***" << std::endl;
    }
    std::cout << "*** DEBUG: Subject destructor COMPLETE, this=" << this << std::endl;
} // Definition outside the class to avoid linker errors
#endif // SUBJECT_H
