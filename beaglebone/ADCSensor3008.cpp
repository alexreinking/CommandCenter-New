#include <mutex>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "ADCSensor3008.h"
#include "muxConfig.h"

// Our static class members
int ADCSensor3008::spiAdcHandle = -1;
int ADCSensor3008::nInstances = 0;

// A mutex to prevent problems with access to the shared spiAdcHandle
std::mutex spiAdcMutex;

ADCSensor3008::ADCSensor3008(int adcNumber)
{
    this->adcNumber = (adcNumber < 0) ? 0 : ((adcNumber > 7) ? 7 : adcNumber);
    this->lastConvertedValue = -1;

    if(nInstances == 0) {
        std::lock_guard<std::mutex> lg(spiAdcMutex);
        if(nInstances == 0) {
            int handle = open("/dev/spidev1.0", O_RDWR);

            // Set to SPI Mode 3, data in on falling edge, data out on rising edge
            uint8_t mode = 3;
            uint8_t bits = 8;
            uint32_t speed = 100000;

            if (handle != -1) {
                int result = ioctl(handle, SPI_IOC_WR_MODE, &mode);
                if(result != -1) {
                    result = ioctl(handle, SPI_IOC_WR_BITS_PER_WORD, &bits);
                    if(result != -1) {
                        result = ioctl(handle, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
                        spiAdcHandle = handle;
                    }
                }
            }

            if(spiAdcHandle == -1) {
                perror("beaglebone: ADCSensor3008");
            }
        }
    }

    std::lock_guard<std::mutex> lg(spiAdcMutex);
    nInstances++;
}

ADCSensor3008::~ADCSensor3008() {
    if(nInstances == 0 && spiAdcHandle != -1) {
        close(spiAdcHandle);
    }
}

int32_t ADCSensor3008::getConversion() const
{
    return this->lastConvertedValue;
}


int32_t ADCSensor3008::readConversion()
{
    // Ensure the read is atomic
    std::lock_guard<std::mutex> lg(spiAdcMutex);

    // Fail if we couldn't aquire the spi handle
    if(spiAdcHandle == -1)
        return -1;

    uint8_t tx[] = {(uint8_t)(0x18 + adcNumber), 0, 0};
    uint8_t rx[] = {0, 0, 0};

    struct spi_ioc_transfer spiTransfer;
    spiTransfer.tx_buf = (unsigned long)tx;
    spiTransfer.rx_buf = (unsigned long)rx;
    spiTransfer.len = 3;
    spiTransfer.delay_usecs = 0;
    spiTransfer.speed_hz = 0;
    spiTransfer.bits_per_word = 0;

    int result = ioctl(spiAdcHandle, SPI_IOC_MESSAGE(1), &spiTransfer);
    if (result == -1)
        return -1;

    // Byte 0 is nonsense 0xFF because our command hadn't been read yet,
    // first two bits of the byte 1 are no good either. (time it takes to do adc conversion).
    // Then we only want the top four bits of byte 2, as the 10-bit total value ends there.
    int32_t adcValue = ((rx[1] & 0x3F) << 4) | ((rx[2] & 0xF0) >> 4);
    lastConvertedValue = adcValue;

    return adcValue;
}
