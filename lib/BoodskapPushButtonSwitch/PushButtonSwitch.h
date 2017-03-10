#ifndef _PUSH_BUTTON_SWITCH_H
#define _PUSH_BUTTON_SWITCH_H

#include <Arduino.h>
#include <BoodskapConfig.h>
#include <SensorModule.h>

class PushButtonSwitch : public SensorModule
{

  public:
    PushButtonSwitch(uint8_t pin, uint32_t pressDelay, void (*callback)(bool on) );
    virtual void setup();
    virtual void loop();
  private:

    uint8_t _pin;
    uint32_t _pressDelay;
    bool _state;
    uint32_t _lastPressed;
    void (*_callback)(bool on);

};

#endif //_PUSH_BUTTON_SWITCH_H
