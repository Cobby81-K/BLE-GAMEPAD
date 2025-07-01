
#include <BLEDevice.h>
#include "BleGamepad.h"

BleGamepad::BleGamepad(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel)
  : _deviceName(deviceName), _deviceManufacturer(deviceManufacturer), _batteryLevel(batteryLevel),
    _hat(0), _x(0), _y(0), _z(0), _rZ(0), _rX(0), _rY(0), _slider1(0), _slider2(0),
    _autoReport(true), _connected(false) {
  memset(_buttons, 0, sizeof(_buttons));
}

void BleGamepad::begin() {
  BLEDevice::init(_deviceName.c_str());
  BLEServer *pServer = BLEDevice::createServer();
  connectionStatus = new BleConnectionStatus();
  pServer->setCallbacks(connectionStatus);

  hid = new BLEHIDDevice(pServer);
  inputGamepad = hid->inputReport(1);
  outputReport = hid->outputReport(1);
  hid->manufacturer()->setValue(String(_deviceManufacturer.c_str()));

  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x01);

  const uint8_t reportMap[] = {0x05,0x01,0x09,0x05,0xA1,0x01,0x85,0x01,0x05,0x09,0x19,0x01,0x29,0x10,0x15,0x00,
    0x25,0x01,0x95,0x10,0x75,0x01,0x81,0x02,0x05,0x01,0x09,0x39,0x15,0x00,0x25,0x07,0x35,0x00,
    0x46,0x3B,0x01,0x65,0x14,0x75,0x04,0x95,0x01,0x81,0x42,0x75,0x04,0x95,0x01,0x81,0x01,0x09,
    0x30,0x09,0x31,0x09,0x32,0x09,0x35,0x15,0x81,0x25,0x7F,0x75,0x08,0x95,0x04,0x81,0x02,0xC0};
  hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
  hid->startServices();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_GAMEPAD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->start();
}

void BleGamepad::setAxes(int16_t x, int16_t y, int16_t z, int16_t rZ, int16_t rX, int16_t rY, int16_t slider1, int16_t slider2) {
  _x = x; _y = y; _z = z; _rZ = rZ; _rX = rX; _rY = rY; _slider1 = slider1; _slider2 = slider2;
  if (_autoReport) sendReport();
}
void BleGamepad::setHat(uint8_t hat) { _hat = hat; if (_autoReport) sendReport(); }
void BleGamepad::press(uint16_t b) { uint8_t i = (b - 1) / 8, m = (b - 1) % 8; _buttons[i] |= (1 << m); if (_autoReport) sendReport(); }
void BleGamepad::release(uint16_t b) { uint8_t i = (b - 1) / 8, m = (b - 1) % 8; _buttons[i] &= ~(1 << m); if (_autoReport) sendReport(); }
void BleGamepad::setButton(uint8_t b, boolean p) { if (p) press(b); else release(b); }
void BleGamepad::releaseAll() { _buttons[0] = _buttons[1] = 0; if (_autoReport) sendReport(); }
bool BleGamepad::isConnected() { return connectionStatus->connected; }
void BleGamepad::sendReport() {
  if (!isConnected()) return;
  uint8_t report[8] = {_buttons[0], _buttons[1], _hat, (uint8_t)_x, (uint8_t)_y, (uint8_t)_z, (uint8_t)_rZ, 0};
  inputGamepad->setValue(report, sizeof(report));
  inputGamepad->notify();
}
