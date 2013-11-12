#ifndef COMMANDCENTERBASE_H
#define COMMANDCENTERBASE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include "eventloop.h"

class Event;
typedef std::function<void(Event*)> Callback;

class CommandCenterBase : public Actor
{
public:
    CommandCenterBase() {}
    virtual ~CommandCenterBase() {}
    virtual void handleEvent(Event *event);

protected:
    template <typename Arg>
    void on(const std::string &sender, std::function<void(Arg*)> callback) {
        callbacks[sender].push_back([callback] (Event *evt) {
            if(Arg *arg = dynamic_cast<Arg*>(evt))
                callback(arg);
        });
    }

    void die();

private:
    std::unordered_map<std::string, std::vector<Callback>> callbacks;
};

#endif // COMMANDCENTERBASE_H
