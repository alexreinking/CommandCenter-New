#include "pidcontroller.h"
using namespace std;

PIDController::PIDController(long double K_p, long double K_i, long double K_d) {
    integral = 0;
    t0 = 0;
    t1 = 0;
    this->K_p = K_p;
    this->K_i = K_i;
    this->K_d = K_d;
}

void setTarget(long double target) {
    this->target = target;
}

void PIDController::addData(long double pos) {
    if(err_cur != 0)
        err_prev = err_cur;
    err_cur = pos - target;

    if(t1 > 0)
        t0 = t1;
    t1 = clock.now();

    if(err_cur < threshold)
        integral = 0;
    else
        integral += 0.5 * (err_cur + err_prev) * (t1 - t0);
}

long double getServoCommand() {
    servo_command = K_p * err_cur + K_i * integral + K_d * (err_cur - err_prev) / (t1 - t0);
    return servo_command;
}


