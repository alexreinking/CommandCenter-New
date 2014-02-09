#include <thread>
#include "servosystem.h"
using namespace std;

ServoSystem::ServoSystem(Actor *parent, std::string name, std::unique_ptr<TTYDevice> tty)
    : Subsystem(parent, name),
      servoDevice(move(tty))
{
    servoDevice->initialize();

    on<ServoEvent>([&] (ServoEvent *evt) {
        servoDevice->writeByte(0x80);
        servoDevice->writeByte(0x01);
        servoDevice->writeByte(0x02);
        servoDevice->writeByte(0x00);
        servoDevice->writeByte(evt->position);
    });
}

void ServoSystem::loop()
{
    this_thread::sleep_for(chrono::microseconds(10));
}
