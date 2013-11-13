#include <iostream>
#include <thread>
#include <chrono>
#include "temperaturesensor.h"
using namespace std;

TemperatureSensor::TemperatureSensor(Actor *parent, std::string name,
                                     std::unique_ptr<ADCSensor3008> adc) :
    Subsystem(parent, name),
    adc(std::move(adc))
{
    on<TemperatureEvent>([&] (TemperatureEvent *evt){
        std::cout << "Got an event in thread " << std::this_thread::get_id() << std::endl;
    });
}

void TemperatureSensor::loop()
{
    double result;
    int32_t adcValue;

    if((adcValue = adc->readConversion()) != -1) {
        int32_t voltage = 5000*(adcValue/1023); //mV
        const double v0 = 5000*(610/1023); //mV. A value of 610 maps to 2.98V
        result = (voltage * 298.0/v0) - 273.15;
    } else {
        result = 50 * ((rand() % 1000) / 1000.0);
    }

    sendEvent(make_shared<TemperatureEvent>(result));
//    this_thread::sleep_for(chrono::milliseconds(50));
}
