#include <ArduinoJson.h>
#include "tracker.h";
StaticJsonDocument<5000> doc; 
JsonArray array ;
String obj;

sqlite3 *db1;
sqlite3 *db2;

int count=1;
int result=1;


void Task1code(){ //void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  const char APN[] = "www";//internet
const char URL[] = "http://103.40.48.140:85/InsertPower";
const char CONTENT_TYPE[] = "application/json";
const char HEADERS[] = "keep-alive";


SIM800L* sim800l;
  for(;;){
        digitalWrite(2, HIGH);
    delay(100);
        digitalWrite(2, LOW);
     delay(500);
     
    if(isUploading==0){
      
    Serial.print("Calling Transmit");
    digitalWrite(2, HIGH);
    
    delay(500);
    while(result==1&&count<=2){
      count++;
//      delay(20000);
//      result=0;
//      break;
      sim800init();
      result=postJsonData(obj);
      
       }
     
     digitalWrite(2, LOW);
    delay(500);
    isUploading=1;
  } 
//      esp_task_wdt_feed();
    
//  taskYIELD();
  }
//  vTaskDelay(10000/portTICK_PERIOD_MS);
}


   char *zErrMsg = 0;

int rc;
const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName){
  JsonObject object;
  if((String)azColName[0]!="batch")
    object= doc.createNestedObject(); // JSON objects for NAME and VALUE of the counter
int i;
Serial.printf("%s: ", (const char*)data);
for (i = 0; i<argc; i++){
  if((String)azColName[i]=="batch"){
    Serial.println(argv[i]);
    batch=((String)argv[i]).toInt();
    Serial.print("BatchID");
    Serial.print(batch);
    Serial.println();
    if(getBatchForUpload==0)
    {
      getTransmitData=batch;
      break;
    }else{
    
    batchID=batch+1;
    break;
    }
  }
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

//char *zErrMsg = 0;
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
       rc = db_exec(db1, "CREATE TABLE IF NOT EXISTS BatchIDtable (Sno INTEGER PRIMARY KEY, batch TEXT, isUploaded TEXT )");
       if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
          rc = db_exec(db1, "Select batch from BatchIDtable order by sno desc LIMIT 1");
       if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }


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
//   rc = db_exec(db1, "DELETE FROM TableA WHERE BatchID='1'");
//   if (rc != SQLITE_OK) {
//       sqlite3_close(db1);
//       sqlite3_close(db2);
//       return;
//   }
   if(result==0){
   String deleteQ="DELETE FROM TableA WHERE BatchID='"+(String)(batch-1)+"'";
   int le= deleteQ.length()+1;
   char del[le];
   deleteQ.toCharArray(del,le);
     rc = db_exec(db1,del );//DELETE FROM TableA WHERE upload='0'   DELETE FROM BatchTable
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   }

    dbInsert(latt,lon,battery,speed,uploaded,datetime,batchID,x,y,z);
//    dbSelect();

}

 
void dbInsert(String latt,String lon,String battery,String speed,String uploaded,String datetime,String batchID,String x,String y,String z)
{

array = doc.to<JsonArray>();
if(result==0){
  updateBatch(getTransmitData);
}
if(isUploading!=0)
 getUploadData(); 
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
   Serial.print("Sno:");
   Serial.println(sno);
   if(sno>=7){
//    isUploading=0;
    sno=1;
    batch++;
    String sql1="INSERT INTO BatchIDtable(batch , isUploaded) VALUES ('"+(String)batch+"','1')";
    int len= sql1.length()+1;
    char query[len];
    sql1.toCharArray(query,len);
   int rc3 = db_exec(db1,query);
   if (rc3 != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   sql1="i";
   }
   

   sqlite3_close(db1);
   sqlite3_close(db2);
}
void dbSelect(int number){
     String sqlSelect="Select * from TableA WHERE BatchID='"+(String)number+"'";
   int leng= sqlSelect.length()+1;
   char charSelect[leng];
   sqlSelect.toCharArray(charSelect,leng);

     int r = db_exec(db1, charSelect);
//     if(array.isNull()==true){
//      isUploading=1;
//     }
//     else {
//     
//     isUploading=0;}
      serializeJsonPretty(array,obj);
   Serial.println(obj);
   
   if (r != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

}
void updateBatch(int number){
       String sqlSelect="UPDATE BatchIDtable SET isUploaded='0' WHERE batch='"+(String)number+"'";
   int leng= sqlSelect.length()+1;
   char charSelect[leng];
   sqlSelect.toCharArray(charSelect,leng);

     int r = db_exec(db1, charSelect);
//      serializeJsonPretty(array,obj);
//   Serial.println(obj);
   if (r != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
  
}
void getUploadData()
{
  isUploading=0;
  getBatchForUpload=0;
  
     int r = db_exec(db1, "Select batch from BatchIDtable WHERE isUploaded='1' ORDER BY sno LIMIT 1");
    
   if (r != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   getBatchForUpload=1;
   dbSelect(getTransmitData);
  
}
