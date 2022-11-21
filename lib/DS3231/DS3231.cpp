

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
    
    return (uint8_t)( ( tw.receive() & 0x80 ) >> 7 );

}





// get current time and date

uint32_t getTimeAndDate() {

    uint8_t clockretval = 0;

    uint32_t epochtime = 0;

    // set the byte pointer to 0
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x00 );
    tw.endTransmission();

    
    // fetch the seconds from position 0
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // seconds ones
    // retarray[0] = ( clockretval & 0b00001111 );
    // seconds tens
    // retarray[1] = ( ( clockretval & 0b01110000 ) >> 4 );
    epochtime += ( ( ( ( clockretval & 0b01110000 ) >> 4 ) * 10 ) + ( clockretval & 0b00001111 ) );


    // fetch the minutes from position 1
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // minutes ones
    // retarray[2] = ( clockretval & 0b00001111 );
    // minutes tens
    // retarray[3] = ( ( clockretval & 0b01110000 ) >> 4 );
    epochtime += ( ( ( ( ( clockretval & 0b01110000 ) >> 4 ) * 10 ) + ( clockretval & 0b00001111 ) ) * 60 );
    


    // fetch the hours from position 2
    tw.requestFrom( DS1307ADDRESS  , 1 );
    clockretval = tw.receive();
    // hours ones
    // retarray[4] = ( clockretval & 0b00001111 );
    // hours tens
    // retarray[5] = ( clockretval & 0b00110000 ) >> 4;
    epochtime += ( ( ( ( ( clockretval & 0b01110000 ) >> 4 ) * 10 ) + ( clockretval & 0b00001111 ) ) * 3600 );
    


    // ignore position 3 day of week
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();



    // fetch the day from position 4
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // day ones
    // retarray[6] = ( clockretval & 0b00001111 );
    // day tens
    // retarray[7] = ( clockretval & 0b00110000 ) >> 4;
    epochtime += ( ( ( ( ( clockretval & 0b01110000 ) >> 4 ) * 10 ) + ( clockretval & 0b00001111 ) ) *  86400 );
    


    // fetch the month from position 5
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // month ones
    // retarray[8] = ( clockretval & 0b00001111 );
    // month tens
    // retarray[9] = ( clockretval & 0b00010000 ) >> 4;
    switch ( ( ( ( ( clockretval & 0b01110000 ) >> 4 ) * 10 ) + ( clockretval & 0b00001111 ) ) ) {
    
        case 1:
            /* code */
            epochtime += 0; // seconds until january
            break;
        case 2:
            /* code */
            epochtime += 2678400; // seconds until february
            break;
        case 3:
            /* code */
            epochtime += 5097600; // seconds until march
            break;
        case 4:
            /* code */
            epochtime += 7776000; // seconds until april
            break;
        case 5:
            /* code */
            epochtime += 10368000; // seconds until may
            break;
        case 6:
            /* code */
            epochtime += 13046400; // seconds until june
            break;
        case 7:
            /* code */
            epochtime += 15638400; // seconds until july
            break;
        case 8:
            /* code */
            epochtime += 18316800; // seconds until august
            break;
        case 9:
            /* code */
            epochtime += 20995200; // seconds until september
            break;
        case 10:
            /* code */
            epochtime += 23587200; // seconds until october
            break;
        case 11:
            /* code */
            epochtime += 26265600; // seconds until november
            break;
        case 12:
            /* code */
            epochtime += 28857600; // seconds until december
            break;
    
        default:
            break;
    }


    // fetch the year from position 6
    tw.requestFrom( DS1307ADDRESS , 1 );
    clockretval = tw.receive();
    // year ones
    // retarray[10] = ( clockretval & 0b00001111 );
    // year tens
    // retarray[11] = ( clockretval & 0b11110000 ) >> 4;
    epochtime += ( ( ( ( ( clockretval & 0b01110000 ) >> 4 ) * 10 ) + ( clockretval & 0b00001111 ) ) *  31536000 );
    

    // all done!
    return epochtime;

}




void setonesecondalarm() {
    // just quickly configure the clock while we are here...
    // set ALM1 registers to once a second.
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x07 );
    tw.send( 0x00 );
    tw.endTransmission();
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x08 );
    tw.send( 0x00 );
    tw.endTransmission();
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x09 );
    tw.send( 0x00 );
    tw.endTransmission();
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x0A );
    tw.send( 0x00 );
    tw.endTransmission();

    // read and writeback the control register byte
    uint8_t controlregister = 0;
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x0E );
    tw.endTransmission();
    tw.requestFrom( DS1307ADDRESS , 1 );
    controlregister = tw.receive();

    // now set the last three bits
    controlregister |= 0b00000111;

    // and now write it back.
    tw.beginTransmission( DS1307ADDRESS );
    tw.send( 0x0E );
    tw.send( controlregister );
    tw.end();
}



//