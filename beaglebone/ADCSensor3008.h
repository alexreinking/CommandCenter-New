#ifndef ADC_SENSOR_3008
#define ADC_SENSOR_3008

#include <stdint-gcc.h>

class ADCSensor3008
{
public:
    // Creates an ADCSensor3008 to read from the specified adc number of the physical device,
    // which is from 0 to 7. Different values will be saturated into this range.
    ADCSensor3008(int adcNumber);
    ~ADCSensor3008();

    // Value is the 10-bit ADC conversion.
    // Returns the last converted value read from the sensor.
    // return -1 if no conversion has yet been made.
    int32_t getConversion() const;

    // Value is the 10-bit ADC conversion
    // Queries the sensor to convert an ADC value according to the adcNumber of this ADCSensor3008.
    // Returns -1 if the conversion is unable to succeed.
    int32_t readConversion();

private:
    // The adc pin number to read from the physical device
    int adcNumber;

    // The last 10-bit value we converted, made by readConversion.
    int32_t lastConvertedValue;

    // A handle to the linux SPI device file, shared by all ADCSensor3008 instances
    static int spiAdcHandle;
    static int nInstances;
};

#endif
