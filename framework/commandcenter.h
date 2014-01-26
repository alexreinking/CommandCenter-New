#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include <typeinfo>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <signal.h>
#include "../beaglebone/muxConfig.h"
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
        commandCenter->addSubsystem<SUBSYS>(commandCenter.get(), ##__VA_ARGS__);

#define MUX(name) \
        if(muxConfig(name) == -1) { \
            cerr << "framework: MUX: Could not configure " \
                 << name << endl; \
        }

#define END_CONFIG \
        return commandCenter; \
    } \
}; \
namespace framework { \
    std::function<void(int)> sigHandler;\
    void handleSigInt(int a) { \
        sigHandler(a);\
    } \
};\
int main() {\
    srand(time(NULL)); \
    EventLoop events; \
    framework::sigHandler = [&] (int _) { events.stop(); }; \
    signal(SIGINT, framework::handleSigInt); \
    auto userProgram(Injector::create(&events)); \
    return events.exec(); \
}

#endif // COMMANDCENTER_H
