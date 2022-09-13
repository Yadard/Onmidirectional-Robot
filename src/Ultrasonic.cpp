#include <Ultrasonic.h>

Ultrasonic::Sensor::Sensor(pin_t trig, pin_t echo_front, pin_t echo_left, pin_t echo_right)
    : sensor_front(trig, echo_front), sensor_left(trig, echo_left), sensor_right(trig, echo_right) {}

Ultrasonic::Sensor::Data Ultrasonic::Sensor::read(uint8_t it) {
    Data data;
    data.cm_front = (sensor_front.ping_median(it) / 2) * 0.0343;
    data.cm_left = (sensor_left.ping_median(it) / 2) * 0.0343;
    data.cm_right = (sensor_right.ping_median(it) / 2) * 0.0343;

    return data;
}