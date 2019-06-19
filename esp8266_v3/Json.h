void JsonSend(){
  delay(10000);
 // while (!Serial) continue;
  DynamicJsonDocument model(capacity_model);
  
  model["id"] = esp_ids;
  model["name"] = "esp3";
  model["link"] = "esp3";
  model["description"] = "System_v3 sensor monitoring";
 
  JsonArray actions = model.createNestedArray("actions");
  actions.add("on");
  actions.add("off");
  
  JsonArray properties = model.createNestedArray("properties");
  properties.add("temperature");

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
  esp_temp = (int)temp(); 
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
    
  if(flagTemp){
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
