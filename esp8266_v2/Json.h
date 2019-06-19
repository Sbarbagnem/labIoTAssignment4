void JsonSend(){
  delay(10000);
 // while (!Serial) continue;
  DynamicJsonDocument model(capacity_model);
  
  model["id"] = esp_ids;
  model["name"] = "esp2";
  model["link"] = "esp2";
  model["description"] = "System_v2 sensor monitoring";
 
  JsonArray actions = model.createNestedArray("actions");
  actions.add("on");
  actions.add("off");
  
  JsonArray properties = model.createNestedArray("properties");
  properties.add("flame");  
  properties.add("temperature");
  properties.add("vibration");
  
  // serialize in one row
  //serializeJson(model, Serial);
  serializeJson(model, buff);
  Serial.println("conversione:");
  Serial.println(model.as<String>());
  client.publish("Request", buff);  
  
  // Start a new line
  Serial.println();
    
  // serialize in more row  
  serializeJsonPretty(model, Serial);

  Serial.println();
 
  //delay(5000); 
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
  esp_flame = flame();  
  esp_temp = (int)temp(); 
  // pulse rate per minute
  esp_vibration = vibration();
}

void send_data(){
    
  //create JSON
  DynamicJsonDocument data(capacity_data);
    
  // JSON to send via MQTT
  data["id"] = esp_ids;
  JsonObject element1 = data.createNestedObject("flame");
  element1["value"] = esp_flame;
  element1["alert"] = "flame_detected";
  if(!flagFlame){     
     element1["type"] = "normal";
  }else{     
     element1["type"] = "error";
  }
  JsonObject element2 = data.createNestedObject("temperature");
  element2["value"] = esp_temp;
  element2["alert"] = "high_temperature";
  if(!flagTemp){    
    element2["type"] = "normal";
  }else{    
    element2["type"] = "error";
  }
  JsonObject element3 = data.createNestedObject("vibration");
  element3["value"] = esp_vibration;
  element3["alert"] = "vibration_alert";
  if(!flagVibration){
    element3["type"] = "normal";
  }else{
    element3["type"] = "error";
  }
  
  if(flagFlame or flagTemp or flagVibration){
    data["db"] = 1;
    Serial.println("salva errore");
  }else if (millis() -  previousMills_save > interval_save ){
    previousMills_save = millis();  //Tiene in memoria invio
    data["db"] = 1;
    Serial.println("salva 30 sec");
   } else{
    data["db"] = 0;
   }
       
  // serialize in one row
  //serializeJson(model, Serial);
  serializeJson(data, val_sensor);

  // serialize in more row  
  serializeJsonPretty(data, Serial);
  
  client.publish("Values",val_sensor); 
  
  delay(10000);   
}
