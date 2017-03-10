#ifndef _BOODSKAP_MESSAGE_H
#define _BOODSKAP_MESSAGE_H

#include <ArduinoJson.h>
#include <BoodskapConfig.h>

class BoodskapMessage
{

  public:

    BoodskapMessage();

    BoodskapMessage(long messageId, const char* domainKey, const char* assetId, const char* apiKey, const char* fwVersion, const char* devModel);

    JsonObject& get();

    JsonObject& header();

    JsonObject& data();

    int id();

    uint32_t command();

    uint32_t correlationId();

    JsonObject& set(const char* field, uint8_t val);

    JsonObject& set(const char* field, uint16_t val);

    JsonObject& set(const char* field, uint32_t val);

    JsonObject& set(const char* field, bool val);

    JsonObject& set(const char* field, int val);

    JsonObject& set(const char* field, float val);

    JsonObject& set(const char* field, double val);

    JsonObject& set(const char* field, long val);

    JsonObject& set(const char* field, const char* val);

    JsonObject& set(const char* field, String val);

    BoodskapMessage* withDomainKey(const char* domainKey);

    BoodskapMessage* withApiKey(const char* apiKey);

    BoodskapMessage* withAssetId(const char* assetId);

    BoodskapMessage* withFirmwareVersion(const char* fwVersion);

    BoodskapMessage* withDeviceModel(const char* devModel);

    BoodskapMessage* withMessageId(const long messageId);

    static bool parse(BoodskapMessage& msg, const char* json);

    static String urlEncode(const char* msg);

  private:
    DynamicJsonBuffer _buffer;
    JsonObject* _msg;
    JsonObject* _header;
    JsonObject* _data;
};

#endif //_BOODSKAP_MESSAGE_H
