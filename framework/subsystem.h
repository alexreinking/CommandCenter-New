#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>
#include "eventloop.h"
#include "commandcenterbase.h"

class Subsystem : public Actor
{
public:
    Subsystem(Actor *parent, std::string name):
        Actor(name, parent->getEventLoop()),
        parent(parent) {}
    virtual ~Subsystem() {}

    bool isRunning() const { return running; }
    int getExitCode() const { return exitCode; }

    void stop(int code = 0) {
        running = false;
        exitCode = code;
    }

    virtual void handleEvent(Event *) {}
    virtual void loop() = 0;

protected:
    virtual void sendEvent(std::shared_ptr<Event> event) {
        Actor::sendEvent(parent->getName(), event);
    }

private:
    Actor *parent;
    bool running = true;
    int exitCode = 0;
};

#endif // SUBSYSTEM_H
