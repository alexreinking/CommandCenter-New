#ifndef COMMANDCENTERBASE_H
#define COMMANDCENTERBASE_H

#include "eventloop.h"
#include "actor.h"

class Event;

class CommandCenterBase : public Actor
{
public:
    CommandCenterBase() {}
    virtual ~CommandCenterBase() {}
    virtual void handleEvent(shared_ptr<Event> event);

protected:
    void die();
};

#endif // COMMANDCENTERBASE_H
