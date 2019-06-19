void JsonSend(){
  delay(10000);
 
  DynamicJsonDocument model(capacity_model);
  
  model["id"] = esp_ids;
  model["name"] = "esp1";
  model["link"] = "esp1";
  model["description"] = "Health sensor monitoring";
 
  JsonArray actions = model.createNestedArray("actions");
  actions.add("on");
  actions.add("off");
  
  JsonArray properties = model.createNestedArray("properties");
  properties.add("temperature");
  properties.add("vibration");
  properties.add("tilt");
  properties.add("heartbeat");    
  properties.add("distance");
  properties.add("called");
  
  // serialize in one row
  serializeJson(model, buff);
  Serial.println("conversione:");
  Serial.println(model.as<String>());
  client.publish("Request", buff);  
  
  // Start a new line
  Serial.println();
    
  // serialize in more row  
  serializeJsonPretty(model, Serial);

  Serial.println();

}

void EspConnect(){
  DynamicJsonDocument esp_con(256);
  // JSON to send via MQTT
  esp_con["id"] = esp_ids;
    
  // serialize in one row
  serializeJson(esp_con, ping);
  
  client.publish("Ping",ping); 
  
}

void rileva(){
  //Take value
  esp_call = call(); 
  esp_temp = (int)temp(); 
  // pulse rate per minute
  esp_heart = (int)(heart()/12); 
  esp_tilt = tilt();
  esp_vibration = vibration();
  esp_distance = distance();
}

void send_data(){
  //create JSON
  DynamicJsonDocument data(capacity_data);
  // JSON to send via MQTT
  data["id"] = esp_ids;
  JsonObject element1 = data.createNestedObject("temperature");
  element1["value"] = esp_temp;
  element1["alert"] = "high_temperature";
  if(!flagTemp){    
    element1["type"] = "normal";
  }else{    
    element1["type"] = "error";
  }
  JsonObject element2 = data.createNestedObject("vibration");
  element2["value"] = esp_vibration;
  element2["alert"] = "vibration_alert";
  if(!flagVibration){
    element2["type"] = "normal";
  }else{
    element2["type"] = "error";
  }
  JsonObject element3 = data.createNestedObject("tilt");
  element3["value"] = esp_tilt;
  element3["alert"] = "tilt_alert";
  if(!flagTilt){
    element3["type"] = "normal";
  }else{
    element3["type"] = "error";
  }
  JsonObject element4 = data.createNestedObject("heartbeat");
  element4["value"] = esp_heart;
  if(flagDead){
    element4["alert"] = "dead_alert";
  }else{
    element4["alert"] = "heartbeat_alert";
  }
  if(!flagHeart and !flagDead){
    element4["type"] = "normal";
  }else{
    element4["type"] = "error";
  }
  JsonObject element5 = data.createNestedObject("distance");
  element5["value"] = esp_distance;
  element5["alert"] = "distance_alert";
  if(!flagDistance){
    element5["type"] = "normal";
  }else{
    element5["type"] = "error";
  }
  JsonObject element6 = data.createNestedObject("called");
  element6["value"] = esp_call;
  element6["alert"] = "call_alert";
  if(!flagCall){
    element6["type"] = "normal";
  }else{
    element6["type"] = "error";
  }
  
  if(flagTemp or flagVibration or flagTilt or flagHeart or flagDead or flagDistance or flagCall){
    data["db"] = 1;
    Serial.println("salva errore");
  }else if (millis() -  previousMills_save > interval_save ){
    previousMills_save = millis();  //Tiene in memoria invio
    data["db"] = 1;
    Serial.println("salva 30 sec");
   } else{
    data["db"] = 0;
   }
   
  serializeJson(data, val_sensor);

  // serialize in more row  
  serializeJsonPretty(data, Serial);
  
  client.publish("Values",val_sensor); 
  
  delay(10000); 
  // Publish (send data) in a topic so that MKR1000 can receive the sensor values
  
}
