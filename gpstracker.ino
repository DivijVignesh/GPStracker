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
    gpsinit();
   Serial.begin(115200);
   
}

void loop() {

   if(flag!=1){
    Serial.println("Entering Deep Sleep for 9 sec");
  flag=1;
  deepsleep(8);
}
delay(1000);
Serial.println("Deep Sleep Done");

getGpsData();

flag=0;
    }
