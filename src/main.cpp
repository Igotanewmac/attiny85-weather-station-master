#include <Arduino.h>

// wire master library for the USI hardware
#include <TinyWireM.h>

// TCA9548 Library for i2c bus switching
#include "TCA9548.h"

// ds3231 library for clock functions
#include "DS3231.h"

// bh1759 luminance sensor
#include "BH1750.h"

// htu2x temperature and humidity sensor
#include "HTU2X.h"

// bmp0855 pressure sensor
#include "tinyBMP085.h"
tinyBMP085 mybmp085;


// fram utility routines
#include "I2CFram.h"






// Utility definition for TinyWireM
#define wire TinyWireM

// the actual bus id's
// sensors are on bus 0x00
#define I2CBUSIDSENSORS 0x00
// eeprom storage is on bus 0x00
#define I2CBUSIDSTORAGE 0x01

// led pin(s)!
// Pin of red output LED
#define LED_RED 3
// Pin of yellow output LED
#define LED_YEL 4

// i2c side io pin
// input pin for now.
#define PIN_TRIGGER1 1




// globals

// global 32 byte data ram cache
uint8_t globalcache[32] = { 0 };

// global 32bit variable
uint32_t myuint32 = 0;

// global 16 bit variable
uint16_t myuint16 = 0;








// function definitions




/// @brief Read the sensor data, prepare it, then store it to fram.
/// @return returns the datamemorycursor.
uint16_t doSensorReadAndStore();











/// @brief Setup code run at power on
void setup() {
  // put your setup code here, to run once:

  // set up outputs
  pinMode( LED_RED , OUTPUT );
  pinMode( LED_YEL , OUTPUT );
  digitalWrite( LED_RED , LOW );
  digitalWrite( LED_YEL , LOW );

  // an input trigger
  // turn off the high on the pin first
  digitalWrite( PIN_TRIGGER1 , LOW );
  pinMode ( PIN_TRIGGER1 , INPUT_PULLUP );
  
  // that's actually it for now.

}















/// @brief Main loop, called repeatedly by main.
void loop() {
  // put your main code here, to run repeatedly:

  // if the trigger pin is anything but LOW, return super early.
  if ( digitalRead( PIN_TRIGGER1 ) != LOW ) { return; }

  // the alarm has triggered, our pin went low!
  // turn on the red led
  digitalWrite( LED_RED , HIGH );

  // switch on the i2c bus
  i2cbuson();

  // switch to the sensor bus
  i2cswitchbus( I2CBUSIDSENSORS );

  // turn off the alarm
  clearalarms();

  // now do the actual job...
  doSensorReadAndStore();


  // now turn off the i2c bus
  i2cbusoff();


  // now write the LED pin low
  digitalWrite( LED_RED , LOW );

  // all done, return.
  return;

}











/// @brief Read the sensor data, prepare it, then store it to fram.
/// @return returns the datamemorycursor.
uint16_t doSensorReadAndStore() {

  // read sensors into ram buffer

  // fetch the current time as an epoch from 00-01-01-00-00-00 ( year 2000, first of january , midnight.)
  myuint32 = getTimeAndDate();

  // put this value into the cache
  globalcache[0] = (uint8_t)( ( myuint32 >> 24 ) & 0xFF );
  globalcache[1] = (uint8_t)( ( myuint32 >> 16 ) & 0xFF );
  globalcache[2] = (uint8_t)( ( myuint32 >> 8  ) & 0xFF );
  globalcache[3] = (uint8_t)( ( myuint32       ) & 0xFF );


  // fetch the current luminance value
  myuint16 = bh1750lowresoneshot();

  // put this value into the cache
  globalcache[4] = (uint8_t)( ( myuint16 >> 8  ) & 0xFF );
  globalcache[5] = (uint8_t)( ( myuint16       ) & 0xFF );


  // HTU2X temperature and humidity.
  // the chip requires the order 1. temperature, 2. humidity.
  myuint16 = htu2x_gettemperature();

  // put this value into the cache
  globalcache[6] = (uint8_t)( ( myuint16 >> 8  ) & 0xFF );
  globalcache[7] = (uint8_t)( ( myuint16       ) & 0xFF );

  // get the humidity
  myuint16 = htu2x_gethumidity();

  // put this value into the cache
  globalcache[8] = (uint8_t)( ( myuint16 >> 8  ) & 0xFF );
  globalcache[9] = (uint8_t)( ( myuint16       ) & 0xFF );


  // fetch the current pressure
  myuint32 = mybmp085.readPressure();

  // put this value into the cache
  globalcache[10] = (uint8_t)( ( myuint32 >> 24 ) & 0xFF );
  globalcache[11] = (uint8_t)( ( myuint32 >> 16 ) & 0xFF );
  globalcache[12] = (uint8_t)( ( myuint32 >> 8  ) & 0xFF );
  globalcache[13] = (uint8_t)( ( myuint32       ) & 0xFF );

  // add status bits.
  // has the clock stopped?
  globalcache[14] = clockHasStopped();
  globalcache[15] = 0xA1;


  return framwritesensordata( globalcache );

};
































//