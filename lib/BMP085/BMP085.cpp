
// bmp085 library

#include <Arduino.h>

#include <TinyWireM.h>
#define wire TinyWireM


#define BMP085ADDRESS 0x77



// write 0x2E into reg 0xF4, 
// wait 4.5ms 
// read reg 0xF6 (MSB), 0xF7 (LSB)
// UT = MSB << 8 + LSB
void bmp085gettemperaturerawstart() {

    // say hello to the chip
    wire.beginTransmission( BMP085ADDRESS );
    //write the register address
    wire.send( 0xF4 );
    // write the data for this register
    wire.send( 0x2E );
    // end the transaction
    wire.endTransmission();
    
    // now return to caller
    return;

}





void bmp085gettemperaturerawend( uint8_t *globalcache ) {

    // first say hello to the chip
    wire.beginTransmission( BMP085ADDRESS );
    // write the register address
    wire.send( 0xF4 );
    // now end the transmission
    wire.endTransmission();

    // now request the byte at this address
    wire.requestFrom( BMP085ADDRESS , 1 );
    // check if bit 5 is true
    if ( ( wire.receive() & 0b00100000 ) == 0b00100000 ) {
        // scan is still in progress
        // return early
        return;
    }

    // eoc bit has been set to zero, gather the data

    // say hello to the chip
    wire.beginTransmission( BMP085ADDRESS );
    // send the register address
    wire.send( 0xF6 );
    // now say goodbye
    wire.endTransmission();

    // now request two bytes back
    wire.requestFrom( BMP085ADDRESS , 2 );

    // store the bytes
    globalcache[12] = wire.receive();
    globalcache[13] = wire.receive();

    // now return to caller
    return;

}













// read uncompensated pressure value
// write 0x34+(oss<<6) into reg 0xF4, wait
// read reg 0xF6 (MSB), 0xF7 (LSB), 0xF8 (XLSB)
// UP = (MSB<<16 + LSB<<8 + XLSB) >> (8-oss)

void bmp085getpressurerawstart() {

    // oss is zero ( oversampling mode )

    // say hello to the chip
    wire.beginTransmission( BMP085ADDRESS );
    // set the register address
    wire.send( 0xF4 );
    // now set the command opcode
    wire.send( 0x34 );
    // now say goodbye
    wire.endTransmission();

    // and now return to caller
    return;

}





void bmp085getpressurerawend( uint8_t *globalcache ) {

    // first say hello to the chip
    wire.beginTransmission( BMP085ADDRESS );
    // write the register address
    wire.send( 0xF4 );
    // now end the transmission
    wire.endTransmission();

    // now request the byte at this address
    wire.requestFrom( BMP085ADDRESS , 1 );
    // check if bit 5 is true
    if ( ( wire.receive() & 0b00100000 ) == 0b00100000 ) {
        // scan is still in progress
        // return early
        return;
    }

    // eoc bit has been set to zero, gather the data

    // say hello to the chip
    wire.beginTransmission( BMP085ADDRESS );
    // send the register address
    wire.send( 0xF6 );
    // now say goodbye
    wire.endTransmission();

    // now request two bytes back
    wire.requestFrom( BMP085ADDRESS , 2 );

    // store the bytes
    globalcache[14] = wire.receive();
    globalcache[15] = wire.receive();

    // now return to caller
    return;

}





























































//