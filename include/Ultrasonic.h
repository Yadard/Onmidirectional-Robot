#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include <NewPing.h>

namespace AntiCollisionSystem {
using pin_t = uint8_t;
using cm_t = float;
struct Data {
    cm_t cm_front;
    cm_t cm_left;
    cm_t cm_right;
};

class Base {
  public:
    virtual ~Base() {}

    virtual Data read(uint8_t it = 5);
};

class TriUltrassonic : public AntiCollisionSystem::Base {
  public:
    TriUltrassonic(pin_t trig, pin_t echo_front, pin_t echo_left, pin_t echo_right);

    Data read(uint8_t it = 5) override;

  private:
    struct _Sensors {
        NewPing front, left, right;

        _Sensors(pin_t trig, pin_t echo_front, pin_t echo_left, pin_t echo_right);
    } sensors;
};
} // namespace AntiCollisionSystem

#endif // ULTRASONIC_H