#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#ifndef BLUETOOTH_BUFFER_SIZE
#define BLUETOOTH_BUFFER_SIZE 255
#endif

namespace BluetoothClient {

class Base {
    virtual void operator()() {}
    virtual bool arrivedMessage() { return false; }
    virtual String readMessage() { return ""; }
};

class BluetoothClient : public Base, private SoftwareSerial {
  public:
    using SoftwareSerial::begin;
    using SoftwareSerial::SoftwareSerial;
    ~BluetoothClient() {}
    void operator()();
    bool arrivedMessage();
    String readMessage();

  private:
    struct Buffer {
        byte data[BLUETOOTH_BUFFER_SIZE] = {0};
        size_t used = 0, size = BLUETOOTH_BUFFER_SIZE;

        void push_back(byte data) {
            if (used < size) {
                this->data[used++] = data;
            }
        }

        void erase_back(size_t amount) {
            for (size_t i = ++amount; i < used; i++) {
                data[i - amount] = data[i];
            }
            used -= amount;
        }

        byte front() { return data[used - 1]; }

        void clear() { used = 0; }
    } buffer;
    bool reading = false;
    int arrived = 0;
};
} // namespace BluetoothClient

#endif // BLUETOOTHCLIENT_H