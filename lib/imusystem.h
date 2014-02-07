#ifndef IMUSYSTEM_H
#define IMUSYSTEM_H

#include "imu/imudecoder.h"
#include "../framework/subsystem.h"
#include "../beaglebone/TTYDevice.h"

class IMUEvent : public Event
{
public:
    IMUEvent(ImuCalcData icd) : data(icd) {}
    ImuCalcData data;
};

class IMUSystem : public Subsystem
{
public:
    IMUSystem(Actor *parent, std::string name, std::unique_ptr<TTYDevice> tty);
    void loop();

private:
    std::unique_ptr<TTYDevice> imuDevice;
    IMUDecoder decoder;
};

#endif // IMUSYSTEM_H
