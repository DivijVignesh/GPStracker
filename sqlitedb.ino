#include <ArduinoJson.h>

StaticJsonDocument<5000> doc; 
JsonArray array ;
String obj;
   
sqlite3 *db1;
sqlite3 *db2;
   
//   char *zErrMsg = 0;

int rc;
const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName){
JsonObject object = doc.createNestedObject(); // JSON objects for NAME and VALUE of the counter
int i;
Serial.printf("%s: ", (const char*)data);
for (i = 0; i<argc; i++){
    object[(String)azColName[i]]=(String)argv[i];
    Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");

   return 0;
}

int openDb(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       Serial.printf("Opened database successfully\n");
   }
   return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   return rc;
}

void db_init(String latt,String lon,String battery,String speed,String uploaded,String datetime,String batchID,String x, String y, String z){
  sno++;

SPI.begin();
SD.begin();
sqlite3_initialize();


   // Open database 1
   if (openDb("/sd/testinggps.db", &db1))
       return;
   if (openDb("/sd/mdr512.db", &db2))
       return;

     rc = db_exec(db1, "CREATE TABLE IF NOT EXISTS TableA (Sno INTEGER PRIMARY KEY, DeviceDate TEXT , lat TEXT ,longitude TEXT , Speed TEXT , Deviceid TEXT , battery TEXT , upload TEXT , BatchID TEXT ,  XValue TEXT ,YValue TEXT ,ZValue TEXT)");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
  rc = db_exec(db1, "CREATE TABLE IF NOT EXISTS TableB (Sno INTEGER PRIMARY KEY, DeviceDate TEXT , lat TEXT ,longitude TEXT , Speed TEXT , Deviceid TEXT , battery TEXT , upload TEXT , BatchID TEXT,  XValue TEXT ,YValue TEXT ,ZValue TEXT )");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
     rc = db_exec(db1, "DELETE FROM TableA WHERE upload='0'");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

    dbInsert(latt,lon,battery,speed,uploaded,datetime,batchID,x,y,z);
//    dbSelect();

}
void dbSelect(){
  int rc;
  array = doc.to<JsonArray>();
     rc = db_exec(db1, "Select * from TableB order by Sno DESC LIMIT 2 ");
      serializeJsonPretty(array,obj);
   Serial.println(obj);
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
      sqlite3_close(db1);
   sqlite3_close(db2);
}
 
void dbInsert(String latt,String lon,String battery,String speed,String uploaded,String datetime,String batchID,String x,String y,String z)
{
array = doc.to<JsonArray>();
  batchID=batch;
  int rc,rcB;
  String sql= "INSERT INTO TableA(DeviceDate,lat,longitude,Speed,Deviceid,battery,upload,BatchID,XValue,YValue,ZValue) VALUES ('"+(String)datetime+"','"+(String)latt+"','"+(String)lon+"','"+(String)speed+"','12345678','"+(String)battery+"','"+(String)uploaded+"','"+(String)batchID+"','"+(String)x+"','"+(String)y+"','"+(String)z+"')"; 
  Serial.println(sql);
  // Length (with one extra character for the null terminator)
  int str_len = sql.length() + 1; 
  // Prepare the character array (the buffer) 
  char sqlQuery[str_len];
  // Copy it over 
  sql.toCharArray(sqlQuery, str_len);
  String sqlB= "INSERT INTO TableB(DeviceDate,lat,longitude,Speed,Deviceid,battery,upload,BatchID,XValue,YValue,ZValue) VALUES ('"+(String)datetime+"','"+(String)latt+"','"+(String)lon+"','"+(String)speed+"','12345678','"+(String)battery+"','"+(String)uploaded+"','"+(String)batchID+"','"+(String)x+"','"+(String)y+"','"+(String)z+"')"; 
  Serial.println(sqlB);
  // Length (with one extra character for the null terminator)
  int str_lenB = sqlB.length() + 1; 

  // Prepare the character array (the buffer) 
  char sqlQueryB[str_lenB];

  // Copy it over 
  sqlB.toCharArray(sqlQueryB, str_lenB);
  

  rc = db_exec(db1, sqlQuery);
  rcB = db_exec(db1, sqlQueryB);

  if (rc != SQLITE_OK ) {
    sqlite3_close(db1);
    sqlite3_close(db2);
    return;
   }
   if(rcB != SQLITE_OK){
    sqlite3_close(db1);
    sqlite3_close(db2);
    return;
   }
   sql="i";
   Serial.println(sno);
   if(sno>=10){// if a complete 4 min cylce is completed data transfer will take place
        sno=1;
    batch++;
     rc = db_exec(db1, "Select * from TableA WHERE upload=1 order by Sno DESC LIMIT 14 ");
      serializeJsonPretty(array,obj);
   Serial.println(obj);
   int result=1,count=1;
   while(result==1&&count<=2){
    count++;
       sim800init();
     result=postJsonData(obj);
   }
   if(result==0){
    rc =db_exec(db1,"UPDATE TableA SET upload='0' WHERE upload='1'");
   }
   if (rc != SQLITE_OK) {
    
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   }

   sqlite3_close(db1);
   sqlite3_close(db2);
}
