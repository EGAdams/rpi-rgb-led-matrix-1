#ifndef command_h 
#define command_h
/*
 * Command Interface
 */
class Command {
public:
	virtual void execute() = 0;
    virtual void undo()    = 0;
};

#endif /* command_h */
