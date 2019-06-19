#include <math.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <SPI.h>
#include <WiFi101.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <PubSubClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h"
#include "function.h"
#include "Variables.h"
#include "Database.h"
#include "UpdateSystem.h"
#include "GetSensors.h"
#include "mqtt.h"
#include "http.h"

void setup() {
  
  // inizializza porta seriale
  Serial.begin(9600);
  
  // set up pin ditsance sensor
  pinMode(triggerPort, OUTPUT);
  pinMode(echoPort, INPUT);
  
  // set up pin flame sensor
  pinMode (Led, OUTPUT);
  pinMode (pinFlame, INPUT);
  
  // reset array that store values from sensor
  resetAllArray();

  // set value for connection to net
  WiFi.config(ip, dns, gateway, subnet);

  // set up  broker, port and calback function for MQTT connection
  clientPubSub.setServer(mqtt_addr, 1883);
  clientPubSub.setCallback(callback); // callback function to get message from subscribe topic
  clientPubSub.subscribe("Values");
  clientPubSub.subscribe("Request");
  clientPubSub.subscribe("Ping");

  WiFi.macAddress(mkr_mac);
  //mac_address = mkr_mac[5] + ":" + mkr_mac[4] + ":" + mkr_mac[3] + ":";// + mkr_mac[2] + ":" + mkr_mac[1] + ":" + mkr_mac[0];
  //mac_address.toCharArray(buff_id, 30);
  //server.begin();
}

void loop() {
  //Serial.println(mac_address);

  // connessione alla rete
  connectToNet();

  // connessione a MySQL
  connectToMysql();

  // connessione a broker MQTT
  while(!clientPubSub.connected()){
    reconnect();
  }

  Serial.println("mqtt attivato");

  // metodo per gestire le richieste http in arrivo da node js
  listenForClients();

  // invio dati se richiesto (on/off)
  checkToSend();

  // rimango in ascolto sulla callback per messaggi MQTT
  clientPubSub.loop();

  delay(1000); // invia dati ogni secondo circa

}

void checkToSend(){

  if(onHome){

    // rilevo dai sensori
    Serial.println("rilevo dai sensori");
    getStatusHome();

    // controllo possibili allerte
    Serial.println("aggiorno stato interno");
    updateHome();

    // invio dati 
    Serial.println("invio i dati a node");
    sendStatusHome();

    //Serial.println("aggiorno db per mkr se necessario");
    //updateDbHome();
     
  }

}

void connectToNet(){

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("In attesa di connettersi al SSID: ");
    Serial.println(SECRET_SSID);
  
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.println(".");
      delay(10000);
    }
    
    Serial.println("\nConnesso alla rete.");
  
    server.begin();
    //Serial.println("Server Arduino avviato.");
    
    printWifiStatus();
  }
}

boolean connectToMysql(){
 
  if (conn.connect(server_addr, 3306, user, password)){
    delay(1000);
    //Serial.print("Connesione a Mysql riuscita");
  }
  else {
    //Serial.println("Connessione a Mysql fallita.");
  } 
}

void reconnect() {
  
  while (!clientPubSub.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    
    if (clientPubSub.connect("clientArduino")) {
      Serial.println("connected");
      clientPubSub.subscribe("Values");
      clientPubSub.subscribe("Request");
      clientPubSub.subscribe("Ping");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(clientPubSub.state());
      //Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void printWifiStatus() {

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Indirizzo IP: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("potenza del segnale (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void resetAllArray(){ 
  resetArray(arr_distance);
  resetArray(arr_light);
  resetArray(arr_sound);
  resetArray(arr_wifi);
  resetArray(arr_temperature); 
}
