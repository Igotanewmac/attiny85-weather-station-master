#include <Arduino.h>

// wire master library for the USI hardware
#include <TinyWireM.h>
#define wire TinyWireM




// include ds3231
#include "DS3231.h"

// incluce BH1750 luminance sensor
#include "BH1750.h"

// include HTU2X humidity and temperature sensor
#include "HTU2X.h"

// include external library for now...
#include "tinyBMP085.h"
tinyBMP085 mybmp085;



// the actual bus id's
#define I2CADDRESSBUSIDSENSORS 0x00
#define I2CADDRESSBUSIDSTORAGE 0x01




// led pin(s)!
#define LED_RED 3
#define LED_YEL 4


// i2c side io pin
// input pin for now.
#define PIN_TRIGGER1 1




// globals

// global 32 byte data ram cache
uint8_t globalcache[32] = { 0 };

// declare some variables to reuse
uint32_t myuint32 = 0;
uint16_t myuint16 = 0;


// bus switcher
uint8_t currentbusid = 0x00;






// switch to a new i2c bus.
// busid is an int from 0 to 7 that corresponds to the on-chip bus ids.

#define I2CADDRESSBUSMASTER 0x70

void switchbus( uint8_t newbusid ) {

  // say hello to the chip
  wire.beginTransmission( I2CADDRESSBUSMASTER );

  // send the actual switch register byte
  wire.send( (uint8_t)( 1 << newbusid ) );

  // say goodbye to the chip
  wire.endTransmission();

  // update the currentbusid storage
  currentbusid = newbusid;

  // all done, now return to caller
  return;

}







// FRAM addresses
#define I2CADDRESSFRAMBUS 0x00
#define I2CADDRESSFRAM0 0x50
#define I2CADDRESSFRAM1 0x51

uint8_t framworkmemory = I2CADDRESSFRAM0;
uint8_t framdatamemory = I2CADDRESSFRAM1;

// set up the bank ids as to which fram is which
void frambegin() {
  // TODO: hard set for now until wear levelling code written.
  framworkmemory = I2CADDRESSFRAM0;
  framdatamemory = I2CADDRESSFRAM1;

  // and return to caller
  return;
}




void framstoresensordata() {

  // fetch the cursor from working memory
  
  // create a cursor variable.
  uint16_t datamemorycursor = 0;

  // say hello to chip
  wire.beginTransmission( framworkmemory );
  // set position to read from
  wire.send( 0x00 );
  wire.send( 0x00 );
  // say goodbye to the chip
  wire.endTransmission();

  // now request two bytes from said position
  wire.requestFrom( framworkmemory , 2 );


  // and actually receive them.
  datamemorycursor |= wire.receive() << 8;
  datamemorycursor |= wire.receive();

  // and now my 16 bit data cursor is set


  // write to this location in data memory
  // say hello to the chip
  wire.beginTransmission( framdatamemory );
  // send the msb of the data cursor
  wire.send( (uint8_t)( ( datamemorycursor >> 8 ) & 0xFF ) );
  // send the lsb of the data cursor
  wire.send( (uint8_t)( datamemorycursor & 0xFF ) );
  // now send in the first 8 bytes of data record
  for ( int i = 0 ; i < 8 ; i++ ) {
    //wire.send( globalcache[ i ] );
    wire.send( i );
    
  }
  // now say goodbye to the chip
  wire.endTransmission();

  datamemorycursor += 8;

  // say hello to the chip
  wire.beginTransmission( framdatamemory );
  // send the msb of the data cursor
  wire.send( (uint8_t)( ( datamemorycursor >> 8 ) & 0xFF ) );
  // send the lsb of the data cursor
  wire.send( (uint8_t)( datamemorycursor & 0xFF ) );
  // now send in the next 8 bytes of data record
  for ( int i = 8 ; i < 16 ; i++ ) {
    //wire.send( globalcache[ i ] );
    wire.send( i );
    
  }
  // now say goodbye to the chip
  wire.endTransmission();

  // increase the cursor to the next record
  datamemorycursor += 8;


  // cgeck for rollover
  //if ( datamemorycursor == 0 ) {
  //  // halt here with red led on.
  //  while(1);
  //}
  // cgeck for rollover
  if ( datamemorycursor ==  32768) {
    // halt here with red led on.
    digitalWrite( LED_RED , HIGH );
    digitalWrite( LED_YEL , HIGH );
    while(1);
  }



  // store the cursor in working memory
  // say hello to the chip
  wire.beginTransmission( framworkmemory );

  // send the address of the datacursor
  wire.send( 0x00 );
  wire.send( 0x00 );

  // now send the datacursor
  wire.send( (uint8_t)( ( datamemorycursor >> 8 ) & 0xFF ) );
  wire.send( (uint8_t)( datamemorycursor & 0xFF ) );

  // now say goodbye to the chip
  wire.endTransmission();

  // and now return to the caller.
  return;

}


















void setup() {
  // put your setup code here, to run once:

  // set an output
  pinMode( LED_RED , OUTPUT );
  pinMode( LED_YEL , OUTPUT );

  // an input trigger
  // turn off the high on the pin first
  digitalWrite( PIN_TRIGGER1 , LOW );
  pinMode ( PIN_TRIGGER1 , INPUT_PULLUP );

  setonesecondalarm();


  // that's actually it for now.

}




// this is a change






void loop() {
  // put your main code here, to run repeatedly:

   setonesecondalarm();

  
  // check for trigger input...
  if ( digitalRead( PIN_TRIGGER1 ) != LOW ) {

    digitalWrite( LED_YEL , HIGH );
    delay( 100 );
    digitalWrite( LED_YEL , LOW );
    delay( 100 );

  }
  else {

    // turn on led!
    digitalWrite( LED_RED , HIGH );

    // begin i2c wire lib
    wire.begin();

    // begin for fram setup
    frambegin();


    // setup for bmp sensor
    mybmp085.begin();

    // read the sensors and put the data into data memory

    // reset the data cache
    for ( int i = 0 ; i < 32 ; i++ ) {
      globalcache[i] = 0;
    }

    // first, switch to the sensor bus
    switchbus( I2CADDRESSBUSIDSENSORS );

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
    
    // now I have a 16 byte data record to store!
    framstoresensordata();

    // now just stop here for a while
    wire.end();
    pinMode( SDA , INPUT_PULLUP );
    pinMode( SCL , INPUT_PULLUP );
    
    // turn off red led
    digitalWrite( LED_RED , LOW );

    // wait for trigger to go off low...
    while( digitalRead( PIN_TRIGGER1 ) == LOW ) {
      delay(1);
    }
  
  }


  // wait for one seconds
  //delay( 100 );



}




























//