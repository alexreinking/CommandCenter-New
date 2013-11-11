#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <queue>
#include <memory>
#include <vector>
#include <string>

class Subsystem;
class CommandCenterBase;

class Event
{
public:
    Event(Subsystem *sender, std::string name):sender(sender),name(name) {}
    virtual ~Event() {}

    Subsystem *getSender() const { return sender; }
    std::string getName() const { return name; }

private:
    Subsystem *sender;
    std::string name;
};

class EventLoop
{
public:
    EventLoop();

    void setReceiver(CommandCenterBase *receiver);
    void addSubsystem(std::shared_ptr<Subsystem> subsys);

    void stop() { running = false; }
    int exec();

    void postEvent(std::shared_ptr<Event> event);

private:
    bool running = true;
    CommandCenterBase *receiver;
    std::vector<std::shared_ptr<Subsystem>> subsystems;
    std::queue<std::shared_ptr<Event>> events;
};

#endif // EVENTLOOP_H
