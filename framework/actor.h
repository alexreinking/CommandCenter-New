#ifndef ACTOR_H
#define ACTOR_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <string>
#include "eventloop.h"
using std::shared_ptr;

typedef std::function<void(Event*)> Callback;

class Actor
{
public:
    Actor(std::string name = "", EventLoop *eventLoop = nullptr):
        eventLoop(eventLoop),
        name(name) {}

    virtual ~Actor() {}
    virtual void handleEvent(shared_ptr<Event> event) = 0;

    void setName(const std::string &name) { this->name = name; }
    std::string getName() const { return name; }

    void setEventLoop(EventLoop *evt) { eventLoop = evt; }
    EventLoop *getEventLoop() const { return eventLoop; }

protected:
    std::unordered_map<std::string, std::vector<Callback>> callbacks;

    virtual void sendEvent(std::string to, std::shared_ptr<Event> event);
    template <typename Arg>
    void on(const std::string &sender, std::function<void(Arg*)> callback) {
        callbacks[sender].push_back([callback] (Event *evt) {
            if(Arg *arg = dynamic_cast<Arg*>(evt))
                callback(arg);
            else std::cerr << "Failed to cast from " << evt->getSender()->getName()
                      << ". Expected " << typeid(Arg).name() << ", got "
                      << typeid(*evt).name() << "." << std::endl;
        });
    }

private:
    EventLoop *eventLoop;
    std::string name;
};

#endif // ACTOR_H
