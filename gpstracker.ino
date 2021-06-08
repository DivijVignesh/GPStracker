#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SD.h"
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds*/

RTC_DATA_ATTR byte flag = 0;
RTC_DATA_ATTR int batch = 1;
RTC_DATA_ATTR int sno = 1;


void setup() {
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  Serial.print("Core:");
  Serial.println(xPortGetCoreID()); // Printing the core number in which setup is being executed
    gpsinit();
    acc_init();
   
   
}

void loop() {
  Serial.print("Core:");
Serial.println(xPortGetCoreID());
   if(flag!=1){
    Serial.println("Entering Deep Sleep for 9 sec");
  flag=1;
  deepsleep(8);
}
delay(1000);
Serial.println("Deep Sleep Done");
digitalWrite(2,HIGH);               //Using on board led as an indicator to tell us whether the data is being gattered or not
getGpsData();                       //Gattering gps data 
pinMode(2,LOW);

flag=0;
    }
