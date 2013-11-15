#include <thread>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "dummyuart.h"
using namespace std;

DummyUart::DummyUart()
{
    tcgetattr(fileno(stdin), &ttystate);
    ttysave = ttystate;

    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 1;

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    cerr << "test: DummyUart: Initialized" << endl;
}

DummyUart::~DummyUart()
{
    tcsetattr(fileno(stdin), TCSANOW, &ttysave);
    cerr << "test: DummyUart: Reset terminal" << endl;
}

// Reads a single byte from the TTY or -1 if there are none available.
int32_t DummyUart::readByte()
{
    struct timeval tv;
    fd_set readset;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&readset);
    FD_SET(STDIN_FILENO, &readset);
    select(STDIN_FILENO+1, &readset, NULL, NULL, &tv);

    if(FD_ISSET(STDIN_FILENO, &readset) != 0)
        return fgetc(stdin);
    return -1;
}

// Writes a single byte to the TTY
void DummyUart::writeByte(uint8_t value)
{
    cout << (char) value << flush;
}

// Write the given null terminated string.
void DummyUart::writeString(const char* str)
{
    cout << str << flush;
}
