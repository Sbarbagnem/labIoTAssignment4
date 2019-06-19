void callback(char* topic, byte* payload, unsigned int length) {
  
  // nel messaggio che mi arriva dall'esp con i valori:
  //  * i valori devono essere inviati come json
  //  * ci deve essere anche l'id per poter aggiornare modello interno a nodejs
  // esempio
  /*
  {"id": id,
    "temperature": {value: value_sensor, type: error/normal, error_string: "errore"},
    "flame": {value: value_sensor, type: error/normal, error_string: "errore"},
    ...}
  */
  
  if (strcmp(topic,"Values")==0){
  
    char  message[length];
  
    for (int i=0;i<length;i++) {
      message[i] = (char)payload[i];
      Serial.print(message[i]);
    }

    Serial.println();

    // nel payload dovrei avere un json da deserializzare? oppure posso inviare diretto a node js?

    deserializeJson(value, payload, length);

    postMessage = value.as<String>();

    http.post("/sensor/value", "application/json", postMessage);

    int statusCode = http.responseStatusCode();
  
    String response = http.responseBody();
  
    Serial.print("Status code: ");
  
    Serial.println(statusCode);
  
    Serial.print("Response: ");
  
    Serial.println(response);

    if(value["db"] == 1)
      writeMultiToDb(message);
  } 

  if (strcmp(topic,"Request")==0){

    char  message[length];
  
    for (int i=0;i<length;i++) {
      message[i] = (char)payload[i];
      Serial.print(message[i]);
    }

    deserializeJson(req, payload, length);

    serializeJsonPretty(req, Serial);
   
      // mando struttura a server con post a /updateNetwork/createNode
      Serial.println("making POST request");  
      
      postMessage = req.as<String>();
  
      http.post("/updateNetwork/createNode", "application/json", postMessage);  
      int statusCode = http.responseStatusCode();    
      String response = http.responseBody();
      Serial.print("Status code: ");
      Serial.println(statusCode);
      Serial.print("Response: ");
      Serial.println(response);

      postMessage = "";
      
      id_res = req["id"].as<String>();
      id_res.toCharArray(char_id_res,30);

      // mando accept a nodo
      clientPubSub.publish("Accept", char_id_res);

      createTable(message);
   
  }

  if (strcmp(topic,"Ping")==0){
    deserializeJson(id_ping, payload, length);
    serializeJsonPretty(id_ping, Serial);
    postMessage = id_ping.as<String>();
    http.post("/node/check", "application/json", postMessage);
    int statusCode = http.responseStatusCode();  
    String response = http.responseBody();  
    Serial.print("Status code: ");  
    Serial.println(statusCode);  
    Serial.print("Response: ");  
    Serial.println(response);
  }
}
