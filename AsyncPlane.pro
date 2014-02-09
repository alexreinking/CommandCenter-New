TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = arm-linux-gnueabihf-g++
QMAKE_CC = arm-linux-gnueabihf-g++
QMAKE_LINK = arm-linux-gnueabihf-g++

QMAKE_CXXFLAGS -= -m64
QMAKE_CFLAGS -= -m64
QMAKE_LFLAGS -= -m64

QMAKE_CXXFLAGS += -std=gnu++11 -O3
QMAKE_CXXFLAGS_WARN_ON = -Wl,--no-as-needed
QMAKE_LFLAGS += -Wl,--no-as-needed
LIBS += -pthread

# Win32 fixes
QMAKE_CXXFLAGS -= -fno-keep-inline-dllexport
QMAKE_LFLAGS -= -fno-keep-inline-dllexport
QMAKE_CFLAGS -= -fno-keep-inline-dllexport
QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions
QMAKE_LFLAGS_EXCEPTIONS_ON =
QMAKE_LFLAGS_CONSOLE =

SOURCES += main.cpp \
    framework/commandcenterbase.cpp \
    framework/eventloop.cpp \
    lib/temperaturesensor.cpp \
    beaglebone/_impl/Gpio.c \
    beaglebone/muxConfig.cpp \
    beaglebone/ADCSensor3008.cpp \
    framework/actor.cpp \
    framework/subsystem.cpp \
    beaglebone/Uart.cpp \
    lib/transceiversystem.cpp \
    beaglebone/crc8.cpp \
    test/dummyuart.cpp \
    lib/imusystem.cpp \
    lib/imu/imudecoder.cpp \
    lib/imu/nmeaParse.cpp \
    lib/servosystem.cpp

HEADERS += \
    framework/commandcenter.h \
    framework/commandcenterbase.h \
    framework/subsystem.h \
    framework/eventloop.h \
    lib/temperaturesensor.h \
    lib/timersystem.h \
    beaglebone/_impl/Gpio.h \
    beaglebone/_impl/am335x.h \
    beaglebone/muxConfig.h \
    beaglebone/ADCSensor3008.h \
    framework/actor.h \
    beaglebone/Uart.h \
    lib/transceiversystem.h \
    beaglebone/crc8.h \
    beaglebone/TTYDevice.h \
    test/dummyuart.h \
    lib/imusystem.h \
    lib/imu/imudecoder.h \
    lib/imu/nmeaParse.h \
    lib/servosystem.h

