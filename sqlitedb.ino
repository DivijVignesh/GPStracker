   
   sqlite3 *db1;
   sqlite3 *db2;
//   char *zErrMsg = 0;
   int rc;
const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
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

void db_init(String latt,String lon,String battery,String speed,String uploaded,String dat,String time,String batchID){
	sno++;

SPI.begin();
SD.begin();
sqlite3_initialize();


   // Open database 1
   if (openDb("/sd/testinggps.db", &db1))
       return;
   if (openDb("/sd/mdr512.db", &db2))
       return;

     rc = db_exec(db1, "CREATE TABLE IF NOT EXISTS dataTableA (Sno INTEGER PRIMARY KEY, Date TEXT , Time TEXT , lat TEXT ,long TEXT , Speed TEXT , Deviceid TEXT , battery TEXT , upload TEXT , BatchID TEXT )");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
  rc = db_exec(db1, "CREATE TABLE IF NOT EXISTS dataTableB (Sno INTEGER PRIMARY KEY, Date TEXT , Time TEXT , lat TEXT ,long TEXT , Speed TEXT , Deviceid TEXT , battery TEXT , upload TEXT , BatchID TEXT )");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

    dbInsert(latt,lon,battery,speed,uploaded,dat,time,batchID);

}
 
void dbInsert(String latt,String lon,String battery,String speed,String uploaded,String dat,String time,String batchID)
{
  batchID=batch;
  int rc,rcB;
	String sql= "INSERT INTO dataTableA(Date,Time,lat,long,Speed,Deviceid,battery,upload,BatchID ) VALUES ('"+(String)dat+"','"+(String)time+"','"+(String)latt+"','"+(String)lon+"','"+(String)speed+"','12345678','"+(String)battery+"','"+(String)uploaded+"','"+(String)batchID+"')"; 
Serial.println(sql);
// Length (with one extra character for the null terminator)
int str_len = sql.length() + 1; 

// Prepare the character array (the buffer) 
char sqlQuery[str_len];

// Copy it over 
sql.toCharArray(sqlQuery, str_len);

  String sqlB= "INSERT INTO dataTableB(Date,Time,lat,long,Speed,Deviceid,battery,upload,BatchID ) VALUES ('"+(String)dat+"','"+(String)time+"','"+(String)latt+"','"+(String)lon+"','"+(String)speed+"','12345678','"+(String)battery+"','"+(String)uploaded+"','"+(String)batchID+"')"; 
Serial.println(sqlB);
// Length (with one extra character for the null terminator)
int str_lenB = sqlB.length() + 1; 

// Prepare the character array (the buffer) 
char sqlQueryB[str_lenB];

// Copy it over 
sqlB.toCharArray(sqlQueryB, str_lenB);
  

rc = db_exec(db1, sqlQuery);
rcB = db_exec(db1, sqlQueryB);
   if (rc != SQLITE_OK ||rcB != SQLITE_OK) {
    sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }
   sql="i";

   rc = db_exec(db1, "Select * from dataTableB order by Sno DESC LIMIT 2 ");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       sqlite3_close(db2);
       return;
   }

   sqlite3_close(db1);
   sqlite3_close(db2);
     if(sno==3)
  {
    sno=1;
    batch++;
  }
  
  delay(5000);
}
