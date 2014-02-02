#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <memory>
#include "../framework/subsystem.h"
#include "../beaglebone/ADCSensor3008.h"

class TemperatureSensor : public Subsystem
{
public:
    TemperatureSensor(Actor *parent, std::string name,
                      std::unique_ptr<ADCSensor3008> adc);
    void loop();

private:
    std::unique_ptr<ADCSensor3008> adc;
};

class TemperatureEvent : public Event
{
public:
    TemperatureEvent(double temperature) : temperature(temperature) {}
    double temperature;
};


#endif // TEMPERATURESENSOR_H
