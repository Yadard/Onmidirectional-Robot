#include <Motor.h>

Motor::ContinuousRotationServo::ContinuousRotationServo(Motor::Pin_t pin, uint16_t pwm_max, bool inverse) : pwm_max(pwm_max), inverse(inverse) {
    servo.attach(pin);
}

/*
 * Could be this Servo: https://protosupplies.com/product/servo-motor-mg995-360-degree-continuous-rotation/
 */
bool Motor::ContinuousRotationServo::setPower(Watts_t watts) {
    if (watts > maxPower())
        servo.write(inverse ? 0 : 180);
    else if (watts < -maxPower())
        servo.write(inverse ? 180 : 0);
    else
        servo.write(inverse ? map(watts, -60, 60, pwm_max, 0) : map(watts, -60, 60, 0, pwm_max));
    return true;
}

Motor::Watts_t Motor::ContinuousRotationServo::maxPower() { return 60; }

Motor::Pin_t Motor::ContinuousRotationServo::pin() { return pwm_max; }