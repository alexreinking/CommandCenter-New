#include "commandcenterbase.h"

void CommandCenterBase::handleEvent(shared_ptr<Event> event)
{
    std::vector<Callback> receivers = callbacks[event->getSender()->getName()];
    for(Callback &f : receivers) {
        f(event.get());
    }
}

void CommandCenterBase::die()
{
    if(getEventLoop())
        getEventLoop()->stop();
}
