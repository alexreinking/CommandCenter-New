#include "imusystem.h"
#include <iostream>
using std::cout;

IMUSystem::IMUSystem(Actor *parent, std::string name, std::unique_ptr<TTYDevice> tty)
    : Subsystem(parent, name)
    , imuDevice(move(tty))
{
    imuDevice->initialize();
}

void IMUSystem::loop()
{
    if(decoder.decodeByte(imuDevice->readByte()))
        sendEvent(make_shared<IMUEvent>(decoder.getCurrentData()));
}
