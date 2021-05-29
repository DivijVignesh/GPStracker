#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SD.h"
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds*/
#define SD_MODULE 2
#define GPS_MODULE 4
RTC_DATA_ATTR byte flag = 0;
RTC_DATA_ATTR int batch = 1;
RTC_DATA_ATTR int sno = 1;


// Interfacing Arduino with NEO-6M GPS module

#include <TinyGPS++.h>           // Include TinyGPS++ library
#include <SoftwareSerial.h>      // Include software serial library
 
TinyGPSPlus gps;
 
#define S_RX    16               // Define software serial RX pin
#define S_TX    17            // Define software serial TX pin
 
SoftwareSerial SoftSerial(S_RX, S_TX);    // Configure SoftSerial library


void setup() {
  pinMode(SD_MODULE,OUTPUT);
pinMode(GPS_MODULE,OUTPUT);
   Serial.begin(9600);
   SoftSerial.begin(9600);
   Serial.println("Starting in 10");
}

void loop() {
   if(flag!=1){
    digitalWrite(SD_MODULE,LOW);
    digitalWrite(GPS_MODULE,LOW);
    Serial.println("Entering Deep Sleep");
  flag=1;
  deepsleep(8);
}
digitalWrite(SD_MODULE,HIGH);
digitalWrite(GPS_MODULE,HIGH);
Serial.println("Deep Sleep Done");
getGpsData();
flag=0;
    }
