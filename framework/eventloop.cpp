#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include "eventloop.h"
#include "commandcenterbase.h"
#include "subsystem.h"
using std::cerr;
using std::endl;

EventLoop::EventLoop() { }

void EventLoop::addActor(std::shared_ptr<Actor> subsys) {
    actors.push_back(subsys);
}

int EventLoop::exec()
{
    std::vector<std::thread> workers;
    for(std::shared_ptr<Actor> actor : actors) {
        if(Subsystem *subsys = dynamic_cast<Subsystem*>(actor.get())) {
            workers.push_back(std::thread([=]() {
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

    for(auto& thread : workers) {
        thread.join();
    }

    // Return 0 or some error
    return 0;
}

std::mutex emptyLock;
std::recursive_mutex queueLock;
std::condition_variable not_empty;

void EventLoop::postEvent(std::string recipient, std::shared_ptr<Event> event)
{
    std::unique_lock<std::recursive_mutex> q(queueLock);
    events.push(make_pair(recipient, event));
    not_empty.notify_all();
}

void EventLoop::processEvents()
{
    std::unique_lock<std::mutex> l(emptyLock);
    not_empty.wait(l);

    std::unique_lock<std::recursive_mutex> q(queueLock);
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
