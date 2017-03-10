#ifndef _SENSOR_MODULE_H
#define _SENSOR_MODULE_H

class SensorModule{

public:
  virtual void setup() = 0;
  virtual void loop() = 0;
};

#endif //_SENSOR_MODULE_H
