// Interfacing Arduino with NEO-6M GPS module

#include <TinyGPS++.h>           // Include TinyGPS++ library
#include <SoftwareSerial.h>      // Include software serial library
#include "tracker.h";
//RTC_DATA_ATTR
//TinyGPSPlus gps;
//RTC_DATA_ATTR TinyGPSPlus gps;

 
#define S_RX    26          // Define software serial RX pin
#define S_TX    25        // Define software serial TX pin
//  RTC_DATA_ATTR 
//SoftwareSerial SoftSerial(26, 25);
  // Configure SoftSerial library

  float battery=0.0,speed=0.0;
  byte uploaded=1;
  String dat="2021-01-01",time1="01:01:01.100",latt="9.0", lon="0.0";
  float x=0.0,y=0.0,z=0.0;
void gpsinit(){
//  TinyGPSPlus gps;
//  SoftwareSerial SoftSerial(26, 25);
//  SoftSerial.begin(9600);
}
void getGpsData(){
  byte signl=1;
    TinyGPSPlus gps;
  SoftwareSerial SoftSerial(26, 25);
  SoftSerial.begin(9600);
  getAccData(&x,&y,&z);
  while(true){
  while (SoftSerial.available() > 0) 
  {

//    Serial.println("Getting Gps ");
    if (gps.encode(SoftSerial.read())) {
      signl=0;
      Serial.println("Getting Gps 1 ");
      if (gps.location.isValid()) {
        Serial.print("Latitude   = ");
        Serial.println(gps.location.lat(), 6);
        latt=String(gps.location.lat(),6);
        Serial.print("Longitude  = ");
        Serial.println(gps.location.lng(), 6);
        lon=String(gps.location.lng(), 6);
        Serial.println("GPS location fixed");
//        signl=0;
      }
      else{
        Serial.println("Location Invalid");
//      latt=0.0, lon=0.0;
              Serial.print("Latitude   = ");
        Serial.println(gps.location.lat(), 6);
        latt=String(gps.location.lat(),6);
        Serial.print("Longitude  = ");
        Serial.println(gps.location.lng(), 6);
        lon=String(gps.location.lng(), 6);
        Serial.println("GPS location fixed");
      }
      if (gps.speed.isValid()) {
        Serial.print("Speed      = ");
        Serial.print(gps.speed.kmph());
        speed=gps.speed.kmph();
        Serial.println(" kmph");
      }
      else{
        Serial.println("Speed Invalid");
        speed=0.0;
        //dat="00/00/00";
      }
      if (gps.time.isValid()) {
        String hour,min,sec;
        Serial.print("Time (GMT) : ");
        if(gps.time.hour() < 10){  hour="0"+(String)gps.time.hour();   Serial.print("0");}
        else{hour=(String)gps.time.hour();}
        Serial.print(gps.time.hour());
        Serial.print(":");
        if(gps.time.minute() < 10){min="0"+(String)gps.time.minute();   Serial.print("0");}
        else{min=(String)gps.time.minute();}
        Serial.print(gps.time.minute());
        Serial.print(":");
        if(gps.time.second() < 10) {sec="0"+(String)gps.time.second();  Serial.print("0");}
        else{sec=(String)gps.time.second();}
        Serial.println(gps.time.second());
        time1=hour+":"+min+":"+sec+".123";
        Serial.println(time1);
      }
      else{
        Serial.println("Time Invalid");
        Serial.println(time1);
      }
 
      if (gps.date.isValid()) {
        String day,month;
        Serial.print("Date       : ");
        if(gps.date.day() < 10) { day="0"+(String)gps.date.day();    Serial.print("0");}
        else{day=gps.date.day();}
        Serial.print(gps.date.day());
        Serial.print("-");
        if(gps.date.month() < 10){month="0"+(String)gps.date.month();    Serial.print("0");}
        else{month=gps.date.month();}
        Serial.print(gps.date.month());
        Serial.print("-");
        Serial.println(gps.date.year());
        dat=(String)gps.date.year()+"-"+month+"-"+day;
      
      if(dat=="2000-00-00")
      dat="2021-01-01";
      Serial.println(dat);
      }
      else{
        Serial.println("Date Invalid");
       dat="2021-01-01";
       Serial.println(time1);
    }
    }
      if(signl==0)
      {db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),(dat+" "+time1),String(batchID),String(x),String(y),String (z));    
    break;
    }
    Serial.println("while serial is available");
}
Serial.println("while true");
  if(signl==0)
    break;
}
//  if (!(SoftSerial.available() > 0) ){// Gets executed when there is no gps data input
//  Serial.println("No Gps signal");
//  db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),(dat+" "+time1),String(batchID),String(x),String(y),String (z));
//  }
}
