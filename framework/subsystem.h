#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include "eventloop.h"
#include "actor.h"

class Subsystem : public Actor
{
public:
    Subsystem(Actor *parent, std::string name):
        Actor(name, parent->getEventLoop()),
        parent(parent) {}
    virtual ~Subsystem() {}

    inline bool isRunning() const { return running; }
    int getExitCode() const { return exitCode; }

    virtual void stop(int code = 0) {
        running = false;
        exitCode = code;
    }

    virtual void handleEvent(shared_ptr<Event>);
    virtual void process();
    virtual void loop() = 0;

protected:
    virtual void sendEvent(std::shared_ptr<Event> event) {
        Actor::sendEvent(parent->getName(), event);
    }

    template <typename Arg>
    void on(std::function<void(Arg*)> callback) {
        Actor::on(parent->getName(), callback);
    }

private:
    std::queue<shared_ptr<Event>> events;
    std::mutex queueMutex;
    Actor *parent;

    bool running = true;
    int exitCode = 0;
};

#endif // SUBSYSTEM_H
