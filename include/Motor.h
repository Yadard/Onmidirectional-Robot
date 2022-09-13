#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Event.h>
#include <Servo.h>

namespace Motor {

using Pin_t = uint8_t;
using Watts_t = int;

constexpr inline Watts_t counterClockWise(Watts_t watts) { return watts; }

constexpr inline Watts_t clockWise(Watts_t watts) { return -watts; }

class Base {
  public:
    // * Use the Motor::counterClockWise & Motor::clockWise to control rotation direction.
    //@param Watts: set the power output of the motor to the specified power, if
    // the power is to much for the motor, it'll cap at the motor max power output.
    //@return true if everything worked well and false if everything went wrong.
    virtual bool setPower(Watts_t watts);

    virtual Watts_t maxPower();

    virtual Pin_t pin();

    virtual ~Base() {}
};

/*
 * I literally received a fucking continuous rotation servo motor whitout any kind of labeling
 * it is literally a black pandora box, there is no fucking way to tell how to ajust the power output
 * making me believe the 3 i'm using for the robot have diferrent power output. making me going insane
 * trying to make this piece of garbage walk straight.
 */
class ContinuousRotationServo : public Motor::Base {
  public:
    //@param pin: pin connected to the signal pin of the Servo.
    ContinuousRotationServo(Pin_t pin, uint16_t pwm_max = 180, bool inverse = false);

    bool setPower(Watts_t watts) override;
    Pin_t pin() override;

    Watts_t maxPower() override;

  private:
    Servo servo;
    uint16_t pwm_max;
    bool inverse;
};
} // namespace Motor

namespace Event {
class Motor : public Event::Base {
  public:
    Motor(::Motor::Base *motor, String &valor);
    void operator()() override;
    ~Motor() override;

  private:
    ::Motor::Base *motor;
    const String valor;
};
} // namespace Event

#endif // MOTOR_H