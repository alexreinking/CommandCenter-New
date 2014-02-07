#ifndef UART_WRAPPER
#define UART_WRAPPER

#include <stdint.h>
#include <termios.h>
#include "TTYDevice.h"

// A wrapper for the Linux's UART serial devices
class Uart : public TTYDevice
{
public:
    // Opens up the internal /dev/ttyO# device and sets it to use the given baud rate
    // Acceptable uart numbers are from 1 to 5. Different values are saturated to that range.
    // If anything goes wrong, the file handle is released and isReady() will return false.
    // Note that UART3 is write only.
    Uart(int uartNumber, int32_t baudRate);
    
    bool initialize();
    bool isReady() const { return isInitialized; }
    
    // Reads a single byte from the UART or -1 if there are none available.
    int32_t readByte();
    
    // Writes a single byte to the UART
    void writeByte(uint8_t value);
    
    // Write the given null terminated string.
    void writeString(const char* str);
    
private:
    speed_t rate;

    int uartNumber;
    int32_t baudRate;

    // File handle to our /dev/ttyO# device
    int uartHandle = -1;
    
    // Whether we initialized correctly, value returned by isReady
    bool isInitialized = false;
};

#endif
