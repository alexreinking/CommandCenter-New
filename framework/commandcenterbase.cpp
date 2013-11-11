#include <iostream>
#include <chrono>
#include <thread>
#include "commandcenterbase.h"

void CommandCenterBase::handleEvent(Event *event)
{
    std::vector<Callback> receivers = callbacks[event->getName()];
    for(Callback &f : receivers) {
        f(event);
    }
}

void CommandCenterBase::die()
{
    if(eventLoop)
        eventLoop->stop();
}
