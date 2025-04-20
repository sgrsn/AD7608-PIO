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

  void begin()
  {
    // Initialize SPI
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // Configure pins
    pinMode(_cs, OUTPUT);
    pinMode(_busy, INPUT);
    pinMode(_convst, OUTPUT);
    digitalWrite(_cs, LOW);
    digitalWrite(_convst, LOW);

    // Set up interrupt for BUSY pin
    attachInterrupt(digitalPinToInterrupt(_busy), _busyFallingISR, FALLING);
  }

  void end()
  {
    detachInterrupt(digitalPinToInterrupt(_busy));
    SPI.endTransaction();
  }

  bool isReady()
  {
    return !_is_reading;
  }

  void startConversion()
  {
    if (_is_reading)
      return;

    _is_reading = true;
    digitalWrite(_convst, HIGH);
    delayMicroseconds(1);
    digitalWrite(_convst, LOW);
  }

  void readData()
  {
    _is_reading = true;
    digitalWrite(_cs, HIGH);
    delayMicroseconds(1);
    digitalWrite(_cs, LOW);

    // Read 18 bytes (8 channels × 18 bits per channel, packed)
    for (int i = 0; i < 18; i++)
    {
      _buf[i] = SPI.transfer(0x00);
    }

    // Reset data array
    for (int i = 0; i < 8; i++)
    {
      _data[i] = 0;
    }

    // Unpack data (MSB format)
    _data[0] = (_buf[0] << 10) | (_buf[1] << 2) | (_buf[2] >> 6);
    _data[1] = ((_buf[2] & 0x3F) << 12) | (_buf[3] << 4) | (_buf[4] >> 4);
    _data[2] = ((_buf[4] & 0x0F) << 14) | (_buf[5] << 6) | (_buf[6] >> 2);
    _data[3] = ((_buf[6] & 0x03) << 16) | (_buf[7] << 8) | _buf[8];
    _data[4] = (_buf[9] << 10) | (_buf[10] << 2) | (_buf[11] >> 6);
    _data[5] = ((_buf[11] & 0x3F) << 12) | (_buf[12] << 4) | (_buf[13] >> 4);
    _data[6] = ((_buf[13] & 0x0F) << 14) | (_buf[14] << 6) | (_buf[15] >> 2);
    _data[7] = ((_buf[15] & 0x03) << 16) | (_buf[16] << 8) | _buf[17];

    _is_reading = false;
  }

  void processData()
  {
    // Convert to voltage
    for (int i = 0; i < 8; i++)
    {
      // Handle signed 18-bit conversion (2's complement)
      if (_data[i] & 0x20000)
      {
        _data[i] = -((~_data[i] & 0x3FFFF) + 1);
      }
      _voltage[i] = (double)_data[i] * 2.5 / 0xFFFF;
    }
  }

  void printData()
  {
    std::stringstream ss;
    ss << "v1:" << _voltage[0] << ", " << "v2:" << _voltage[1] << ", "
       << "v3:" << _voltage[2] << ", " << "v4:" << _voltage[3] << ", "
       << "v5:" << _voltage[4] << ", " << "v6:" << _voltage[5] << ", "
       << "v7:" << _voltage[6] << ", " << "v8:" << _voltage[7];
    Serial.println(ss.str().c_str());
  }

  // Getter methods
  double getVoltage(uint8_t channel)
  {
    if (channel >= 0 && channel < 8)
    {
      return _voltage[channel];
    }
    return 0.0;
  }

  double *getAllVoltages()
  {
    return _voltage;
  }

  int64_t getRawData(uint8_t channel)
  {
    if (channel >= 0 && channel < 8)
    {
      return _data[channel];
    }
    return 0;
  }
};

// Initialize static instance pointer
AD7608 *AD7608::_instance = nullptr;