#include <Arduino.h>
#include <BluetoothClient.h>
#include <Logger.h>
#include <Motor.h>
#include <Movement.h>
#include <Ultrasonic.h>

Logger logger;

#define LOG_EXPR(l, x) logger.log(l, '[', #x, "]: ", x)

class Robo {
  public:
    Robo(Movement::Base *mov) : mov(mov) {}

    void setSpeed(float x, float y) {
        speed_base.x = x;
        speed_base.y = y;
        speed.x = x;
        speed.y = y;
    }

    void addSpeed(float x, float y) {
        speed.x = speed_base.x + x;
        speed.y = speed_base.y + y;
    }

    struct Vector;
    const Vector &getSpeed() { return speed; }

    void operator()() { mov->applySpeed(speed.x, speed.y); }

  private:
    Movement::Base *mov;
    struct Vector {
        float x = 0, y = 0;
    } speed_base, speed;
};

void parseEvent(String &message, Robo *robo) {
    String values = message.substring(2);
    size_t separator;
    switch (message[0]) {
    case 'J':
        LOG_EXPR(Logger::LogLevel::DEBUG, 'J');
        separator = values.indexOf(',');
        LOG_EXPR(Logger::LogLevel::DEBUG, values.substring(0, separator).toInt());
        LOG_EXPR(Logger::LogLevel::DEBUG, values.substring(separator + 1).toInt());
        robo->setSpeed(values.substring(0, separator).toInt(), values.substring(separator + 1).toInt());
        break;

    default:
        break;
    }
}

BluetoothClient::BluetoothClient bt_client(10, 11);

void setup() {
    Serial.begin(9600);
    bt_client.begin(9600);
#ifdef __DEBUG
    logger.setLogLevel(Logger::LogLevel::DEBUG);
#else
    logger.setLogLevel(Logger::LogLevel::RELEASE);
#endif
}

void loop() {
    static Motor::ContinuousRotationServo motors[] = {{3, 170}, {5, 175}, {6, 190}};
    static Movement::Onmidirectional_3Wheels movement(&motors[0], &motors[1], &motors[2]);
    static AntiCollisionSystem::TriUltrassonic tri_ultrasonic(2, A3, A4, A5);
    static AntiCollisionSystem::Base *anti_collision_system = &tri_ultrasonic;
    static Robo robo(&movement);

    static const uint16_t angles[] = {30, 150, 270};
    static const uint16_t margin_sensor = 10;

    bt_client();

    if (bt_client.arrivedMessage()) {
        auto message = bt_client.readMessage();
        LOG_EXPR(Logger::LogLevel::DEBUG, message);
        parseEvent(message, &robo);
    }

    auto data = anti_collision_system->read(5);
    logger.log(Logger::LogLevel::DEBUG, "====================");

    if (!data.cm_front)
        data.cm_front = 5000;
    if (!data.cm_left)
        data.cm_left = 5000;
    if (!data.cm_right)
        data.cm_right = 5000;

    if (robo.getSpeed().x || robo.getSpeed().y) {
        if (data.cm_front < data.cm_left) {
            if (data.cm_front < data.cm_right) {
                if (data.cm_front < margin_sensor) {
                    logger.log(Logger::LogLevel::DEBUG, "front");
                    robo.addSpeed(map(data.cm_front, margin_sensor, 0, 50, 200) * cos(radians(angles[0])),
                                  map(data.cm_front, margin_sensor, 0, 50, 200) * sin(radians(angles[0])));
                }
            } else {
                if (data.cm_right < margin_sensor) {
                    logger.log(Logger::LogLevel::DEBUG, "right");
                    robo.addSpeed(map(data.cm_right, margin_sensor, 0, 50, 200) * cos(radians(angles[2])),
                                  map(data.cm_right, margin_sensor, 0, 50, 200) * sin(radians(angles[2])));
                }
            }
        } else {
            if (data.cm_left < data.cm_right) {
                if (data.cm_left < margin_sensor) {
                    logger.log(Logger::LogLevel::DEBUG, "left");
                    robo.addSpeed(map(data.cm_left, margin_sensor, 0, 50, 200) * cos(radians(angles[1])),
                                  map(data.cm_left, margin_sensor, 0, 50, 200) * sin(radians(angles[1])));
                }
            } else {
                if (data.cm_right < margin_sensor) {
                    logger.log(Logger::LogLevel::DEBUG, "right");
                    robo.addSpeed(map(data.cm_right, margin_sensor, 0, 50, 200) * cos(radians(angles[2])),
                                  map(data.cm_right, margin_sensor, 0, 50, 200) * sin(radians(angles[2])));
                }
            }
        }
    }

    // if (!(data.cm_front && data.cm_front < margin_sensor) && !(data.cm_left && data.cm_left < margin_sensor) &&
    //     !(data.cm_right && data.cm_right < margin_sensor)) {
    //     robo.addSpeed(0, 0);
    // } else if (data.cm_front && data.cm_front < margin_sensor && (robo.getSpeed().x || robo.getSpeed().y)) {
    //     logger.log(Logger::LogLevel::DEBUG, "front");
    //     robo.addSpeed(map(data.cm_front, margin_sensor, 0, 50, 200) * cos(radians(angles[0])),
    //                   map(data.cm_front, margin_sensor, 0, 50, 200) * sin(radians(angles[0])));
    // } else if (data.cm_left && data.cm_left < margin_sensor && (robo.getSpeed().x || robo.getSpeed().y)) {
    //     logger.log(Logger::LogLevel::DEBUG, "left");
    //     robo.addSpeed(map(data.cm_left, margin_sensor, 0, 50, 200) * cos(radians(angles[1])),
    //                   map(data.cm_left, margin_sensor, 0, 50, 200) * sin(radians(angles[1])));
    // } else if (data.cm_right && data.cm_right < margin_sensor && (robo.getSpeed().x || robo.getSpeed().y)) {
    //     logger.log(Logger::LogLevel::DEBUG, "right");
    //     robo.addSpeed(map(data.cm_right, margin_sensor, 0, 50, 200) * cos(radians(angles[2])),
    //                   map(data.cm_right, margin_sensor, 0, 50, 200) * sin(radians(angles[2])));
    // }

    LOG_EXPR(Logger::LogLevel::DEBUG, data.cm_front);
    LOG_EXPR(Logger::LogLevel::DEBUG, data.cm_left);
    LOG_EXPR(Logger::LogLevel::DEBUG, data.cm_right);
    logger.log(Logger::LogLevel::DEBUG, "--------------------");
    LOG_EXPR(Logger::LogLevel::DEBUG, robo.getSpeed().x);
    LOG_EXPR(Logger::LogLevel::DEBUG, robo.getSpeed().y);
    logger.log(Logger::LogLevel::DEBUG, "====================");

    robo();
}
