#ifndef COMMANDCENTERBASE_H
#define COMMANDCENTERBASE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include "eventloop.h"

class Event;
typedef std::function<void(Event*)> Callback;

class CommandCenterBase
{
public:
    CommandCenterBase() {}
    virtual ~CommandCenterBase() {}

    template <typename Func>
    void on(std::string event, Func callback) {
        callbacks[event].push_back(callback);
    }

    virtual void handleEvent(Event *event);
    void setEventLoop(EventLoop *evt) { eventLoop = evt; }

protected:
    void die();

private:
    EventLoop *eventLoop = nullptr;
    std::unordered_map<std::string, std::vector<Callback>> callbacks;
};

#endif // COMMANDCENTERBASE_H
