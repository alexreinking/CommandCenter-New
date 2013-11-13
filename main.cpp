#include <iostream>
#include <functional>
#include "framework/commandcenter.h"
#include "lib/temperaturesensor.h"
#include "lib/timersystem.h"
using namespace std;
using namespace std::placeholders;

//Can also use a member function via
//bind(MissionControl::handleTemperature, this, std::placeholders::_1);

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
MUX("BB-CommandCenter");
SUBSYSTEM(TemperatureSensor, "temp",
          unique_ptr<ADCSensor3008>(new ADCSensor3008(7)));
SUBSYSTEM(TimerSystem<2000>, "die");
END_CONFIG
