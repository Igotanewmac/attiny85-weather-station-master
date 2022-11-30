
// include the arduino definitions
#include <Arduino.h>



// the actual bus id's
// sensors are on bus 0x00
#define I2CBUSIDSENSORS 0x00
// eeprom storage is on bus 0x01
#define I2CBUSIDEEPROM 0x01



/// @brief Switch to a new i2c bus, from 0-7
/// @param newbusid The busid to switch to.
void i2cswitchbus( uint8_t newbusid );


/// @brief Connect i2c busses
void i2cbuson();

/// @brief Disconnect from i2c busses
void i2cbusoff();

