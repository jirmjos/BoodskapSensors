#ifndef _THREE_PHASE_VOLTAGE_SENSOR_H
#define _THREE_PHASE_VOLTAGE_SENSOR_H

#include <Arduino.h>
#include <SensorModule.h>
#include <BoodskapConfig.h>

class ThreePhaseVoltageSensor : public SensorModule{

  public:

  ThreePhaseVoltageSensor(uint8_t p1, uint8_t p2, uint8_t p3, void (*callback)(bool p1, bool p2, bool p3));

  virtual void setup();

  virtual void loop();

  void voltage(bool* p1, bool* p2, bool* p3);

  private:

  void (*_callback)(bool p1, bool p2, bool p3);

  bool readVoltage(uint8_t pin);

  bool _oP1;
  bool _oP2;
  bool _oP3;
  uint8_t _p1;
  uint8_t _p2;
  uint8_t _p3;
  uint32_t _lastProcessed;
};

#endif //_THREE_PHASE_VOLTAGE_SENSOR_H
