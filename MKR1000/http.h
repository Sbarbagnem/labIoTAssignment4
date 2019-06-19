/*void listenForClients() {


  client = server.available();   

  if (client) {                             

    //Serial.println("new client");          

    String currentLine = "";               

    while (client.connected()) {            

      if (client.available()) {             

        char c = client.read();             

        Serial.write(c);                    

        if (c == '\n') {                    

          

          if (currentLine.length() == 0) {

            break;

          }

          else {      

            currentLine = "";

          }

        }

        else if (c != '\r') {   

          currentLine += c;      

        }

        if(currentLine.endsWith("POST /action")){

          //char action[50];
          int ctr = 0;
          String data = "";
          String ptr = "";
          // dovrebbe esserci il body della post ricevuta dall'mkr
          while(client.available()) { 
            int c = client.read();
            //Serial.write(c);
            body_post[ctr] = c;
            ctr++;
            //strcpy(msg, result.c_str());
            //Serial.println(msg);
            //clientPubSub.publish("request/values", msg);
            Serial.println();
          }
          Serial.println();
          //Serial.println("dati da node");
          //Serial.println(body_post);
          ptr = strtok (body_post, "{");
          Serial.println("json is ");
          while (ptr != NULL)
          {
            ptr = strtok (NULL, "}");   // look for .
            if(ptr != ""){
              data = '{' + ptr + '}';
              //Serial.println(data);
              const char *data_complete = data.c_str();
              Serial.println(data_complete);
              deserializeJson(node, data_complete);
              if(node["id"] != "1"){
                Serial.println("mando azione a esp");
                clientPubSub.publish("Action", data_complete);
              } 
              else {
                if(node["action"] == "onMkr"){
                  Serial.println("ricevuta azione onMKr");
                  onHome = true;
                }
                else{
                Serial.println("ricevuta azione offMkr");
                  onHome = false;
                }
              }
            }
          }
          ctr = 0;
          data = "";
          ptr = "";
          break;
        }

    }

    // close the connection:

    //client.stop();

    Serial.println("client disconnected");

  }

}

}*/

void listenForClients() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          char tempBuffer[200];
          char tempBuffer2[200];
          char singleChar;
          int i = 0;
          // Here is where the POST data is.
          while (client.available())
          {
            singleChar = client.read();
            tempBuffer[i] = singleChar;
            tempBuffer2[i] = singleChar;
            i++;
            //Serial.write(client.read());
          }
          Serial.println("0000");

          Serial.println(tempBuffer);
          DeserializationError error = deserializeJson(node, tempBuffer);
          if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
          }

          if (node["id"] == "1") {
            if(node["action"] == "onMkr"){
              Serial.println("ricevuta azione onMKr");
              onHome = true;
            }
            else{
            Serial.println("ricevuta azione offMkr");
              onHome = false;
            }
          }
          else {
            Serial.println("mando azione a esp");
            clientPubSub.publish("Action", tempBuffer2);            
          }

          Serial.println("Sending response");
          // send a standard http response header
          /*client.println("HTTP/1.0 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("ok");*/
          client.stop();
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    Serial.println("Client Disconnected");
  }
}

// mando http con datiMKR a nodejs
void sendStatusHome(){

  DynamicJsonDocument cose(1024);

  JsonObject json_values = cose.to<JsonObject>();
  //prima controllo i flag, in base al valore dei flag mando dato sensore o allerta specifica
  json_values["id"] = id_mkr;

  JsonObject temperature_obj = json_values.createNestedObject("temperature");
  temperature_obj["value"] = int(temperature);
  temperature_obj["alert"] = errTemp;
  if(flagTemp)
    temperature_obj["type"] = "error";
  else
    temperature_obj["type"] = "normal";

  JsonObject flame_obj = json_values.createNestedObject("flame");
  flame_obj["value"] = flame;
  flame_obj["alert"] = errFlame;
  if(flagFlame)  
    flame_obj["type"] = "error";
  else
    flame_obj["type"] = "normal";

  JsonObject wifi_obj = json_values.createNestedObject("wifi");
  wifi_obj["value"] = wifi;
  wifi_obj["alert"] = errWifi;
  if(flagWifi)  
    wifi_obj["type"] = "error";
  else
    wifi_obj["type"] = "normal";

  JsonObject light_obj = json_values.createNestedObject("light");
  light_obj["value"] = light;
  light_obj["alert"] = errLight;
  if(flagLight)
    light_obj["type"] = "error";
  else
    light_obj["type"] = "normal";

  JsonObject sound_obj = json_values.createNestedObject("sound");
  sound_obj["value"] = sound;
  sound_obj["alert"] = errSound;
  if(flagSound)
    sound_obj["type"] = "error";
  else
    sound_obj["type"] = "normal";

  JsonObject distance_obj = json_values.createNestedObject("distance");
  distance_obj["value"] = distance;
  distance_obj["alert"] = errIntrusion;
  if(flagIntrusion)
    distance_obj["type"] = "error";
  else
    distance_obj["type"] = "normal";
    
  if(writeDB){
    json_values["db"] = 1;
    Serial.println("salva errore");
  }else if (millis() -  previousMills_save > interval_save ){
    previousMills_save = millis();  //Tiene in memoria invio
    json_values["db"] = 1;
    Serial.println("salva 30 sec");
  } else{
    json_values["db"] = 0;
  }
    
  serializeJsonPretty(json_values, Serial);
  postMessage = cose.as<String>();
  //serializeJson(cose, buff_values, 512);
  Serial.println(postMessage);
  http.post("/sensor/value", "application/json", postMessage); 
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  char to_db[1024];
  
  if (json_values["db"] == 1) {
    Serial.print("invio db = 1");
    postMessage.toCharArray(to_db, 1024);
    writeMultiToDb(to_db);
  } 
  postMessage = "";
}
