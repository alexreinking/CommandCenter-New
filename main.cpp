#include <iostream>
#include <memory>
#include "framework/commandcenter.h"
#include "lib/temperaturesensor.h"
#include "lib/timersystem.h"
using namespace std;

class MissionControl : public CommandCenterBase
{
public:
    MissionControl() {
        on<TemperatureEvent>("temp", [&] (TemperatureEvent *evt) {
            cout << "Temp = " << evt->temperature << " degrees C." << endl;
        });

        on<TimeEvent>("die", [&] (TimeEvent *evt) {
            die();
        });
    }
};

BEGIN_CONFIG(MissionControl)
MUX("BB-CommandCenter")
SUBSYSTEM(TemperatureSensor, "temp",
          unique_ptr<ADCSensor3008>(new ADCSensor3008(7)))
SUBSYSTEM(TimerSystem<500>, "die")
END_CONFIG
