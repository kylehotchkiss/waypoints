/**
 *
 * Waypoints Game (for Arduino)
 * Copyright 2013 Kyle Hotchkiss (for Hotchkissmade)
 * Released under the GPL
 *
 * Reads GPS, outputs distance to waypoint to LCD.
 * Neat for Treasure Hunts
 *
 */

#define _USE_MATH_DEFINES
#include <SoftwareSerial.h>


////////////////////////////////////
// What Matters to you - Settings //
////////////////////////////////////

// Where to?
const double waypoint_lat = 37.7070794;
const double waypoint_lon = -79.2889216;

// Assumed average speed in MPH (Randomization indexer)
const int avgSpeed = 30;


///////////////////
// LCD Constants //
///////////////////
const int lcd_pin = 3;

// ASCII control codes
const int lcd_clear = 12;
const int lcd_lightOn = 17;
const int lcd_lightOff = 18;
const int lcd_newline = 13;


///////////////////
// GPS Constants //
///////////////////
const char gps_endline = 13;
const int gps_maxchars = 82;


////////////////////
// Math Constants //
////////////////////
const double RADIANS = M_PI / 180;


SoftwareSerial lcd = SoftwareSerial(255, lcd_pin);


///////////////////
// Init Routines //
///////////////////
void setup() {
    Serial.begin(4800);
    
    //////////////////////////////////////////
    // GPS Setup: Stop showing so much crap //
    //////////////////////////////////////////
    /*Serial.write("$PSRF105,1*3E\r\n");
    Serial.write("$PSRF103,01,00,00,01*25\r\n");
    Serial.write("$PSRF103,02,00,00,01*26\r\n");
    Serial.write("$PSRF103,03,00,00,01*27\r\n");
    Serial.write("$PSRF103,04,00,00,01*20\r\n");
    Serial.write("$PSRF103,05,00,00,01*21\r\n");*/
    
    ////////////////
    // LCD Setup  //
    ////////////////
    pinMode(lcd_pin, OUTPUT);
    digitalWrite(lcd_pin, HIGH);
    lcd.begin(9600);
    delay(100);
    lcd.write( lcd_clear );
    delay(500);
}


void loop() {
    double latitude = 0;
    double longitude = 0;
    double altitude = 0;

    if ( Serial.available() > 0 ) {
        int count = 0;
        char NMEA[gps_maxchars] = "";
        
        count = Serial.readBytesUntil('\r', NMEA, gps_maxchars);
        
        ////////////////////////
        // Valid GPGGA Packet //
        ////////////////////////
        if ( count == 77 ) { // Changes with Altitude FIX ME FOR GPGGA FIX ME FOR 300ft+
            int gps_checksum = 0;
            int calc_checksum = 0;
            char gps_checksum_hex[3];
            
                
            ///////////////////////////////////////
            // Calculate Checksum for Validation //
            ///////////////////////////////////////
            for ( int i = 2; i < gps_maxchars; i++ ) {
                if ( NMEA[i] != '*') {
                    calc_checksum ^= byte( NMEA[i] );
                } else {
                    break;
                }
            }
            
            for ( int i = 0; i < 2; i++ ) {
                gps_checksum_hex[i] = NMEA[ i + 75 ];
            }
            
            gps_checksum_hex[2] = '\0';
            
            gps_checksum = strtol(gps_checksum_hex, NULL, 16);
            
            
            ////////////////////////////////////
            // Checksums Passed, Data is Safe //
            ////////////////////////////////////
            if ( calc_checksum == gps_checksum ) {
                //
                // Assignment
                //
                double latitude_dec, latitude_min, longitude_dec, longitude_min;
                char char_latitude_dec[3], char_latitude_min[8], char_longitude_dec[4], char_longitude_min[8], char_altitude[6];
                String gps_latitude_dec, gps_latitude_min, gps_longitude_dec, gps_longitude_min, gps_altitude, ns_indicator, ew_indicator;
                String NMEA_string = NMEA;
                
                //
                // Convert to Substrings
                //
                gps_latitude_dec = NMEA_string.substring(19, 21);
                gps_latitude_min = NMEA_string.substring(21, 28);
                gps_longitude_dec = NMEA_string.substring(31, 34);
                gps_longitude_min = NMEA_string.substring(34, 41);
                gps_altitude = NMEA_string.substring(53, 59);
                ns_indicator = NMEA_string.substring(29, 30);
                ew_indicator = NMEA_string.substring(42, 43);
                
                //
                // Back to Char Arrays
                //
                gps_latitude_dec.toCharArray(char_latitude_dec, 3);
                gps_latitude_min.toCharArray(char_latitude_min, 8);
                gps_longitude_dec.toCharArray(char_longitude_dec, 4);
                gps_longitude_min.toCharArray(char_longitude_min, 8);
                gps_altitude.toCharArray(char_altitude, 6);
                
                //
                // Properly null-terminate strings
                //
                char_latitude_dec[2] = '\0';
                char_latitude_min[7] = '\0';
                char_longitude_dec[3] = '\0';
                char_longitude_min[7] = '\0';
                char_altitude[5] = '\0';
                
                //
                // Char arrays to Double
                //
                latitude_dec = atof( char_latitude_dec );
                latitude_min = atof( char_latitude_min );
                longitude_dec = atof( char_longitude_dec );
                longitude_min = atof( char_longitude_min );
                altitude = atof( char_altitude );
                
                //
                // Convert Lat/Lon to Decimal Degrees
                //
                latitude = latitude_dec + ( latitude_min / 60 );
                longitude = longitude_dec + ( longitude_min / 60 );
                
                //
                // Negative assignment, where needed
                //
                if ( ns_indicator == "S" ) {
                    latitude = latitude * -1;
                }
                
                if ( ew_indicator = "W" ) {
                    longitude = longitude * -1;
                }
            }
        }
    }
    
    ///////////////////////////////////////////
    // GPS-Lock Confirmed | Engagement Layer //
    ///////////////////////////////////////////
    if ( latitude != 0 && longitude != 0 ) {
        lcd.write(212);
        lcd.write(220);
        delay(5);
        lcd.write( lcd_lightOn );
        delay(5);
        lcd.write( lcd_clear );
        delay(5);
        lcd.print("Distance:\r");
        lcd.print( distance( latitude, longitude, waypoint_lat, waypoint_lon ), 4);
        lcd.print( "km" );
        
        delay( 5000 );
        
        lcd.write( lcd_clear );
        delay(5);
        lcd.write( lcd_lightOff );
        
        delay( 20000 );
    }
}


/////////////////////////////////////////////
// Return Distance in Kilometers to Target //
/////////////////////////////////////////////
double distance( double startLat, double startLon, double endLat, double endLon ) {
    double distanceLat  = ( endLat - startLat ) * RADIANS;
    double distanceLon  = ( endLon - startLon ) * RADIANS;
    double lat1		    = startLat * RADIANS;
    double lat2		    = endLat * RADIANS;

    double step1 = sin( distanceLat / 2 ) * sin( distanceLat / 2 ) + sin( distanceLon / 2 ) *
        sin( distanceLon / 2 ) * cos( lat1 ) * cos( lat2 );
    double step2 = 2 * atan2( sqrt( step1 ), sqrt( 1 - step1 ));

    return 6367 * step2;
}