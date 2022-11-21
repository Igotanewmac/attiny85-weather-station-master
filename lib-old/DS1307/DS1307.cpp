

// rtc functions

#include <TinyWireM.h>

#ifndef tw
#define tw TinyWireM
#endif

#define DS1307ADDRESS 0x68


// functions required are purely RTC functional
// no bus setup since that's handled by main





// check if clock is set

// check for clock halted bit


/// @brief Check if the clock has stopped.
/// @return 0 (running) or 1 (has stopped) as a uint8_t
uint8_t clockHasStopped() {

    // address the clock
    // retrive the data

    // say hello to the clock
    // send a zero to set out ram position
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x00 );
    tw.endTransmission();
    
    // request a byte
    tw.requestFrom( DS1307ADDRESS , 1 );
    uint8_t retval = tw.receive();
    uint8_t rtcdata = ( retval & 0x80 ) >> 7;

    return rtcdata;

}





// get current time and date

void getTimeAndDate( uint8_t *retarray ) {

    uint8_t clockretval = 0;

    // set the byte pointer to 0
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x00 );
    tw.endTransmission();

    
    // fetch the seconds from position 0
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // seconds ones
    retarray[0] = ( clockretval & 0b00001111 );
    // seconds tens
    retarray[1] = ( ( clockretval & 0b01110000 ) >> 4 );


    // fetch the minutes from position 1
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // minutes ones
    retarray[2] = ( clockretval & 0b00001111 );
    // minutes tens
    retarray[3] = ( ( clockretval & 0b01110000 ) >> 4 );



    // fetch the hours from position 2
    tw.requestFrom( DS1307ADDRESS  , 1 );
    clockretval = tw.receive();
    // hours ones
    retarray[4] = ( clockretval & 0b00001111 );
    // hours tens
    retarray[5] = ( clockretval & 0b00110000 ) >> 4;



    // ignore position 3 day of week
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();



    // fetch the day from position 4
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // day ones
    retarray[6] = ( clockretval & 0b00001111 );
    // day tens
    retarray[7] = ( clockretval & 0b00110000 ) >> 4;



    // fetch the month from position 5
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // month ones
    retarray[8] = ( clockretval & 0b00001111 );
    // month tens
    retarray[9] = ( clockretval & 0b00010000 ) >> 4;


    // fetch the year from position 6
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // year ones
    retarray[10] = ( clockretval & 0b00001111 );
    // year tens
    retarray[11] = ( clockretval & 0b11110000 ) >> 4;


    // all done!

}








//