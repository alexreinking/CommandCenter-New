#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <utility>
#include <memory>
#include "../framework/subsystem.h"
#include "../beaglebone/ADCSensor3008.h"

class TemperatureSensor : public Subsystem
{
public:
    TemperatureSensor(EventLoop *events, std::unique_ptr<ADCSensor3008> adc) :
        Subsystem(events),
        adc(std::move(adc)) {}
    void loop();

private:
    std::unique_ptr<ADCSensor3008> adc;
};

class TemperatureEvent : public Event
{
public:
    TemperatureEvent(Subsystem *sender, double temperature) :
        Event(sender, "temperature"),
        temperature(temperature)
    {}

    double temperature;
};

#endif // TEMPERATURESENSOR_H
