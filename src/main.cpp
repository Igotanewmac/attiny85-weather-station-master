#include <Arduino.h>

// wire master library for the USI hardware
#include <TinyWireM.h>
#define wire TinyWireM



// the actual bus id's
#define I2CBUSIDSENSORS 0x00
#define I2CBUSIDSTORAGE 0x01

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



// FRAM addresses
#define I2CADDRESSFRAMBUS 0x00
#define I2CADDRESSFRAM0 0x50
#define I2CADDRESSFRAM1 0x51

uint8_t framworkmemory = I2CADDRESSFRAM0;
uint8_t framdatamemory = I2CADDRESSFRAM1;





// switch to a new i2c bus.
// busid is an int from 0 to 7 that corresponds to the on-chip bus ids.

#define I2CADDRESSBUSMASTER 0x70









// function declarations
void i2cswitchbus();
void i2cbuson();
void i2cbusoff();




















void setup() {
  // put your setup code here, to run once:

  // set an output
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







void loop() {
  // put your main code here, to run repeatedly:

  // if the trigger pin is anything but LOW, return super early.
  if ( digitalRead( PIN_TRIGGER1 ) != LOW ) { return; }

  // the alarm has triggered, our pin went low!
  // turn on the red led
  digitalWrite( LED_RED , HIGH );

  // now turn off our alarm




}












/// @brief Switch to a new i2c bus, from 0-7
/// @param newbusid The busid to switch to.
void i2cswitchbus( uint8_t newbusid ) {

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








/// @brief Connect i2c busses
void i2cbuson() {

  // turn on our i2c interface
  wire.begin();

  // all done, return to caller
  return;

}









/// @brief Disconnect from i2c busses
void i2cbusoff() {

  // turn off the i2c switches
  wire.beginTransmission( I2CADDRESSBUSMASTER );
  wire.send( 0x00 );
  wire.endTransmission();

  // now turn off our own i2c
  wire.end();

  // and pull our pins back up
  pinMode( SDA , INPUT_PULLUP );
  pinMode( SCL , INPUT_PULLUP );
  
  // all done, now return to caller
  return;

}












//