#include "BoodskapMessage.h"
#include <Arduino.h>

BoodskapMessage::BoodskapMessage(long messageId, const char* domainKey, const char* assetId, const char* apiKey, const char* fwVersion, const char* devModel):_msg(0), _header(0), _data(0)
{


  _msg = &_buffer.createObject();
  _header = &_msg->createNestedObject("header");
  _data = &_msg->createNestedObject("data");


  (*_header)["key"] = domainKey;
  (*_header)["mid"] = messageId;
  (*_header)["aid"]   = assetId;
  (*_header)["api"]  = apiKey;
  (*_header)["dmdl"]  = devModel;
  (*_header)["fwver"]  = fwVersion;

}

BoodskapMessage::BoodskapMessage(): BoodskapMessage(1, "", "", "", "", "")
{
}

BoodskapMessage* BoodskapMessage::withDomainKey(const char* domainKey) {
  (*_header)["key"] = domainKey;
  return this;
}

BoodskapMessage* BoodskapMessage::withApiKey(const char* apiKey){
  (*_header)["api"]  = apiKey;
  return this;
}

BoodskapMessage* BoodskapMessage::withAssetId(const char* assetId){
  (*_header)["aid"]   = assetId;
  return this;
}

BoodskapMessage* BoodskapMessage::withFirmwareVersion(const char* fwVersion){
  (*_header)["fwver"]  = fwVersion;
  return this;
}

BoodskapMessage* BoodskapMessage::withDeviceModel(const char* devModel){
  (*_header)["dmdl"]  = devModel;
  return this;
}

BoodskapMessage* BoodskapMessage::withMessageId(const long messageId){
  (*_header)["mid"] = messageId;
  return this;
}

bool BoodskapMessage::parse(BoodskapMessage& msg, const char* json) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject* root = &jsonBuffer.parseObject(json);

  if (!root->success()) {
    return false;
  }

  if (!root->containsKey("header")) return false;
  if (!root->containsKey("data")) return false;

  JsonObject& h = (*root)["header"];
  JsonObject& d = (*root)["data"];

  if (!h.containsKey("key")) return false;
  if (!h.containsKey("mid")) return false;
  if (!h.containsKey("aid")) return false;
  if (!h.containsKey("api")) return false;
  if (!h.containsKey("cmdid")) return false;
  if (!h.containsKey("corrid")) return false;

  msg.header()["key"] = h["key"];
  msg.header()["mid"] = h["mid"];
  msg.header()["aid"] = h["aid"];
  msg.header()["api"] = h["api"];
  msg.header()["cmdid"] = h["cmdid"];
  msg.header()["corrid"] = h["corrid"];

  for (auto dataobj : d) {
    msg.data()[dataobj.key] = d[dataobj.key];
  }

  return true;
}

JsonObject& BoodskapMessage::get() {
  return *_msg;
}

JsonObject& BoodskapMessage::header() {
  return *_header;
}

JsonObject& BoodskapMessage::data() {
  return *_data;
};

int BoodskapMessage::id() {
  return _header->get<int>("mid");
}

uint32_t BoodskapMessage::command() {
  return _header->get<uint32_t>("cmdid");
}

uint32_t BoodskapMessage::correlationId() {
  return _header->get<uint32_t>("corrid");
}

JsonObject& BoodskapMessage::set(const char* field, uint8_t val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, bool val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, int val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, uint16_t val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, uint32_t val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, float val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, double val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, long val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, const char* val) {
  (*_data)[field]  = val;
  return data();
}

JsonObject& BoodskapMessage::set(const char* field, String val) {
  (*_data)[field]  = val;
  return data();
}

String BoodskapMessage::urlEncode(const char* msg)
{
  const char *hex = "0123456789abcdef";
  String encodedMsg = "";

  while (*msg != '\0') {
    if ( ('a' <= *msg && *msg <= 'z')
         || ('A' <= *msg && *msg <= 'Z')
         || ('0' <= *msg && *msg <= '9') ) {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[*msg >> 4];
      encodedMsg += hex[*msg & 15];
    }
    msg++;
  }
  return encodedMsg;
}
