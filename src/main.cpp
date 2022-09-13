#include <Arduino.h>
#include <BluetoothClient.h>
#include <Event.h>
#include <Logger.h>
#include <Motor.h>
#include <Movement.h>
#include <Ultrasonic.h>

Logger logger;

#define LOG_EXPR(l, x) logger.log(l, '[', #x, "]: ", x)

Event::Base *parseEvent(String &message, Movement::Base *mov, Motor::Base *motors) {
    String values = message.substring(2);
    size_t separator;
    Motor::ContinuousRotationServo *array;
    switch (message[0]) {
    case 'J':
        LOG_EXPR(Logger::LogLevel::DEBUG, 'J');
        separator = values.indexOf(',');
        LOG_EXPR(Logger::LogLevel::DEBUG, values.substring(0, separator).toInt());
        LOG_EXPR(Logger::LogLevel::DEBUG, values.substring(separator + 1).toInt());
        return new Event::Movement(mov, values.substring(0, separator).toInt(), values.substring(separator + 1).toInt());
        break;

    case 'A':
    case 'B':
    case 'C':
        LOG_EXPR(Logger::LogLevel::DEBUG, values);
        array = static_cast<Motor::ContinuousRotationServo *>(motors);
        return new Event::Motor(&array[message[0] - 'A'], values);
        break;

    default:
        break;
    }
    return nullptr;
}

BluetoothClient::BluetoothClient bt_client(10, 11);

void setup() {
    Serial.begin(9600);
    bt_client.begin(9600);
    logger.setLogLevel(Logger::LogLevel::DEBUG);
}

void loop() {
    static Motor::ContinuousRotationServo motors[] = {{3, 160, true}, {5, 170}, {6, 185}};
    static Movement::Onmidirectional_3Wheels movement(&motors[0], &motors[1], &motors[2]);
    static Event::Base *event;
    static Ultrasonic::Sensor ultrasonic(2, A3, A4, A5);
    static uint32_t timer_start = 0;

    bt_client();
    uint32_t timer_end = millis();
    if (bt_client.arrivedMessage()) {
        timer_start = timer_end;
        auto message = bt_client.readMessage();
        LOG_EXPR(Logger::LogLevel::DEBUG, message);
        event = parseEvent(message, &movement, (Motor::Base *)motors);
        if (event)
            event->operator()();
    } else if (timer_end - timer_start >= 5000) {
        auto data = ultrasonic.read();
        float fx = 0, fy = 100;

        logger.log(Logger::LogLevel::DEBUG, "====================");
        if (data.cm_front && data.cm_front < 10) {
            logger.log(Logger::LogLevel::DEBUG, "front");
            fx += map(data.cm_front, 10, 0, 0, 100) * cos(radians(210));
            fy += map(data.cm_front, 10, 0, 0, 100) * sin(radians(210));
        }
        if (data.cm_left && data.cm_left < 10) {
            logger.log(Logger::LogLevel::DEBUG, "left");
            fx += map(data.cm_left, 10, 0, 0, 100) * cos(radians(150));
            fy += map(data.cm_left, 10, 0, 0, 100) * sin(radians(150));
        }
        if (data.cm_right && data.cm_right < 10) {
            logger.log(Logger::LogLevel::DEBUG, "right");
            fx += map(data.cm_right, 10, 0, 0, 100) * cos(radians(0));
            fy += map(data.cm_right, 10, 0, 0, 100) * sin(radians(0));
        }

        LOG_EXPR(Logger::LogLevel::DEBUG, data.cm_front);
        LOG_EXPR(Logger::LogLevel::DEBUG, data.cm_left);
        LOG_EXPR(Logger::LogLevel::DEBUG, data.cm_right);
        logger.log(Logger::LogLevel::DEBUG, "--------------------");
        LOG_EXPR(Logger::LogLevel::DEBUG, fx);
        LOG_EXPR(Logger::LogLevel::DEBUG, fy);
        logger.log(Logger::LogLevel::DEBUG, "====================");

        if (fx && fy)
            movement.applySpeed(fx, fy);
    }
}
