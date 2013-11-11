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
    static CommandCenterBase* create(EventLoop *events) { \
        CommandCenterBase* commandCenter = new CLASS_NAME(); \
        commandCenter->setName(typeid(CLASS_NAME).name()); \
        events->setReceiver(commandCenter);

#define SUBSYSTEM(SUBSYS, ...) \
        events->addSubsystem(shared_ptr<Subsystem>(new SUBSYS(events, ##__VA_ARGS__)));

#define MUX(name) \
        if(muxConfig(name) == -1) { \
            cerr << "framework: MUX: Could not configure " \
                 << name << endl; \
        }

#define END_CONFIG \
        commandCenter->setEventLoop(events); \
        return commandCenter; \
    } \
}; \
int main() {\
    srand(time(NULL)); \
    EventLoop events; \
    unique_ptr<CommandCenterBase> userProgram(Injector::create(&events)); \
    return events.exec(); \
}

#endif // COMMANDCENTER_H
