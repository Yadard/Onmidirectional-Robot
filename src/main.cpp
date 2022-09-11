#include <Arduino.h>
#include <MatrixMath.h>
#include <Servo.h>
#include <SoftwareSerial.h>

mtx_type A[3][3] = {{cos(radians(240)), cos(radians(120)), cos(0)}, {sin(radians(240)), sin(radians(120)), sin(0)}, {1, 1, 1}};

#define rxPin 10
#define txPin 11

#define STOP_MOTORA 1350
#define STOP_MOTORB 1510
#define STOP_MOTORC 1440

Servo servo[3];
uint16_t servo_values[] = {1350, 1510, 1440};
constexpr uint16_t stop_motor[] = {1350, 1510, 1440};
// Set up a new SoftwareSerial object
SoftwareSerial btSerial = SoftwareSerial(rxPin, txPin);

void setup() {
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);

    btSerial.begin(9600);
    Serial.begin(9600);
    servo[0].attach(3); // Stopping = 1350
    servo[1].attach(6); // Stopping = 1510
    servo[2].attach(5); // Stopping = 1440
    Matrix.Invert((mtx_type *)A, 3);
    Matrix.Print((mtx_type *)A, 3, 3, "A");
}

void loop() {
    bool received = false;
    bool numeric = false;
    static char cmd[6] = {0};
    if (btSerial.available()) {
        delay(2);
        btSerial.readBytes(cmd, 2);
        if (cmd[0] == 'J') {
            cmd[2] = btSerial.read();
            Serial.print("[btSerial]: ");
            Serial.print(cmd[0]);
            Serial.print((uint8_t)cmd[1]);
            Serial.print('|');
            Serial.println((uint8_t)cmd[2]);
        } else if (cmd[0] == 'A' || cmd[0] == 'B' || cmd[0] == 'C') {
            if (isDigit(cmd[1])) {
                Serial.println("[btSerial]: numeric");
                int ch = '0';
                size_t i = 2;
                while (isDigit(ch)) {
                    int ch = btSerial.peek();
                    Serial.print("[btSerial]: i = ");
                    Serial.print(i);
                    Serial.print(" | ch = ");
                    Serial.println((char)ch);
                    if (isDigit(ch)) {
                        cmd[i++] = btSerial.read();
                    } else {
                        cmd[i] = '\0';
                        break;
                    }
                }
                numeric = true;
            } else {
                Serial.println("[btSerial]: comamnd");
                cmd[2] = '\0';
            }
            Serial.print("[btSerial]: ");
            Serial.println(cmd);
        }
        received = true;
    }
    if (received) {
        int sx = 0;
        int sy = 0;
        int sw = 0;
        int speedA = 0;
        int speedB = 0;
        int speedC = 0;
        int valor;
        switch (cmd[0]) {
        case 'A':
        case 'B':
        case 'C':
            if (numeric) {
                valor = String(cmd + 1).toInt();
                Serial.print("[SERVO ");
                Serial.print(cmd[0]);
                Serial.print("]: valor = ");
                Serial.println(valor);
                servo_values[cmd[0] - 'A'] = valor;
            } else {
                Serial.print("[SERVO ");
                Serial.print(cmd[0]);
                Serial.print("]: ");
                Serial.println(cmd[1]);
                switch (cmd[1]) {
                case '<':
                    servo_values[cmd[0] - 'A'] = stop_motor[cmd[0] - 'A'] - 800;
                    break;
                case '>':
                    servo_values[cmd[0] - 'A'] = stop_motor[cmd[0] - 'A'] + 500;
                    break;
                case '.':
                    servo_values[cmd[0] - 'A'] = stop_motor[cmd[0] - 'A'];
                    break;

                default:
                    break;
                }
                Serial.print("[SERVO ");
                Serial.print(cmd[0]);
                Serial.print("]: ");
                Serial.print("index = ");
                Serial.print(cmd[0] - 'A');
                Serial.print(" | value = ");
                Serial.println(servo_values[cmd[0] - 'A']);
            }
            break;

        case 'J':
            sx = cmd[1];
            sy = cmd[2];

            Serial.print("[JOY]: sx = ");
            Serial.print(sx);
            Serial.print("| sy = ");
            Serial.println(sy);

            speedA = sx * A[0][0] + sy * A[0][1] + sw * A[0][2];
            speedB = sx * A[1][0] + sy * A[1][1] + sw * A[1][2];
            speedC = sx * A[2][0] + sy * A[2][1] + sw * A[2][2];

            Serial.print("[JOY]: speedA = ");
            Serial.print(speedA);
            Serial.print("| SpeedB = ");
            Serial.print(speedB);
            Serial.print("| SpeedC = ");
            Serial.println(speedC);

            servo_values[0] = speedA ? map(speedA, -100, 100, 1000, 2000) : STOP_MOTORA;
            servo_values[1] = speedB ? map(speedB, -100, 100, 1000, 2000) : STOP_MOTORB;
            servo_values[2] = speedC ? map(speedC, -100, 100, 1000, 2000) : STOP_MOTORC;

            Serial.print("[JOY]: servo_values[0] = ");
            Serial.print(servo_values[0]);
            Serial.print("| servo_values[1] = ");
            Serial.print(servo_values[1]);
            Serial.print("| servo_values[2] = ");
            Serial.println(servo_values[2]);
            break;

        default:
            break;
        }
    }

    for (size_t i = 0; i < 3; i++) {
        servo[i].writeMicroseconds(servo_values[i]);
    }
}
