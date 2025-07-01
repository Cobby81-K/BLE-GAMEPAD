#include <BLEServer.h>

class BleConnectionStatus : public BLEServerCallbacks {
public:
  bool connected = false;

  void onConnect(BLEServer* pServer) override {
    connected = true;
  }

  void onDisconnect(BLEServer* pServer) override {
    connected = false;
  }
};
