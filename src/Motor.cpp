#include <Motor.h>

Motor::ContinuousRotationServo::ContinuousRotationServo(Motor::Pin_t pin, uint16_t pwm_max) : pwm_max(pwm_max) { servo.attach(pin); }

/*
 * This shit might not work because this motors don't have a fucking label so my best guess is a chinese
 * ripoff of this servo motor: https://protosupplies.com/product/servo-motor-mg995-360-degree-continuous-rotation/
 */
bool Motor::ContinuousRotationServo::setPower(Watts_t watts) {
    if (watts > maxPower())
        servo.write(180);
    else if (watts < -maxPower())
        servo.write(0);
    else
        servo.write(map(watts, -60, 60, 0, pwm_max));
    return true;
}

Motor::Watts_t Motor::ContinuousRotationServo::maxPower() { return 60; }

Motor::Pin_t Motor::ContinuousRotationServo::pin() { return pwm_max; }

Event::Motor::Motor(::Motor::Base *motor, String &valor) : motor(motor), valor(valor) { }
Event::Motor::~Motor() { }

void Event::Motor::operator()() {
    if (this->valor.length()) {
        switch (valor[0]) {
        case '.':
            this->motor->setPower(0);
            break;
        case '>':
            this->motor->setPower(::Motor::clockWise(this->motor->maxPower()));
            break;
        case '<':
            this->motor->setPower(::Motor::counterClockWise(this->motor->maxPower()));
            break;

        default:
            this->motor->setPower(valor.toInt());
            break;
        }
    }

    delete this;
}