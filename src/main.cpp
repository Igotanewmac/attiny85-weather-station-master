
#include "globalheader.h"



// wire master library for the USI hardware
// #include <TinyWireM.h>

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














// globals

// global 32 byte data ram cache
uint8_t globalcache[64] = { 0 };

// global 32bit variable
uint32_t myuint32 = 0;

// global 16 bit variable
uint16_t myuint16 = 0;








// function definitions






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