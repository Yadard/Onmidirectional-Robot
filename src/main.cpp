#include <Arduino.h>
#include <Motor.h>

#define PRINT(x)                                                                                                                                               \
    Serial.print("[");                                                                                                                                         \
    Serial.print(#x);                                                                                                                                          \
    Serial.print("]: ");                                                                                                                                       \
    Serial.println(x)

void test(Motor::Base *motor) {
    PRINT(motor->maxPower());
    PRINT(Motor::counterClockWise(motor->maxPower()));
    motor->setPower(Motor::counterClockWise(motor->maxPower()));
    delay(1500);
    motor->setPower(Motor::clockWise(motor->maxPower()));
    delay(1500);
    motor->setPower(0);
    delay(1500);
}

void setup() {
    Serial.begin(9600);
    // Servos are different 🤬🤬🤬
    static Motor::ContinuousRotationServo A(3, 155), B(5, 170), C(6);
    test(&A);
    test(&B);
    test(&C);
}

void loop() {}