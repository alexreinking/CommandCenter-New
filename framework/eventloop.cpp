#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include "eventloop.h"
#include "commandcenterbase.h"
#include "subsystem.h"

EventLoop::EventLoop() { }

void EventLoop::setReceiver(CommandCenterBase *receiver)
{
    this->receiver = receiver;
}

void EventLoop::addSubsystem(std::shared_ptr<Subsystem> subsys) {
    subsystems.push_back(subsys);
}

std::mutex queueLock;
std::mutex lock;
std::condition_variable not_empty;

int EventLoop::exec()
{
    std::vector<std::future<int>> exitCodes;
    for(std::shared_ptr<Subsystem> subsys : subsystems) {
        exitCodes.push_back(std::async(std::launch::async,
                                       [=]() {
            while(subsys->isRunning()) {
                subsys->loop();
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            return subsys->getExitCode();
        }));
    }

    std::unique_lock<std::mutex> l(lock);
    while(running) {
        not_empty.wait(l);
        if(queueLock.try_lock()) {   // Once we get the chance,
            while(!events.empty()) { // Dispatch all events
                std::shared_ptr<Event> nextEvent = events.front();
                events.pop();
                receiver->handleEvent(nextEvent.get());
            }
            queueLock.unlock();
        }
    }

    // Shutdown all subsystems
    for(std::shared_ptr<Subsystem> subsys : subsystems) {
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

void EventLoop::postEvent(std::shared_ptr<Event> event)
{
    queueLock.lock();
    events.push(event);
    queueLock.unlock();
    not_empty.notify_all();
}
