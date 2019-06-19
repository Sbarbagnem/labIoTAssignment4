void executeQuery (char* query) {

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);


  // Execute the query
  cur_mem->execute(query);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
}

void createTable(char* json) {

  char queryStart [200];
  char queryAdd [200];
  char cTable [] = "CREATE TABLE iot01.`%s` ("
                   "`id` INT NOT NULL AUTO_INCREMENT,"
                   "`data` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,PRIMARY KEY ( `id` ))";

  // allocate the memory for the document
  const size_t CAPACITY = JSON_OBJECT_SIZE(300);
  StaticJsonDocument<CAPACITY> doc;
 
  deserializeJson(doc, json);

  // extract the data
  JsonObject object = doc.as<JsonObject>();

  String field0 = object["id"].as<String>();
  field0.toLowerCase();
  Serial.print("campo id: ");
  char field0cc[50];
  field0.toCharArray(field0cc,50); 
  Serial.println(field0cc);

  sprintf(queryStart, cTable, field0cc);
  Serial.println(queryStart);
  executeQuery(queryStart);


  // predecessor serve a concatenare i nuovi attributi
  const char* predecessor = "data";

  Serial.println("inizio inserimento dati");
  // using C++11 syntax (preferred):
  int arraySize =  object["properties"].size();

  for (int i = 0; i < arraySize; i++) {
    const char* field1 = object["properties"][i];
    //Serial.println(field1);
    char insert [] = "ALTER TABLE iot01.`%s` ADD COLUMN %s INT AFTER %s";
    sprintf(queryAdd, insert, field0cc, field1, predecessor);
    Serial.println(queryAdd);
    executeQuery(queryAdd);
    //assegno nuovo predecessore
    predecessor = field1;
  }
}


void dropTable(char* field0) {

  char tempQuery[50];
  char tempCom[] = "DROP TABLE iot01.`%s`";

  sprintf(tempQuery, tempCom, field0);
  Serial.println(tempQuery);
  executeQuery(tempQuery);

}

void writeMultiToDb(char* json) {

  const size_t CAPACITY = JSON_OBJECT_SIZE(300);
  StaticJsonDocument<CAPACITY> doc;

  deserializeJson(doc, json);
  JsonObject object = doc.as<JsonObject>();
  serializeJsonPretty(object, Serial);
  // lunghezza query
  char query[512];
  char query2[512];
  char q1[100];

  // prima parte della query
  char startq1[] = "INSERT INTO iot01.`%s` (";
  String field0 = object["id"].as<String>();
   Serial.print("campo id: ");
  char field0c[50];
  field0.toCharArray(field0c,50); 
  Serial.println(field0c);
  // aggiungo il nome della tabella alla query
  sprintf(q1, startq1, field0c);
  // inizio a costruire la prima marte della query
  strcpy(query, q1);
  int cont1 = strlen(q1);

  //costruisco iterativamente la prima parte e la seconda parte della query
  char q2 [] = "VALUES (";
  strcpy(query2, q2);

  // definisco un contatore per contare le coppie oggetto valore
  int cont = 0;


  const char* virgola = ",";

  // ciclo su tutto il json per contare le coppie
  for (JsonPair p : object) {
    cont = cont + 1;
  }

  int contMax = cont;
  // ciclo su tutto il json per creare la query
  for (JsonPair p : object) {
    Serial.println("-----------------------");
    const char* key = p.key().c_str();
    Serial.println(key);
    Serial.print("\t");
    JsonVariant value = p.value();

    /*******************  new  ****************/
    //ottengo l'oggetto del sensore
    JsonObject jSensor = value.as<JsonObject>();
    String valS = jSensor["value"];
    char valSc[10];
    valS.toCharArray(valSc,10); 
    Serial.println(valSc);
    /*
       //converto il valore in string
       // const char* valS = value.as<char*>();
    */

    // Serial.print(valS);
    // cont != 1 poichè l'ultimo jsonpair è db
    if (cont != contMax && cont != 1) {

      strcat(query, key);
      strcat(query2, valSc);
      //aggiungo una virgola poichè ci sono altri sensori
      // cont > 2 poichè esiste anche il campo db
      if (cont > 2) {
        strcat(query2, virgola);
        strcat(query, virgola);
      }
      Serial.println(query);
      Serial.println(query2);

    }
    cont = cont - 1;
  }

  char qEnd [] = ")";

  strcat(query, qEnd);
  strcat(query, " ");
  strcat(query2, qEnd);

  strcat(query, query2);
  Serial.println("query finale:");
  Serial.println(query);
  executeQuery(query);
}

void updateDbHome() {

  char to_db[1024];

  if (writeDB) {
    postMessage.toCharArray(to_db, 1024);
    writeMultiToDb(to_db);
    writeDB = false;
  } else if (millis() -  previousMills_save > interval_save ) {
    postMessage.toCharArray(to_db, 1024);
    writeMultiToDb(to_db);
  }
}
