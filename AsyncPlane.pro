TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=gnu++11 -pthread -O3
QMAKE_CXXFLAGS_WARN_ON = -Wl,--no-as-needed
QMAKE_LFLAGS += -Wl,--no-as-needed
LIBS += -pthread

SOURCES += main.cpp \
    framework/commandcenterbase.cpp \
    framework/eventloop.cpp \
    lib/temperaturesensor.cpp \
    beaglebone/_impl/Gpio.c \
    beaglebone/muxConfig.cpp \
    beaglebone/ADCSensor3008.cpp \
    framework/actor.cpp \
    framework/subsystem.cpp \
    beaglebone/Uart.cpp

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
    beaglebone/Uart.h

