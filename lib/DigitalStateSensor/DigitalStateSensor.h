#ifndef _DIGITAL_STATE_SENSOR
#define _DIGITAL_STATE_SENSOR

#include <Arduino.h>
#include <SensorModule.h>

class DigitalStateSensor : public SensorModule
{

  public:
    DigitalStateSensor(uint8_t pin, uint32_t procDelay, void (*callback)(uint8_t state) );
    virtual void setup();
    virtual void loop();
  private:

    uint8_t _pin;
    const uint32_t _procDelay;
    void (*_callback)(uint8_t state);
    uint8_t _state;
    uint32_t _lastProcessed;

};

#endif //_DIGITAL_STATE_SENSOR
