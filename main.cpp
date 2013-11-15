#include <iostream>
#include <functional>
#include "framework/commandcenter.h"
#include "lib/temperaturesensor.h"
#include "lib/transceiversystem.h"
#include "lib/timersystem.h"
#include "test/dummyuart.h"
using namespace std;

class MissionControl : public CommandCenterBase
{
public:
    MissionControl() {
        on<TemperatureEvent>("temp", [&] (TemperatureEvent *evt) {
            cout << "Temp = " << evt->temperature << " degrees C." << endl;
        });

        on<TimeEvent>("die", [&] (TimeEvent *evt) {
            cout << "Timer resolution is: " << evt->interval << endl;
            die();
        });
    }
};

BEGIN_CONFIG(MissionControl)
MUX("BB-CommandCenter");
SUBSYSTEM(TemperatureSensor, "temp",
          unique_ptr<ADCSensor3008>(new ADCSensor3008(7)));
SUBSYSTEM(TimerSystem<2000>, "die");
SUBSYSTEM(TransceiverSystem, "comm",
          unique_ptr<TTYDevice>(new DummyUart));
END_CONFIG
