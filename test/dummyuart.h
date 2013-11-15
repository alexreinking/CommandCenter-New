#ifndef DUMMYUART_H
#define DUMMYUART_H

#include <termios.h>
#include <sys/time.h>
#include "../beaglebone/TTYDevice.h"

class DummyUart : public TTYDevice
{
public:
    DummyUart();
    ~DummyUart();

    virtual bool initialize() { return true; }
    virtual bool isReady() const { return true; }

    // Reads a single byte from the TTY or -1 if there are none available.
    virtual int32_t readByte();

    // Writes a single byte to the TTY
    virtual void writeByte(uint8_t value);

    // Write the given null terminated string.
    virtual void writeString(const char* str);

private:
    struct termios ttystate, ttysave;
};

#endif // DUMMYUART_H
