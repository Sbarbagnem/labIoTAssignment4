/*
 * Gruppo: 
 * - Mirko Rima 793435
 * - Samuele Ventura 793060
 * - Luca Virgilio 794866
 * SYSTEM_V1 (Health monitoring)
 * 
 * ESP8266
 * SENSOR: - VIBRATION
 *         - TILT
 *         - HEART
 *         - TEMPERATURE
 *         - PROXIMITY
 * 1 BUTTON
 */
 
// Library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Change in folder "Arduino\libraries\PubSubClient\src\PubSubClient.h" #define MQTT_MAX_PACKET_SIZE 512 instead 128
#include <ArduinoJson.h>
#include <SPI.h>
#include <OneWire.h>

#include "arduino_secrets.h"
#include "Variables.h"
#include "Functions.h"
#include "mqtt.h"
#include "Json.h"
#include "Alert.h"
     
void setup() {
  
    Serial.begin(9600);
    Serial.setTimeout(2000);
    Serial.println();
    Serial.println("-------------------------------------");
       
    // define switch sensor output interface
    pinMode (call_pin, INPUT); 
    // define tilt sensor output interface
    pinMode (tilt_pin, INPUT) ; 
    // define vibration sensor output interface
    pinMode (vibration_pin, INPUT) ; 
    // define proximity sensor output/input interface
    pinMode(triggerPort, OUTPUT);
    pinMode(echoPort, INPUT);
}

void loop() {
  while (!Serial) continue;  
  //WiFi connection
  WiFiConnect();
  //MQTT and sensor rilevation
  MQTTsetup();
  
  if (millis() -  previousMills > interval ){
    previousMills = millis();  //Tiene in memoria invio
    Serial.println("invia connessione 20 sec");
    EspConnect();
   } 
   
  // send JSON Model 
  if(!accepted) {
    JsonSend();
  }
    
  // Waiting for the start message from MKR1000
  if(invia_dati){
    rileva();
    update_data();
    send_data();
  }   
}

void MQTTsetup(){
  client.loop();
  // Set property for MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  // reconnect to MQTT
  if (!client.connected()) {
    reconnect();
  }
}

//WiFi Connect
// WiFi configuration and connection test

void WiFiConnect(){
  WiFi.config(ip, dns, gateway, subnet);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("In attesa di connettersi al SSID: ");
    Serial.println(SECRET_SSID);
  
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.println(".");
      delay(10000);
    }
    Serial.println("\nConnesso.");
    // Now you are connected
  }
}
// Reconnect function
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("espClient_v1")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("Action");
      client.subscribe("Accept"); 
      client.subscribe("Connect"); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
