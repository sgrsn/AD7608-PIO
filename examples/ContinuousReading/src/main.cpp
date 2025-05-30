/**
 * AD7608 Continuous Reading Example
 * 
 * This example demonstrates how to continuously read data from
 * the AD7608 ADC at a high sampling rate.
 * 
 * Hardware Connections:
 * - CS_PIN: Chip Select pin connected to the AD7608
 * - BUSY_PIN: BUSY pin from the AD7608
 * - CONVST_PIN: CONVST pin connected to the AD7608
 */

#include <Arduino.h>
#include <SPI.h>
#include <ad7608.hpp>

const uint8_t CS_PIN = 41;
const uint8_t BUSY_PIN = 40;
const uint8_t CONVST_PIN = 25;

AD7608 adc(CS_PIN, BUSY_PIN, CONVST_PIN);
unsigned long lastSampleTime = 0;
unsigned long lastPrintTime = 0;

void setup() {
  // Set high-speed serial
  Serial.begin(1000000);
  
  // Initialize ADC
  adc.begin();
  
  Serial.println("AD7608 Read Test Start");
}

void loop() {
  unsigned long currentTime = micros();
  
  // Start a new conversion every 2000 microseconds
  if (currentTime - lastSampleTime >= 2000 && adc.isReady()) {
    adc.startConversion();
    lastSampleTime = currentTime;
  }
  
  // Process and print data every 10 milliseconds
  if (millis() - lastPrintTime >= 10) {
    adc.processData();
    adc.printData();
    lastPrintTime = millis();
  }
}