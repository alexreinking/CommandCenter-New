#include <thread>
#include <chrono>
#include "temperaturesensor.h"

void TemperatureSensor::loop()
{
    double result;
    int32_t adcValue;

    if((adcValue = adc->readConversion()) != -1) {
        // We will assume 2.98V at 298K, adc is 10-bit
        // T = V * T0 / V0
        // V0 = 2.98 volts * 1023 max value / 5 volts max value
        // return value = T - 24850 so that the result is in celsius, not kelvin
        int32_t voltage = 5000*(adcValue/1023); //mV
        const double v0 = 5000*(610/1023); //mV. A value of 610 maps to 2.98V
        result = (voltage * 298.0/v0) - 273.15;
    } else {
        result = 50 * ((rand() % 1000) / 1000.0);
    }

    sendEvent(std::shared_ptr<Event>(new TemperatureEvent(this, result)));
}
