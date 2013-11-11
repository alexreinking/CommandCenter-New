#include <iostream>
#include <chrono>
#include <thread>
#include "subsystem.h"
#include "commandcenterbase.h"

void CommandCenterBase::handleEvent(Event *event)
{
    std::vector<Callback> receivers = callbacks[event->getSender()->getName()];
    for(Callback &f : receivers) {
        f(event);
    }
}

void CommandCenterBase::die()
{
    if(getEventLoop())
        getEventLoop()->stop();
}
