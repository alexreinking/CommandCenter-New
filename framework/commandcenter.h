#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include <typeinfo>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <utility>
#include <iostream>
#include "beaglebone/muxConfig.h"
#include "commandcenterbase.h"
#include "eventloop.h"
#include "subsystem.h"
using std::move;
using std::shared_ptr;
using std::unique_ptr;
using std::cerr;
using std::endl;

#define BEGIN_CONFIG(CLASS_NAME) \
class Injector\
{ \
public: \
    static shared_ptr<CommandCenterBase> create(EventLoop *events) { \
        auto commandCenter = shared_ptr<CommandCenterBase>(new CLASS_NAME()); \
        commandCenter->setName(#CLASS_NAME); \
        events->addActor(commandCenter); \
        commandCenter->setEventLoop(events); \

#define SUBSYSTEM(SUBSYS, ...) \
        events->addActor(shared_ptr<Subsystem>(new SUBSYS(commandCenter.get(), ##__VA_ARGS__)));

#define MUX(name) \
        if(muxConfig(name) == -1) { \
            cerr << "framework: MUX: Could not configure " \
                 << name << endl; \
        }

#define END_CONFIG \
        return commandCenter; \
    } \
}; \
int main() {\
    srand(time(NULL)); \
    EventLoop events; \
    auto userProgram(Injector::create(&events)); \
    return events.exec(); \
}

#endif // COMMANDCENTER_H
