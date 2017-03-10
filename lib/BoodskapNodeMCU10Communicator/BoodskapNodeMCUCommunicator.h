#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <FS.h>
#include <BoodskapCommunicator.h>
#include <BoodskapConfig.h>

#ifndef _BOODSKAP_NODEMCU_COMMUNICATOR_H
#define _BOODSKAP_NODEMCU_COMMUNICATOR_H

class BoodskapNodeMCUCommunicator : public BoodskapCommunicator
{

  public:

    BoodskapNodeMCUCommunicator(void (*callback)(BoodskapMessage* command));

    virtual bool isConnected();

    virtual void send(BoodskapMessage* message);

  protected:

    virtual void begin();

    virtual void process();

    virtual void commandReceived(BoodskapMessage* command);

    virtual bool saveSystemConfig(JsonObject& config);

    virtual bool saveDeviceConfig(JsonObject& config);

  private:

    bool joinAP(uint32_t timeout);

    bool saveConfig(const char* path, JsonObject& json);

    bool loadConfig();

    void doOTA(BoodskapMessage* command);

  protected:

    WiFiUDP udp;
};

#endif //_BOODSKAP_NODEMCU_COMMUNICATOR_H
