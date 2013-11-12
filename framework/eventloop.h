#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <utility>
#include <queue>
#include <memory>
#include <vector>
#include <string>

class Actor;
class CommandCenterBase;
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

    void setReceiver(CommandCenterBase *receiver);
    void addActor(std::shared_ptr<Actor> subsys);

    void stop() { running = false; }
    int exec();

    void postEvent(std::string recipient, std::shared_ptr<Event> event);

private:
    bool running = true;
    CommandCenterBase *receiver;
    std::vector<std::shared_ptr<Actor>> actors;
    std::queue<Message> events;

    void processEvents();
    Actor *lookupActor(const std::string &name);
};

class Actor
{
public:
    Actor(std::string name = "", EventLoop *eventLoop = nullptr):
        eventLoop(eventLoop),
        name(name) {}

    virtual ~Actor() {}
    virtual void handleEvent(Event *event) = 0;

    void setName(const std::string &name) { this->name = name; }
    std::string getName() const { return name; }

    void setEventLoop(EventLoop *evt) { eventLoop = evt; }
    EventLoop *getEventLoop() const { return eventLoop; }

protected:
    virtual void sendEvent(std::string to, std::shared_ptr<Event> event) {
        event->setSender(this);
        eventLoop->postEvent(to, event);
    }

private:
    EventLoop *eventLoop;
    std::string name;
};

#endif // EVENTLOOP_H
