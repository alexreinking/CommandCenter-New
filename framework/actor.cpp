#include "actor.h"

void Actor::sendEvent(std::string to, std::shared_ptr<Event> event) {
    event->setSender(this);
    eventLoop->postEvent(to, event);
}
