
// htu2x humidity sensor


#include "globalheader.h"






// temperature
uint16_t htu2x_gettemperature() {

    // return a two byte temperature

    // say hello to the chip
    wire.beginTransmission( HTU2XADDRESS );
    // send the measure command with hold
    wire.send( 0xe3 );
    // all done
    wire.endTransmission();

    // read back the temperature
    wire.requestFrom( HTU2XADDRESS , 3 );
    // msb
    uint8_t tempmsb = wire.receive();
    uint8_t templsb = wire.receive();
    uint8_t tempcrc = wire.receive();

    tempcrc += 0;
    
    uint16_t rawtempval = ( ( tempmsb << 8 ) | templsb ) & 0xFFFC;

    //Given the raw temperature data, calculate the actual temperature
    // float tempTemperature = rawTemperature * (175.72 / 65536.0); //2^16 = 65536
    // float realTemperature = tempTemperature - 46.85; //From page 14

    float temptempval = rawtempval * ( 175.72 / 65536.0 );
    float finaltempval = temptempval - 46.85;

    // integer portion
    int8_t intpart = (int8_t)finaltempval;

    // fractional portion
    uint8_t fracpart = (uint8_t)( (finaltempval - (int)finaltempval ) * 100 );


    return ( intpart << 8 ) | fracpart;

}





void htu2xgettemperaturerawstart() {
    // say hello to the chip
    wire.beginTransmission( HTU2XADDRESS );
    // send the measure command with no hold
    wire.send( 0xF3 );
    // all done
    wire.endTransmission();
}


void htu2xgettemperaturerawend( uint8_t *globalcache ) {
    globalcache[8] = 0;
    if ( wire.requestFrom( HTU2XADDRESS , 2 ) != 0 ) { return; }
    globalcache[8] = wire.receive();
    globalcache[9] = wire.receive();
}








// humidity
uint16_t htu2x_gethumidity() {

    // say hello to the chip
    wire.beginTransmission( HTU2XADDRESS );
    // send the measure command with hold
    wire.send( 0xE5 );
    // all done
    wire.endTransmission();

    // read back the temperature
    wire.requestFrom( HTU2XADDRESS , 3 );
    // msb
    uint8_t tempmsb = wire.receive();
    uint8_t templsb = wire.receive();
    uint8_t tempcrc = wire.receive();

    tempcrc += 0;
    
    uint16_t rawtempval = ( ( tempmsb << 8 ) | templsb ) & 0xFFFC;


    // //Given the raw humidity data, calculate the actual relative humidity
    // float tempRH = rawHumidity * (125.0 / 65536.0); //2^16 = 65536
    // float rh = tempRH - 6.0; //From page 14

    float temprh = rawtempval * ( 125.0 / 65536.9 );
    float rh = temprh - 6.0;

     // integer portion
    int8_t intpart = (int8_t)rh;

    // fractional portion
    uint8_t fracpart = (uint8_t)( (rh - (int)rh ) * 100 );

    return ( intpart << 8 ) | fracpart;

};





void htu2xgethumidityrawstart() {
    // say hello to the chip
    wire.beginTransmission( HTU2XADDRESS );
    // send the measure command with no hold
    wire.send( 0xF5 );
    // all done
    wire.endTransmission();
}


void htu2xgethumidityrawend( uint8_t *globalcache ) {
    globalcache[8] = 0;
    if ( wire.requestFrom( HTU2XADDRESS , 2 ) != 0 ) { return; }
    globalcache[8] = wire.receive();
    globalcache[9] = wire.receive();
}








































//