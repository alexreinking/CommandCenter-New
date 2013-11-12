#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <utility>
#include <queue>
#include <memory>
#include <vector>
#include <string>

class Actor;
class EventLoop;

class Event
{
public:
    Event(Actor *sender = nullptr):sender(sender) {}
    virtual ~Event() {}

    Actor *getSender() const { return sender; }
    void setSender(Actor *sender) { this->sender = sender; }

private:
    Actor *sender;
};

typedef std::pair<std::string, std::shared_ptr<Event>> Message;

class EventLoop
{
public:
    EventLoop();

    void addActor(std::shared_ptr<Actor> subsys);

    void stop() { running = false; }
    int exec();

    void postEvent(std::string recipient, std::shared_ptr<Event> event);

private:
    bool running = true;
    std::vector<std::shared_ptr<Actor>> actors;
    std::queue<Message> events;

    void processEvents();
    Actor *lookupActor(const std::string &name);
};

#endif // EVENTLOOP_H
