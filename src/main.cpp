#include <Arduino.h>
#include <BluetoothClient.h>
#include <Event.h>
#include <Logger.h>
#include <Motor.h>
#include <Movement.h>
#include <Ultrasonic.h>

Logger logger;

#define LOG_EXPR(l, x) logger.log(l, '[', #x, "]: ", x)

#define trigPin 2
#define echoPin A3

Ultrasonic::Sensor sensor(trigPin, A3, A4, A5);
void setup() {
    Serial.begin(9600);
    logger.setLogLevel(Logger::LogLevel::DEBUG);
}

void loop() {
    static uint32_t duration, distance;

    auto result = sensor.read();

    LOG_EXPR(Logger::LogLevel::DEBUG, result.cm_front);
    LOG_EXPR(Logger::LogLevel::DEBUG, result.cm_left);
    LOG_EXPR(Logger::LogLevel::DEBUG, result.cm_right);
    logger.log(Logger::LogLevel::DEBUG, "=============================");

    delay(50);
}
