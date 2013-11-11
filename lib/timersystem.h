#ifndef TIMERSYSTEM_H
#define TIMERSYSTEM_H

#include <thread>
#include <chrono>
#include "../framework/subsystem.h"

class TimeEvent;

template <int N>
class TimerSystem : public Subsystem
{
public:
    TimerSystem(EventLoop *events, std::string name = "time") :
        Subsystem(events),
        name(name) {}

    void loop() {
        std::this_thread::sleep_for(std::chrono::milliseconds(N));
        sendEvent(std::shared_ptr<Event>(new TimeEvent(this, N, name)));
    }

private:
    std::string name;
};

class TimeEvent : public Event
{
public:
    TimeEvent(Subsystem *sender, int interval, std::string name) :
        Event(sender, name),
        interval(interval) {}

    int interval = 0;
};

#endif // TIMERSYSTEM_H
