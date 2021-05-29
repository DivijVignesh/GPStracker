
void getGpsData(){
  Serial.println("Getting GPSdata");
float latt=9.0, lon=0.0,battery=0.0,speed=0.0;
byte uploaded;
String dat="90/00/00",time="00-00-00";
int batchID=10;

	    while (SoftSerial.available() > 0) {
//Serial.println("Getting Gps ");

      
    if (gps.encode(SoftSerial.read())) {
 
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
        Serial.print("Time (GMT) : ");
        if(gps.time.hour() < 10)     Serial.print("0");
        Serial.print(gps.time.hour());
        Serial.print(":");
        if(gps.time.minute() < 10)   Serial.print("0");
        Serial.print(gps.time.minute());
        Serial.print(":");
        if(gps.time.second() < 10)   Serial.print("0");
        Serial.println(gps.time.second());
        dat=(String)gps.time.hour()+":"+(String)gps.time.minute()+":"+(String)gps.time.second();
        Serial.println(dat);
      }
      else{
        Serial.println("Time Invalid");
        Serial.println(dat);
      }
 
      if (gps.date.isValid()) {
        Serial.print("Date       : ");
        if(gps.date.day() < 10)      Serial.print("0");
        Serial.print(gps.date.day());
        Serial.print("/");
        if(gps.date.month() < 10)    Serial.print("0");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.println(gps.date.year());
        time=(String)gps.date.day()+"/"+(String)gps.date.month()+"/"+(String)gps.date.year();
      Serial.println(time);
      }
      else{
        Serial.println("Date Invalid");
      
       time="00-00-00";
       Serial.println(time);
 
    }
    Serial.println("DB");
db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),dat,time,String(batchID));    
break;
}


}
if(!SoftSerial.available()){// Gets executed when there is no gps data input
  Serial.println("No Gps signal");
  db_init(String(latt),String(lon),String(battery),String(speed),String(uploaded),dat,time,String(batchID)); 
}
}
