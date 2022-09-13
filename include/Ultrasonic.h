#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include <NewPing.h>

namespace Ultrasonic {
using pin_t = uint8_t;

class Sensor {
  public:
    using cm_t = float;
    Sensor(pin_t trig, pin_t echo_front, pin_t echo_left, pin_t echo_right);

    struct Data {
        cm_t cm_front;
        cm_t cm_left;
        cm_t cm_right;
    };

    Data read(uint8_t it = 5);

  private:
    NewPing sensor_front, sensor_left, sensor_right;
};
} // namespace Ultrasonic

#endif // ULTRASONIC_H