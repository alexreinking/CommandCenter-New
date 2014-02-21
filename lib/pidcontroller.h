#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H
#include <chrono>
using namespace std;
using namespace std::chrono;

class PIDController
{

public:
    PIDController(long double K_p, long double K_i, long double K_d);
    void addPos(long double pos);
    long double getServoCommand();
    void setTarget(long double target);
    void start();
    void stop();

private:
    long double K_p;
    long double K_i;
    long double K_d;
    long double servo_command;
    const long double threshold = 0.1;
    long double integral;
    long double err_cur = 0;
    long double err_prev = 0;
    high_resolution_clock clock;
    high_resolution_clock::time_point t0;
    high_resolution_clock::time_point t1;
    long double target = 0;
};

#endif // PIDCONTROLLER_H
