// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  if (strcmp(topic,"Action") == 0){ 
   Serial.print("Message arrived from topic: "); 
   Serial.println(topic); 

   char  message[length]; 
 
   for (int i=0;i<length;i++) { 
      message[i] = (char)payload[i]; 
      Serial.print((char)payload[i]); 
    } 
 
   Serial.println(); 
    
   deserializeJson(command_action, payload, length);
   
   if(command_action["id"] == esp_ids){
    if(command_action["action"] == "on"){
      invia_dati = true;
      Serial.print(invia_dati);
    }
    if(command_action["action"] == "off"){
      invia_dati = false; 
      Serial.print(invia_dati);
    }
    if(command_action["action"] == "on_sleep_mode"){
       sleep = true;
    }
    if(command_action["action"] == "off_sleep_mode"){
      sleep = false; 
    }
   }
   
   
      
  }
   if (strcmp(topic,"Accept") == 0){ 
   Serial.print("Message arrived from topic: "); 
   Serial.println(topic); 
   
   char  message[length]; 
 
   for (int i=0;i<length;i++) { 
      message[i] = (char)payload[i];  
    } 
    
    Serial.println();
    //Change with number of esp_id
    esp_ids.toCharArray(buff_id,30);  
    Serial.println(message);
    Serial.println(buff_id);
    if (strcmp(message,buff_id) == 0){ 
      accepted = true; 
    }
 
  }

  if (strcmp(topic,"Connect") == 0){ 
   Serial.print("Message arrived from topic: "); 
   Serial.println(topic); 
   
   char  message[length]; 
 
   for (int i=0;i<length;i++) { 
      message[i] = (char)payload[i];  
    } 
    
    Serial.println();
    String id_m = String(message[0]);
    int id = id_m.toInt();
    Serial.println(id);
    esp_connect_array[id] = id;    
  }
}
