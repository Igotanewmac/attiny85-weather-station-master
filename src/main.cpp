#include <Arduino.h>

// wire master library for the USI hardware
#include <TinyWireM.h>



// TCA9548 Library for i2c bus switching
#include "TCA9548.h"



// ds3231 library for clock functions
#include "DS3231.h"






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













// function declarations
void i2cswitchbus( uint8_t newbusid );
void i2cbuson();
void i2cbusoff();




















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

  // now turn off the i2c bus
  i2cbusoff();


  // now write the LED pin low
  digitalWrite( LED_RED , LOW );
  digitalWrite( LED_YEL , LOW );

  // all done, return.
  return;

}



























//