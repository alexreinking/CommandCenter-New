#ifndef TRANSCEIVERSYSTEM_H
#define TRANSCEIVERSYSTEM_H

#include "../framework/subsystem.h"
#include "../beaglebone/TTYDevice.h"

class TransceiverSystem : public Subsystem
{
public:
    TransceiverSystem(Actor *parent, std::string name, std::unique_ptr<TTYDevice> tty);
    void loop();

private:
    std::unique_ptr<TTYDevice> tty;
    std::string buffer;
};

class TransceiverEvent : public Event
{
public:
    TransceiverEvent(std::string tag, std::string data) :
        tag(tag),
        data(data) {}
    std::string tag;
    std::string data;
};

#endif // TRANSCEIVERSYSTEM_H
