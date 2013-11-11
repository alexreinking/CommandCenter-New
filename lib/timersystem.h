#ifndef TIMERSYSTEM_H
#define TIMERSYSTEM_H

#include <thread>
#include <chrono>
#include "../framework/subsystem.h"
using namespace std::chrono;
using std::shared_ptr;

class TimeEvent : public Event
{
public:
    TimeEvent(int interval) : interval(interval) {}
    int interval = 0;
};

template <int N>
class TimerSystem : public Subsystem
{
public:
    TimerSystem(EventLoop *events, std::string name) :
        Subsystem(events, name),
        name(name) {}

    void loop() {
        time_point<system_clock> start, end;
        start = system_clock::now();
        std::this_thread::sleep_for(milliseconds(N));
        end = system_clock::now();

        auto elapsedMillis = duration_cast<milliseconds>(end-start);
        sendEvent(shared_ptr<Event>(new TimeEvent(elapsedMillis.count())));
    }

private:
    std::string name;
};

#endif // TIMERSYSTEM_H
