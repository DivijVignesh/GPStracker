#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

void acc_init(){
   if(!accel.begin())
   {
      Serial.println("No valid sensor found");
      while(1);
   }
}

void getAccData(float *x,float *y,float *z){
   sensors_event_t event; 
   accel.getEvent(&event);
   Serial.print("X: "); *x=event.acceleration.x;  Serial.print(event.acceleration.x); Serial.print("  ");
   Serial.print("Y: "); *y=event.acceleration.y;Serial.print(event.acceleration.y); Serial.print("  ");
   Serial.print("Z: "); *z=event.acceleration.z;Serial.print(event.acceleration.z); Serial.print("  ");
   Serial.println("m/s^2 ");
   delay(100);
}
