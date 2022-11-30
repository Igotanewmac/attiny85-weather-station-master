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
//#include "tinyBMP085.h"
//tinyBMP085 mybmp085;

#include "BMP085.h"

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
uint8_t globalcache[64] = { 0 };

// global 32bit variable
uint32_t myuint32 = 0;

// global 16 bit variable
uint16_t myuint16 = 0;








// function definitions



#define GETBIT00 ( flags & 0b0000000000000001 )
#define GETBIT01 ( ( flags & 0b0000000000000010 ) >> 1 )
#define GETBIT02 ( ( flags & 0b0000000000000100 ) >> 2 )
#define GETBIT03 ( ( flags & 0b0000000000001000 ) >> 3 )
#define GETBIT04 ( ( flags & 0b0000000000010000 ) >> 4 )
#define GETBIT05 ( ( flags & 0b0000000000100000 ) >> 5 )
#define GETBIT06 ( ( flags & 0b0000000001000000 ) >> 6 )
#define GETBIT07 ( ( flags & 0b0000000010000000 ) >> 7 )
#define GETBIT08 ( ( flags & 0b0000000100000000 ) >> 8 )
#define GETBIT09 ( ( flags & 0b0000001000000000 ) >> 9 )
#define GETBIT10 ( ( flags & 0b0000010000000000 ) >> 10 )
#define GETBIT11 ( ( flags & 0b0000100000000000 ) >> 11 )
#define GETBIT12 ( ( flags & 0b0001000000000000 ) >> 12 )
#define GETBIT13 ( ( flags & 0b0010000000000000 ) >> 13 )
#define GETBIT14 ( ( flags & 0b0100000000000000 ) >> 14 )
#define GETBIT15 ( ( flags & 0b1000000000000000 ) >> 15 )

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

  // clear out our globalcache first
  for ( int i = 0 ; i < 16 ; i++ ) {
    globalcache[i] = 0;
  }




  // use a 16 bit variable as flags for scheduling
  uint16_t flags = 0;


/*

  globalcache map for sensor readings
  00 - 05 Timestamp
  06 - 07 luminance
  08 - 09 HTU temp
  10 - 11 HTU humidity
  12 - 13 BMP temp
  14 - 15 BMP pressure

*/

  digitalWrite( LED_YEL , HIGH );

  do {
      
    if ( GETBIT00 == 0 ) {
      // fetch the current time. 6 bytes
      gettimeaddateraw( globalcache );
      SETBIT00
    }


    // fetch the current limunance value. 2 bytes
    // has luminance sensor started?
    if ( GETBIT01 == 0 ) {
      // start bh1750 luminance sensor up
      bh1750lowresoneshotrawstart();
      // set BIT01 to 1;
      SETBIT01
    }

    // has luminance sensor been started?
    if ( GETBIT01 == 1 ) {
      // has luminance sensor finished?
      if ( GETBIT02 == 0 ) {
        // poll for sensor data
        bh1750lowresoneshotrawend( globalcache );
        // if it has arrived, set BIT02 to 1;
        if ( ( globalcache[6] | globalcache[7] ) != 0 ) {
          SETBIT02
        }
      }
    }

    
    // fetch the current temperature and humidity.  2x2 bytes
    //htu2x
    // has required order, temperature then humidity
    if ( GETBIT03 == 0 ) {
      // start temperature reading
      htu2xgettemperaturerawstart();
      // set BIT03 to 1
      SETBIT03
    }

    // if temperature reading has begun,
    if ( GETBIT03 == 1 ) {
      // has temperature reading finished?
      if ( GETBIT04 == 0 ) {
        // poll for data
        htu2xgettemperaturerawend( globalcache );
        // if it has arrived, set BIT04 to 1;
        if ( ( globalcache[8] | globalcache[9] ) != 0 ) {
          SETBIT04
        }
      }
    }

    // if temperature reading is done, start humidity reading
    if ( GETBIT04 == 1 ) {
      // if I have not send the start command previously
      if ( GETBIT05 == 0 ) {
        // send humidity start command
        htu2xgethumidityrawstart();
        // set BIT05 to 1;
        SETBIT05
      }
    }

    // if humidity start has been sent...
    if ( GETBIT05 == 1 ) {
      // is the pressure reading finished?
      if ( GETBIT06 == 0 ) {
        // check if any data has arrived
        htu2xgethumidityrawend( globalcache );
        // if so, set BIT06 to 1;
        if ( ( globalcache[8] | globalcache[9] ) != 0 ) {
          SETBIT06
        }
      }
    }


    
    // fetch the current temperature and pressure. 2x2 bytes
    // bmp085 requires order 1. temperature 2. pressure
    // has temperature measurement been started.
    if ( GETBIT07 == 0 ) {
      // start temperature sensing
      bmp085gettemperaturerawstart();
      // set BIT07 to 1
      SETBIT07
    }

    // if temperature sensing is not yet completed
    if ( GETBIT07 == 1 ) {
      if ( GETBIT08 == 0 ) {
        // check if any data is available
        bmp085gettemperaturerawend( globalcache );
        // if so, set BIT08 to 1.
        if ( ( globalcache[12] | globalcache[13] ) != 0 ) {
          SETBIT08
        }
      }
    }

    //check if it's time to start pressure sensing
    if ( GETBIT08 == 1 ) {
      // check we haven't already sent the command
      if ( GETBIT09 == 0 ) {
        // send the pressure start command
        bmp085getpressurerawstart();
        // set BIT09 to 1.
        SETBIT09
      }
    }

    // if presure start has been sent
    if ( GETBIT09 == 1 ) {
      // if results have not yet been received
      if ( GETBIT10 == 0 ) {
        // if I have data available
        bmp085getpressurerawend( globalcache );
        // set BIT10 to 1.
        if ( ( globalcache[14] | globalcache[15] ) != 0 ) {
          SETBIT10
        }
      }
    }

  }
  while ( ( GETBIT00 == 0 ) || ( GETBIT02 == 0 ) || ( GETBIT06 == 0 ) || ( GETBIT10 == 0 ) );
  // process is complete when the following bits are set.
  // BIT00 = time retreival is complete.
  // BIT02 = luminance sensor has finished.
  // BIT06 = humidity sensor has finished.
  // BIT10 = pressure sensor has finished.
  digitalWrite( LED_YEL , LOW );
  

  // insert data storage code here ;)
  framwritesensordata( globalcache );


  // all done, now return
  return;

}



















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