#include <Movement.h>

Movement::Onmidirectional_3Wheels::Onmidirectional_3Wheels(Motor::Base *MotorA, Motor::Base *MotorB, Motor::Base *MotorC) {
    motors[0] = MotorA;
    motors[1] = MotorB;
    motors[2] = MotorC;

    movement_matrix[0][0] = cos(radians(240));
    movement_matrix[0][1] = cos(radians(120));
    movement_matrix[0][2] = cos(radians(0));

    movement_matrix[1][0] = sin(radians(240));
    movement_matrix[1][1] = sin(radians(120));
    movement_matrix[1][2] = sin(radians(0));

    movement_matrix[2][0] = 1;
    movement_matrix[2][1] = 1;
    movement_matrix[2][2] = 1;

    Matrix.Invert((mtx_type *)movement_matrix, 3);
    stop();
}

bool Movement::Onmidirectional_3Wheels::applySpeed(double sx, double sy) {

    for (size_t i = 0; i < 3; i++) {
        int watt = sx * movement_matrix[i][0] + sy * movement_matrix[i][1];
        motors[i]->setPower(watt);
    }

    return true;
}

bool Movement::Onmidirectional_3Wheels::rotate(uint8_t deegres) {
    // Not implemented yet.
    return false;
}

bool Movement::Onmidirectional_3Wheels::stop() {
    for (size_t i = 0; i < 3; i++)
        motors[i]->setPower(0);

    return true;
}

Event::Movement::Movement(::Movement::Base *mov, double fx, double fy) : mov(mov), fx(fx), fy(fy) {}
Event::Movement::~Movement() {}
void Event::Movement::operator()() {
    mov->applySpeed(fx, fy);
    delete this;
}