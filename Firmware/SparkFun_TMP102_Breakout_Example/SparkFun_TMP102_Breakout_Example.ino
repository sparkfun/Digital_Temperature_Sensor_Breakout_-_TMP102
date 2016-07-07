/******************************************************************************
TMP102_example.ino
Example for the TMP102 I2C Temperature Sensor
Alex Wende @ SparkFun Electronics
April 29th 2016
~

This sketch configures the TMP102 temperature sensor and prints the
temperature and alert state (both from the physical pin, as well as by
reading from the configuration register.

Resources:
Wire.h (included with Arduino IDE)
TMP102.h

Development environment specifics:
Arduino 1.0+
Hardware Version 13

This code is beerware; if you see me (or any other SparkFun employee) at
the local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.   
******************************************************************************/

#include <Wire.h> // Used to establied serial communication on the I2C bus
#include "TMP102.h" // Used to send and recieve specific information from our sensor

// Connections
// VCC = 3.3V
// GND = GND
// SDA = A4
// SCL = A5
const int ALERT_PIN = A3;

TMP102 sensor0(0x48); // Initialize sensor at I2C address 0x48
// Sensor address can be changed with an external jumper to:
// ADD0 - Address
//  VCC - 0x49
//  SDA - 0x4A
//  SCL - 0x4B

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  pinMode(ALERT_PIN,INPUT);  // Declare alertPin as an input
  Wire.begin();  // Join I2C bus

  // Initialize sensor0 settings
  // These settings are saved in the sensor, even if it loses power
  
  // Set the number of consecutive faults before triggering alarm.
  // 0-3: 0:1 fault, 1:2 faults, 2:4 faults, 3:6 faults.
  sensor0.SetFault(0);  // Trigger alarm immediately
  
  // Set the polarity of the Alarm. (0:Active LOW, 1:Active HIGH).
  sensor0.SetAlertPolarity(1); // Active HIGH
  
  // Set the sensor in Comparator Mode (0) or Interrupt Mode (1).
  sensor0.SetAlertMode(0); // Comparator Mode.
  
  // Set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  sensor0.SetConversionRate(2);
  
  //Set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  sensor0.SetExtendedMode(0);

  //Set T_HIGH, the upper limit to trigger the alert on
  sensor0.SetHighTempF(85.0);  // Set T_HIGH in F
  //sensor0.SetHighTempC(29.4); // Set T_HIGH in C
  
  //Set T_LOW, the lower limit to shut turn off the alert
  sensor0.SetLowTempF(80.0);  // Set T_LOW in F
  //sensor0.setLowTempC(26.67); // Set T_LOW in C
}
 
void loop()
{
  float temperature;
  boolean alertPinState, alertRegisterState;
  
  // Turn sensor on to start temperature measurement.
  // Current consumtion typically ~10uA.
  sensor0.Wakeup();

  // Read temperature data
  temperature = sensor0.ReadTempF();
  //temperature = sensor0.ReadTempC();

  // Check for Alert
  alertPinState = digitalRead(ALERT_PIN); // Read the Alert from pin
  alertRegisterState = sensor0.Alert();   // Read the Alert from register
  
  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.Sleep();

  // Print temperature and alarm state
  Serial.print("Temperature: ");
  Serial.print(temperature);
  
  Serial.print("\tAlert Pin: ");
  Serial.print(alertPinState);
  
  Serial.print("\tAlert Register: ");
  Serial.println(alertRegisterState);
  
  delay(1000);  // Wait 1000ms
}

