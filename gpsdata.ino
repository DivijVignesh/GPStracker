// Interfacing Arduino with NEO-6M GPS module

#include <TinyGPS++.h>           // Include TinyGPS++ library
#include <SoftwareSerial.h>      // Include software serial library
 
RTC_DATA_ATTR TinyGPSPlus gps;
 
#define S_RX    26          // Define software serial RX pin
#define S_TX    25        // Define software serial TX pin
  RTC_DATA_ATTR SoftwareSerial SoftSerial(26, 25);

void gpsinit(){
 
    SoftSerial.begin(9600);
}
void getGpsData(){
  
      // Configure SoftSerial library

Serial.println("Getting GPSdata");
float latt=9.0, lon=0.0,battery=0.0,speed=0.0;
byte uploaded;
String dat="2021-01-01",time="01:01:01.100";
int batchID=10;
Serial.println(SoftSerial.read());
      while (SoftSerial.available() > 0) {
Serial.println("Getting Gps ");

      
    if (gps.encode(SoftSerial.read())) {
 Serial.println("Getting Gps 1 ");
      if (gps.location.isValid()) {
        Serial.print("Latitude   = ");
        Serial.println(gps.location.lat(), 6);
        latt=gps.location.lat();
        Serial.print("Longitude  = ");
        Serial.println(gps.location.lng(), 6);
        lon=gps.location.lng();
        Serial.println("GPS location fixed");
      }
      else{
        Serial.println("Location Invalid");
      latt=0.0, lon=0.0;
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
        time=hour+":"+min+":"+sec+".000";
        Serial.println(time);
      }
      else{
        Serial.println("Time Invalid");
        Serial.println(time);
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
      Serial.println(dat);
      }
      else{
        Serial.println("Date Invalid");
      
       dat="00-00-00";
       Serial.println(time);
 
    }
    Serial.println("DB");
db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),(dat+" "+time),String(batchID));    
break;
}
//else {// Gets executed when there is no gps data input
//  Serial.println("No Gps signal");
//  db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),(dat+" "+time),String(batchID));
//   break;
//}

}
if (!(SoftSerial.available() > 0) ){// Gets executed when there is no gps data input
  Serial.println("No Gps signal");
  db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),(dat+" "+time),String(batchID));
   
}

}
