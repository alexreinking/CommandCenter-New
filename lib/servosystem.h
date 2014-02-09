#ifndef SERVOSYSTEM_H
#define SERVOSYSTEM_H

#include "../framework/subsystem.h"
#include "../beaglebone/TTYDevice.h"

class ServoEvent : public Event
{
public:
    ServoEvent(unsigned char pos) : position(pos) {}
    unsigned char position; // 0 = left, 127 = right
};

class ServoSystem : public Subsystem
{
public:
    ServoSystem(Actor *parent, std::string name, std::unique_ptr<TTYDevice> tty);
    void loop();

private:
    std::unique_ptr<TTYDevice> servoDevice;
};

#endif // SERVOSYSTEM_H
