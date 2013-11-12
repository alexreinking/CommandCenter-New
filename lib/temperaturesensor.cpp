#include <iostream>
#include <thread>
#include <chrono>
#include "temperaturesensor.h"
using namespace std;

void TemperatureSensor::handleEvent(Event *)
{
    cout << "TemperatureSensor got an event in thread " << this_thread::get_id() << "." << endl;
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

    sendEvent(shared_ptr<Event>(new TemperatureEvent(result)));
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "TemperatureSensor sent an event from thread " << this_thread::get_id() << "." << endl;
}
