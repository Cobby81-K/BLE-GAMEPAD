
#ifndef BLE_CONNECTION_STATUS_H
#define BLE_CONNECTION_STATUS_H

#include <BLEServer.h>

class BleConnectionStatus : public BLEServerCallbacks {
public:
    bool connected = false;
    void onConnect(BLEServer* pServer) override { connected = true; }
    void onDisconnect(BLEServer* pServer) override { connected = false; }
};

#endif
