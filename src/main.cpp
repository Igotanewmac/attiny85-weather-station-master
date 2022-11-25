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



#define GETBIT00 ( flags & 0b0000000000000001 )
#define GETBIT01 ( flags & 0b0000000000000010 ) >> 1
#define GETBIT02 ( flags & 0b0000000000000100 ) >> 2
#define GETBIT03 ( flags & 0b0000000000001000 ) >> 4
#define GETBIT04 ( flags & 0b0000000000010000 ) >> 5
#define GETBIT05 ( flags & 0b0000000000100000 ) >> 6
#define GETBIT06 ( flags & 0b0000000001000000 ) >> 7
#define GETBIT07 ( flags & 0b0000000010000000 ) >> 8
#define GETBIT08 ( flags & 0b0000000100000000 ) >> 9
#define GETBIT09 ( flags & 0b0000001000000000 ) >> 10
#define GETBIT10 ( flags & 0b0000010000000000 ) >> 11
#define GETBIT11 ( flags & 0b0000100000000000 ) >> 12
#define GETBIT12 ( flags & 0b0001000000000000 ) >> 13
#define GETBIT13 ( flags & 0b0010000000000000 ) >> 14
#define GETBIT14 ( flags & 0b0100000000000000 ) >> 15
#define GETBIT15 ( flags & 0b1000000000000000 ) >> 16

#define SETBIT00 flags |= 0b0000000000000001;
#define SETBIT01 flags |= 0b0000000000000010;
#define SETBIT02 flags |= 0b0000000000000100;
#define SETBIT03 flags |= 0b0000000000001000;
#define SETBIT04 flags |= 0b0000000000010000;
#define SETBIT05 flags |= 0b0000000000100000;
#define SETBIT06 flags |= 0b0000000001000000;
#define SETBIT07 flags |= 0b0000000010000000;
#define SETBIT08 flags |= 0b0000000100000000;
#define SETBIT09 flags |= 0b0000001000000000;
#define SETBIT10 flags |= 0b0000010000000000;
#define SETBIT11 flags |= 0b0000100000000000;
#define SETBIT12 flags |= 0b0001000000000000;
#define SETBIT13 flags |= 0b0010000000000000;
#define SETBIT14 flags |= 0b0100000000000000;
#define SETBIT15 flags |= 0b1000000000000000;


#define CLRBIT00 flags &= 0b1111111111111110;
#define CLRBIT01 flags &= 0b1111111111111101;
#define CLRBIT02 flags &= 0b1111111111111011;
#define CLRBIT03 flags &= 0b1111111111110111;
#define CLRBIT04 flags &= 0b1111111111101111;
#define CLRBIT05 flags &= 0b1111111111011111;
#define CLRBIT06 flags &= 0b1111111110111111;
#define CLRBIT07 flags &= 0b1111111101111111;
#define CLRBIT08 flags &= 0b1111111011111111;
#define CLRBIT09 flags &= 0b1111110111111111;
#define CLRBIT10 flags &= 0b1111101111111111;
#define CLRBIT11 flags &= 0b1111011111111111;
#define CLRBIT12 flags &= 0b1110111111111111;
#define CLRBIT13 flags &= 0b1101111111111111;
#define CLRBIT14 flags &= 0b1011111111111111;
#define CLRBIT15 flags &= 0b0111111111111111;




/// @brief Read the sensor data, prepare it, then store it to fram.
void dosensorread() {

  // read sensors into globalcache
  for ( int i = 0 ; i < 16 ; i++ ) {
    globalcache[i] = 0;
  }

  // fetch the current time. 6 bytes
  gettimeaddateraw( globalcache );


  // use a 16 bit variable as flags for scheduling
  uint16_t flags = 0;

  // fetch the current limunance value. 2 bytes
  
  // has luminance sensor started?
  if ( GETBIT00 == 0 ) {
    // start bh1750 luminance sensor up
    // set BIT00 to 1;
  }

  // has luminance sensor finished?
  if ( GETBIT01 == 0 ) {
    // poll for sensor data
    // if it has arrived, set BIT01 to 1;
  }






  
  // fetch the current temperature and humidity.  2x2 bytes

  // detch the current temperature and pressure. 2x2 bytes



  // all done, now return
  return;

};



















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
  dosensorread();

  // now turn off the i2c bus
  i2cbusoff();

  // now write the LED pin low
  digitalWrite( LED_RED , LOW );




  
  // all done, return.
  return;

}








































//