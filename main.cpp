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
        on("temperature", [&] (Event *evt) {
            TemperatureEvent *tempEvt = dynamic_cast<TemperatureEvent*>(evt);
            cout << "Temp = " << tempEvt->temperature << " degrees C." << endl;
        });

        on("die", [&] (Event *evt) {
            die();
        });
    }
};

BEGIN_CONFIG(MissionControl)
MUX("BB-CommandCenter")
SUBSYSTEM(TemperatureSensor,
          unique_ptr<ADCSensor3008>(new ADCSensor3008(7)))
SUBSYSTEM(TimerSystem<2000>, "die")
END_CONFIG
