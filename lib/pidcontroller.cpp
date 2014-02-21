#include <chrono>
#include "pidcontroller.h"
using namespace std;
using namespace std::chrono;

PIDController::PIDController(long double K_p, long double K_i, long double K_d) {
    integral = 0;
    t0 = clock.now();
    t1 = clock.now();
    this->K_p = K_p;
    this->K_i = K_i;
    this->K_d = K_d;
}

void PIDController::setTarget(long double target) {
    this->target = target;
}

void PIDController::addPos(long double pos) {
    if(err_cur != 0)
        err_prev = err_cur;
    err_cur = pos - target;

    if(t1 > t0)
        t0 = t1;
    t1 = clock.now();

    auto delta = duration_cast<chrono::milliseconds>(t1-t0);

    if(err_cur < threshold)
        integral = 0;
    else
        integral += 0.5 * (err_cur + err_prev) * delta.count();
}

long double PIDController::getServoCommand() {
    auto delta = duration_cast<chrono::milliseconds>(t1-t0);
    return K_p * err_cur + K_i * integral + K_d * (err_cur - err_prev) / delta.count();
}
