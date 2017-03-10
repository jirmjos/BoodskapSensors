#include <Arduino.h>
#include <BoodskapMessage.h>
#include <BoodskapConfig.h>

#ifndef _BOODSKAP_COMMUNICATOR_H
#define _BOODSKAP_COMMUNICATOR_H

#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
//#define DEBUG_PRINT(x)
//#define DEBUG_PRINTLN(x)

class BoodskapCommunicator
{

  public:

    BoodskapCommunicator(void (*callback)(BoodskapMessage* command));

    void wifiSSID(const char* ssid);

    void wifiPassword(const char* password);

    void host(const char* host);

    void port(uint16_t port);

    void localPort(uint16_t localPort);

    void domainKey(const char* domainKey);

    void apiKey(const char* apiKey);

    void assetId(const char* assetId);

    void deviceModel(const char* deviceModel);

    void firmwareVersion(const char* firmwareVersion);

    void heartbeat(uint32_t heartbeat);

    void cleanSession(bool cleanSession);

    void setup();

    void loop();

    void ping();

    void sendAck(uint32_t cmdId, uint32_t corrId, bool ack);

    void create(int messageId, BoodskapMessage* msg);

    virtual void send(BoodskapMessage* message) = 0;

    virtual bool saveSystemConfig(JsonObject& config) = 0;

    virtual bool saveDeviceConfig(JsonObject& config) = 0;

  protected:

    virtual void begin() = 0;

    virtual bool isConnected() = 0;

    virtual void process() = 0;

    virtual void commandReceived(BoodskapMessage* command) = 0;

    void messageReceived(BoodskapMessage* command);

  protected:

    char _ssid[SSID_LEN];
    char _password[SSID_PSWD_LEN];
    char _host[BOODSKAP_HOST_LEN];
    uint16_t _port;
    uint16_t _localPort;
    char _domainKey[DOMAIN_KEY_LEN];
    char _apiKey[API_KEY_LEN];
    char _assetId[ASSET_ID_LEN];
    char _deviceModel[DEVICE_MODEL_LEN];
    char _firmwareVersion[FIRMWARE_VER_LEN];
    uint32_t _heartbeat;
    char _packetBuffer[PACKET_BUFFER_LEN];
    uint32_t _lastHeartbeat;

  private:
    void (*_callback)(BoodskapMessage* command);

};

#endif //_BOODSKAP_COMMUNICATOR_H
