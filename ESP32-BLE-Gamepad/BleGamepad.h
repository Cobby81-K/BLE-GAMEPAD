
#ifndef BLE_GAMEPAD_H
#define BLE_GAMEPAD_H

#include <Arduino.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>
#include <HIDKeyboardTypes.h>
#include <BLE2902.h>
#include "BleConnectionStatus.h"

class BleGamepad {
public:
    BleGamepad(std::string deviceName = "ESP32 Gamepad", std::string deviceManufacturer = "ESP32", uint8_t batteryLevel = 100);
    void begin();
    void end();
    void setAxes(int16_t x, int16_t y, int16_t z, int16_t rZ, int16_t rX = 0, int16_t rY = 0, int16_t slider1 = 0, int16_t slider2 = 0);
    void setHat(uint8_t hat);
    void press(uint16_t b);
    void release(uint16_t b);
    void setButton(uint8_t b, boolean pressed);
    void releaseAll();
    bool isConnected();
    void setBatteryLevel(uint8_t level);
    void setAutoReport(bool autoReport);
    void sendReport();

private:
    BLEHIDDevice* hid;
    BLECharacteristic* inputGamepad;
    BLECharacteristic* outputReport;
    BleConnectionStatus* connectionStatus;
    uint8_t _buttons[2];
    uint8_t _hat;
    int16_t _x, _y, _z, _rZ, _rX, _rY, _slider1, _slider2;
    uint8_t _batteryLevel;
    bool _autoReport;
    bool _connected;
    std::string _deviceName;
    std::string _deviceManufacturer;
};

#endif
