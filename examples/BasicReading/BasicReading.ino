/**
 * AD7608 Basic Reading Example
 * 
 * This example demonstrates how to use the AD7608 library to read
 * data from all 8 channels of the AD7608 ADC.
 * 
 * Hardware Connections:
 * - CS_PIN: Chip Select pin connected to the AD7608
 * - BUSY_PIN: BUSY pin from the AD7608
 * - CONVST_PIN: CONVST pin connected to the AD7608
 * 
 * The AD7608 communicates via SPI, so the standard SPI pins
 * (MOSI, MISO, SCK) must also be connected.
 */

#include <Arduino.h>
#include <SPI.h>
#include <ad7608.hpp>

// Pin definitions - adjust these to match your wiring
const uint8_t CS_PIN = 41;     // Chip Select pin
const uint8_t BUSY_PIN = 40;   // BUSY pin from AD7608
const uint8_t CONVST_PIN = 25; // CONVST pin to AD7608

// Create AD7608 instance
AD7608 adc(CS_PIN, BUSY_PIN, CONVST_PIN);

// Timing variables
unsigned long lastSampleTime = 0;
unsigned long lastPrintTime = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for serial port to connect
  
  // Initialize the AD7608
  adc.begin();
  
  Serial.println("AD7608 Basic Reading Example");
  Serial.println("Reading all 8 channels...");
}

void loop() {
  unsigned long currentTime = micros();
  
  // Start a new conversion every 2000 microseconds (500 Hz)
  if (currentTime - lastSampleTime >= 2000 && adc.isReady()) {
    adc.startConversion();
    lastSampleTime = currentTime;
  }
  
  // Process and print data every 100 milliseconds (10 Hz)
  if (millis() - lastPrintTime >= 100) {
    // Process the raw data into voltage values
    adc.processData();
    
    // Print all channel values
    Serial.println("AD7608 Channel Readings:");
    for (int i = 0; i < 8; i++) {
      Serial.print("Channel ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(adc.getVoltage(i), 6);
      Serial.println(" V");
    }
    Serial.println();
    
    lastPrintTime = millis();
  }
}