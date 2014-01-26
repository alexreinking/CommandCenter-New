#ifndef TTYDEVICE_H
#define TTYDEVICE_H

#include <string>
#include <sstream>
#include <stdint.h>

class TTYDevice
{
public:
    virtual ~TTYDevice() {}

    virtual bool initialize() = 0;
    virtual bool isReady() const = 0;

    // Reads a single byte from the TTY or -1 if there are none available.
    virtual int32_t readByte() = 0;

    // Writes a single byte to the TTY
    virtual void writeByte(uint8_t value) = 0;

    // Write the given null terminated string.
    virtual void writeString(const char* str) = 0;

    template<class T>
    inline TTYDevice& operator << (T val)
    {
        std::stringstream out;
        out << val;
        writeString(out.str().c_str());
        return *this;
    }

};

#endif // TTYDEVICE_H
