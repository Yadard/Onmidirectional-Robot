#include <Motor.h>

Motor::ContinuousRotationServo::ContinuousRotationServo(Motor::Pin_t pin) { servo.attach(pin); }

/*
 * This shit might not work because this motors don't have a fucking label so my best guess is a chinese
 * ripoff of this servo motor: https://protosupplies.com/product/servo-motor-mg995-360-degree-continuous-rotation/
 */
bool Motor::ContinuousRotationServo::setPower(Watts_t watts) {
    if (watts > max_power_output)
        servo.write(180);
    else if (watts < -max_power_output)
        servo.write(0);
    else
        servo.write(map(watts, -60, 60, 0, 180));
}