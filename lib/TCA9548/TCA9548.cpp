

#include "globalheader.h"

// bus switcher current bus id
uint8_t currentbusid = 0x00;



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


