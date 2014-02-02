#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "muxConfig.h"

int muxConfig(const char* capeName)
{
    int muxFile = open("/sys/devices/bone_capemgr.9/slots", O_WRONLY);
    if (muxFile == -1) {
        perror(capeName);
        return -1;
    }
    int nbytes = write(muxFile, capeName, strlen(capeName));
    close(muxFile);
    return nbytes == strlen(capeName);
}
