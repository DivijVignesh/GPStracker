#include<WiFi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SD.h"
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds*/

#include <TinyGPS++.h>           // Include TinyGPS++ library
#include <SoftwareSerial.h>      // Include software serial library
#include "tracker.h";


RTC_DATA_ATTR byte flag = 0;
RTC_DATA_ATTR int batch = 1;
RTC_DATA_ATTR int sno = 1;
RTC_DATA_ATTR int isUploading=1;
RTC_DATA_ATTR int batchID=1;
RTC_DATA_ATTR int getTransmitData=1;
RTC_DATA_ATTR byte getBatchForUpload=1;
//RTC_DATA_ATTR int result=1;
//TaskHandle_t Task1;
TaskHandle_t Task2;


void setup() {
  WiFi.mode( WIFI_MODE_NULL );
  pinMode(2,OUTPUT);
//    xTaskCreatePinnedToCore(
//                    Task1code,   /* Task function. */
//                    "Task1",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    0,           /* priority of the task */
//                    &Task1,      /* Task handle to keep track of created task */
//                    0);          /* pin task to core 0 */  

  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    0,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */  
  Serial.begin(115200);
  Serial.print("Core:");
  Serial.println(xPortGetCoreID()); // Printing the core number in which setup is being executed
    gpsinit();
    acc_init();
   
   
}
void Task2code( void * pvParameters ){
 
////RTC_DATA_ATTR
//TinyGPSPlus gps;
// 
//#define S_RX    26          // Define software serial RX pin
//#define S_TX    25        // Define software serial TX pin
////  RTC_DATA_ATTR 
//  SoftwareSerial SoftSerial(26, 25);
//  // Configure SoftSerial library
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
 Serial.print("Core:");
Serial.println(xPortGetCoreID());
   if(flag!=1){
    Serial.println("Entering Deep Sleep for 9 sec");
  flag=1;
  
  if(isUploading!=0)
  deepsleep(9);
  else
  vTaskDelay(9000/portTICK_PERIOD_MS);
//  delay(9000);
  
  Serial.println("Still sleeping");
}
//delay(1000);
Serial.println("Deep Sleep Done");
getGpsData();                       //Gattering gps data 


flag=0;
//vTaskDelay(10000/portTICK_PERIOD_MS);
    
//  taskYIELD();
  }
  
}


void loop() {
Task1code();
//  Serial.print("Core:");  
//Serial.println(xPortGetCoreID());
//   if(flag!=1){
//    Serial.println("Entering Deep Sleep for 9 sec");
//  flag=1;
////  deepsleep(9);
//  delay(9000);
//  Serial.println("Still sleeping");
//}
//delay(1000);
//Serial.println("Deep Sleep Done");
//getGpsData();                       //Gattering gps data 
//
//
//flag=0;
    }
