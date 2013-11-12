#include "subsystem.h"

void Subsystem::handleEvent(shared_ptr<Event> event)
{
    std::unique_lock<std::mutex> l(queueMutex);
    events.push(event);
}

void Subsystem::process()
{
    std::unique_lock<std::mutex> l(queueMutex);
    while(!events.empty()) {
        shared_ptr<Event> event = events.front();
        std::vector<Callback> receivers = callbacks[event->getSender()->getName()];
        for(Callback &f : receivers) {
            f(event.get());
        }
        events.pop();
    }
}
