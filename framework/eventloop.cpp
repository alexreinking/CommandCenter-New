#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include "eventloop.h"
#include "commandcenterbase.h"
#include "subsystem.h"

EventLoop::EventLoop() { }

void EventLoop::addActor(std::shared_ptr<Actor> subsys) {
    actors.push_back(subsys);
}

int EventLoop::exec()
{
    std::vector<std::future<int>> exitCodes;
    for(std::shared_ptr<Actor> actor : actors) {
        if(Subsystem *subsys = dynamic_cast<Subsystem*>(actor.get())) {
            exitCodes.push_back(std::async(std::launch::async,
                                           [=]() {
                while(subsys->isRunning()) {
                    subsys->process();
                    subsys->loop();
                }
                return subsys->getExitCode();
            }));
        }
    }

    while(running)
        processEvents();

    // Shutdown all subsystems
    for(std::shared_ptr<Actor> actor : actors) {
        if(Subsystem *subsys = dynamic_cast<Subsystem*>(actor.get()))
            if(subsys->isRunning())
                subsys->stop();
    }

    // Check for errors
    int code = 0;
    for(std::future<int> &fut : exitCodes) {
        if(fut.get() != 0)
            code = fut.get();
    }

    // Return 0 or some error
    return code;
}

std::mutex emptyLock;
std::condition_variable not_empty;

void EventLoop::postEvent(std::string recipient, std::shared_ptr<Event> event)
{
    std::unique_lock<std::mutex> l(emptyLock);
    events.push(make_pair(recipient, event));
    not_empty.notify_all();
}

void EventLoop::processEvents()
{
    std::unique_lock<std::mutex> l(emptyLock);
    not_empty.wait(l);

    while(!events.empty()) { // Dispatch all events
        Message nextMessage = events.front();
        std::shared_ptr<Event> nextEvent = nextMessage.second;
        events.pop();
        if(lookupActor(nextMessage.first)) {
            lookupActor(nextMessage.first)->handleEvent(nextEvent);
        }
    }
}

Actor *EventLoop::lookupActor(const std::string &name)
{
    for(std::shared_ptr<Actor> actor : actors) {
        if(actor->getName() == name)
            return actor.get();
    }
    return nullptr;
}
