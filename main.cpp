#include <iostream>
#include <functional>
#include "framework/commandcenter.h"
#include "lib/temperaturesensor.h"
#include "lib/transceiversystem.h"
#include "lib/timersystem.h"
#include "lib/imusystem.h"
#include "lib/servosystem.h"
#include "test/dummyuart.h"
#include "beaglebone/Uart.h"
using namespace std;

#define LED_1 P8_15
#define LED_2 P8_16
#define LED_3 P8_13
#define LED_4 P8_14
#define LED_5 P8_11
#define LED_6 P8_12
#define LED_7 P8_9
#define LED_8 P8_10

class MissionControl : public CommandCenterBase
{
public:
    MissionControl() {
        for(auto led : allLeds) {
            pinMode(led, OUTPUT);
            digitalWrite(led, LOW);
        }

        on<TemperatureEvent>("temp", [&] (TemperatureEvent *evt) {
            cout << "Temp = " << evt->temperature << " degrees C." << endl;
        });

        on<TimeEvent>("servoCreep", [&] (TimeEvent *evt) {
            static int direction = 127;
            sendEvent("servo", make_shared<ServoEvent>(direction));
            direction = (direction) ? 0 : 127;
        });

        on<TimeEvent>("flash", [&] (TimeEvent *evt) {
            // toggle the current LED
            int state = (lightsOn[currentLed]) ? LOW : HIGH;
            digitalWrite(allLeds[currentLed], state);
            lightsOn[currentLed] = !lightsOn[currentLed];

            // select the next LED
            currentLed = (currentLed - 1) % 8;
            if(currentLed < 0) currentLed += 8;
        });

        on<IMUEvent>("imu", [&] (IMUEvent *evt) {
            sendEvent("comm", make_shared<TransceiverEvent>("YAW", to_string(evt->data.yaw)));
        });
    }

    ~MissionControl() {
        for(auto led : allLeds) {
            pinMode(led, OUTPUT);
            digitalWrite(led, LOW);
        }
    }

private:
    int currentLed = 7;
    bool lightsOn[8] = { false };
    PIN allLeds[8] = { LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8 };
};

BEGIN_CONFIG(MissionControl)
MUX("BB-SPIDEV1");
MUX("BB-UART2");
MUX("uart4pinmux"); // There's no BB-UART3, so this one activates it.
//SUBSYSTEM(TemperatureSensor, "temp",
//          unique_ptr<ADCSensor3008>(new ADCSensor3008(7)));
SUBSYSTEM(TimerSystem<50>, "flash");
SUBSYSTEM(TimerSystem<500>, "servoCreep");
SUBSYSTEM(IMUSystem, "imu",
          unique_ptr<Uart>(new Uart(2, 115200)));
SUBSYSTEM(ServoSystem, "servo",
          unique_ptr<Uart>(new Uart(3, 4800)));
SUBSYSTEM(TransceiverSystem, "comm",
          unique_ptr<TTYDevice>(new DummyUart));
END_CONFIG
