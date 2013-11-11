#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>
#include "eventloop.h"
#include "commandcenterbase.h"

class Subsystem
{
public:
    Subsystem(EventLoop *eventLoop):eventLoop(eventLoop) {}
    virtual ~Subsystem() { }

    bool isRunning() const { return running; }
    int getExitCode() const { return exitCode; }

    void stop(int code = 0) {
        running = false;
        exitCode = code;
    }

    virtual void loop() = 0;

    void sendEvent(std::shared_ptr<Event> event) {
        eventLoop->postEvent(event);
    }

private:
    EventLoop *eventLoop;
    bool running = true;
    int exitCode = 0;
};

#endif // SUBSYSTEM_H
