#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include "Uart.h"
#include "muxConfig.h"

#include <iostream>
using std::cout;
using std::endl;

Uart::Uart(int uartNumber, int32_t baudRate)
    : uartNumber((uartNumber < 1) ? 1 : ((uartNumber > 5) ? 5 : uartNumber)),
      baudRate(baudRate)
{
    cout << "Uart: Uart: uartNumber = " << uartNumber
         << " baudRate = " << baudRate << endl;
}

bool Uart::initialize()
{
    if(isInitialized && uartHandle != -1) {
        close(uartHandle);
    }

    switch (baudRate)
    {
    case 50:      rate = B50;      break;
    case 75:      rate = B75;      break;
    case 110:     rate = B110;     break;
    case 134:     rate = B134;     break;
    case 150:     rate = B150;     break;
    case 200:     rate = B200;     break;
    case 300:     rate = B300;     break;
    case 600:     rate = B600;     break;
    case 1200:    rate = B1200;    break;
    case 1800:    rate = B1800;    break;
    case 2400:    rate = B2400;    break;
    case 4800:    rate = B4800;    break;
    case 9600:    rate = B9600;    break;
    case 19200:   rate = B19200;   break;
    case 38400:   rate = B38400;   break;
    case 115200:  rate = B115200;  break;
    case 230400:  rate = B230400;  break;
    case 460800:  rate = B460800;  break;
    case 500000:  rate = B500000;  break;
    case 576000:  rate = B576000;  break;
    case 921600:  rate = B921600;  break;
    case 1000000: rate = B1000000; break;
    case 1500000: rate = B1500000; break;
    case 2000000: rate = B2000000; break;
    case 2500000: rate = B2500000; break;
    case 3000000: rate = B3000000; break;
    case 3500000: /* rate = B3500000; */ break;
    case 4000000: /* rate = B4000000; */ break;
    default: return false;
    }

    char fileName[] = "/dev/ttyO0";
    // Play with the characters in the name to get the paths right;
    fileName[9] += uartNumber;

    // This should be a parameter.
    uartHandle = open(fileName, (uartNumber == 3) ? O_WRONLY : O_RDWR);

    if (uartHandle == -1)
        return (isInitialized = false);

    struct termios terminalOptions;
    tcgetattr(uartHandle, &terminalOptions);
    cfsetispeed(&terminalOptions, rate);
    cfsetospeed(&terminalOptions, rate);

    // disable canonical mode processing in the line discipline driver
    // So everything is read in instantly from stdin!
    // Also, don't echo back characters... so we only see what we receive!
    terminalOptions.c_lflag &= ~(ECHO | ICANON);

    // We do not want to block with getchar
    // We have no minimum break in receiving characters (VTIME = 0)
    // and we have no minimum number of characters to receive (VMIN = 0)
    terminalOptions.c_cc[VTIME] = 0;
    terminalOptions.c_cc[VMIN] = 0;

    if (tcsetattr(uartHandle, TCSANOW, &terminalOptions) == -1)
        return (isInitialized = false);

    return (isInitialized = true);
}

int32_t Uart::readByte()
{
    if (!isInitialized)
        return -1;

    uint8_t value;
    int result = read(uartHandle, &value, sizeof(uint8_t));

    // If we didn't read a single byte... we have a problem.
    if (result != sizeof(uint8_t))
        return -1;
    return value;
}

void Uart::writeByte(uint8_t value)
{
    if (isInitialized)
        write(uartHandle, &value, sizeof(uint8_t));
}

void Uart::writeString(const char *str)
{
    if (isInitialized)
        write(uartHandle, str, strlen(str));\
}
