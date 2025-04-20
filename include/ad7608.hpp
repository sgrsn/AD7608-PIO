#ifndef AD7608_HPP
#define AD7608_HPP

#include <Arduino.h>
#include <SPI.h>
#include <sstream>

class AD7608
{
private:
  const uint8_t _cs;
  const uint8_t _busy;
  const uint8_t _convst;
  uint8_t _buf[18] = {};
  int64_t _data[8] = {};
  double _voltage[8] = {};
  bool _is_reading = false;

  static AD7608 *_instance;

  static void _busyFallingISR()
  {
    if (_instance)
    {
      _instance->readData();
    }
  }

public:
  AD7608(uint8_t cs, uint8_t busy, uint8_t convst) : _cs(cs),
                                                     _busy(busy),
                                                     _convst(convst)
  {
    _instance = this;
  }

  void begin();
  void end();
  bool isReady();
  void startConversion();
  void readData();
  void processData();
  void printData();
  
  // Getter methods
  double getVoltage(uint8_t channel);
  double *getAllVoltages();
  int64_t getRawData(uint8_t channel);
};

#endif // AD7608_HPP