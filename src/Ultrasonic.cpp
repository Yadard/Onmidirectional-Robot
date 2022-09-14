#include <Ultrasonic.h>

AntiCollisionSystem::TriUltrassonic::TriUltrassonic(pin_t trig, pin_t echo_front, pin_t echo_left, pin_t echo_right)
    : sensors(trig, echo_front, echo_left, echo_right) {}

AntiCollisionSystem::Data AntiCollisionSystem::TriUltrassonic::read(uint8_t it) {
    Data data;
    float fx = 0, fy = 0;

    data.cm_front = (sensors.front.ping_median(it) / 2) * 0.0343;
    data.cm_left = (sensors.left.ping_median(it) / 2) * 0.0343;
    data.cm_right = (sensors.right.ping_median(it) / 2) * 0.0343;

    return data;
}

AntiCollisionSystem::TriUltrassonic::_Sensors::_Sensors(pin_t trig, pin_t echo_front, pin_t echo_left, pin_t echo_right)
    : front(trig, echo_front), left(trig, echo_left), right(trig, echo_right) {}