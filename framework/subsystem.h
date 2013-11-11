#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>
#include "eventloop.h"
#include "commandcenterbase.h"

class Subsystem : public Actor
{
public:
    Subsystem(EventLoop *eventLoop, std::string name):
        Actor(name, eventLoop) {}
    virtual ~Subsystem() {}

    bool isRunning() const { return running; }
    int getExitCode() const { return exitCode; }

    void stop(int code = 0) {
        running = false;
        exitCode = code;
    }

    virtual void handleEvent(Event *) {}
    virtual void loop() = 0;

private:
    bool running = true;
    int exitCode = 0;
};

#endif // SUBSYSTEM_H
