# AD7608 Arduino Library

A PlatformIO library for interfacing with the AD7608 8-Channel Simultaneous Sampling ADC from Analog Devices.

## Features

- Supports all 8 channels of the AD7608
- Interrupt-driven data acquisition
- Configurable SPI interface
- Conversion from raw ADC values to voltage
- Simple API for reading and processing data

## Hardware

The AD7608 is an 8-channel, simultaneous sampling, 18-bit, analog-to-digital converter (ADC) with a maximum sampling rate of 200 kSPS.

### Connections

| AD7608 Pin | Arduino/Microcontroller Pin |
|------------|------------------------------|
| CS         | User defined (e.g., pin 41)  |
| BUSY       | User defined (e.g., pin 40)  |
| CONVST     | User defined (e.g., pin 25)  |
| SCLK       | SCK pin of SPI interface     |
| DOUT       | MISO pin of SPI interface    |
| DIN        | MOSI pin of SPI interface    |
| VDD        | 3.3V or 5V (check datasheet) |
| GND        | GND                          |

## Installation

### Using PlatformIO Library Manager

1. Open PlatformIO IDE
2. Go to PlatformIO Home > Libraries
3. Search for "AD7608"
4. Click "Install"

### Manual Installation

1. Download this repository
2. Extract it to your PlatformIO project's `lib` directory
3. Include the library in your code with `#include <ad7608.hpp>`

## Usage

```cpp
#include <Arduino.h>
#include <SPI.h>
#include <ad7608.hpp>

// Pin definitions
const uint8_t CS_PIN = 41;
const uint8_t BUSY_PIN = 40;
const uint8_t CONVST_PIN = 25;

// Create AD7608 instance
AD7608 adc(CS_PIN, BUSY_PIN, CONVST_PIN);

void setup() {
  Serial.begin(115200);
  
  // Initialize the AD7608
  adc.begin();
}

void loop() {
  // Start a conversion if the ADC is ready
  if (adc.isReady()) {
    adc.startConversion();
    
    // Process the raw data into voltage values
    adc.processData();
    
    // Print all channel values
    for (int i = 0; i < 8; i++) {
      Serial.print("Channel ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(adc.getVoltage(i), 6);
      Serial.println(" V");
    }
    
    delay(100); // Wait before next reading
  }
}
```

## API Reference

### Constructor

```cpp
AD7608(uint8_t cs, uint8_t busy, uint8_t convst)
```

Creates an AD7608 object with the specified pins.

- `cs`: Chip Select pin
- `busy`: BUSY pin from the AD7608
- `convst`: CONVST pin to the AD7608

### Methods

- `void begin()`: Initialize the AD7608 and SPI interface
- `void end()`: Clean up resources
- `bool isReady()`: Check if the ADC is ready for a new conversion
- `void startConversion()`: Start a new conversion
- `void readData()`: Read data from the ADC (called automatically by interrupt)
- `void processData()`: Process raw data into voltage values
- `void printData()`: Print all channel values to Serial
- `double getVoltage(uint8_t channel)`: Get voltage for a specific channel (0-7)
- `double* getAllVoltages()`: Get pointer to array of all voltage values
- `int64_t getRawData(uint8_t channel)`: Get raw ADC value for a specific channel (0-7)

## License

This library is released under the MIT License.

## Credits

Developed by [sgrsn](https://github.com/sgrsn)