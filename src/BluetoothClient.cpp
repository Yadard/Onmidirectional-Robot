#include <BluetoothClient.h>

String BluetoothClient::BluetoothClient::readMessage() {
    String str((char *)(this->buffer.data));
    buffer.erase_back(str.length());
    --this->arrived;
    return str;
}

void BluetoothClient::BluetoothClient::operator()() {
    if (this->available()) {
        const int16_t data = this->read();
        if (data != -1) {
            if (data == '{') {
                this->reading = true;
            } else if (data == '}') {
                buffer.push_back('\0');
                ++this->arrived;
                this->reading = false;
            } else if (this->reading) {
                buffer.push_back(static_cast<byte>(data));
            }
        }
    }
}

bool BluetoothClient::BluetoothClient::arrivedMessage() { return this->arrived; }
