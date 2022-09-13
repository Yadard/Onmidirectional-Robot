#include <Arduino.h>
#include <BluetoothClient.h>
#include <Event.h>
#include <Logger.h>
#include <Motor.h>
#include <Movement.h>

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
    static Motor::ContinuousRotationServo motors[] = {{3, 160}, {5, 170}, {6, 185}};
    static Movement::Onmidirectional_3Wheels movement(&motors[0], &motors[1], &motors[2]);
    static Event::Base *event;

    bt_client();
    if (bt_client.arrivedMessage()) {
        auto message = bt_client.readMessage();
        LOG_EXPR(Logger::LogLevel::DEBUG, message);
        event = parseEvent(message, &movement, (Motor::Base *)motors);
        if (event)
            event->operator()();
    }
}
