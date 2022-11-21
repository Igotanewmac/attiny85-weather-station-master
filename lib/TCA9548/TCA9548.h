
// include the arduino definitions
#include <Arduino.h>



/// @brief Switch to a new i2c bus, from 0-7
/// @param newbusid The busid to switch to.
void i2cswitchbus( uint8_t newbusid );


/// @brief Connect i2c busses
void i2cbuson();

/// @brief Disconnect from i2c busses
void i2cbusoff();

