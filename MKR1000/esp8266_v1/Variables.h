#ifndef VARIABLES_H
#define VARIABLES_H 


// WiFi settings
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

IPAddress ip(149, 132, 182, 63);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(149, 132, 2, 3);
IPAddress gateway(149, 132, 182, 1);
IPAddress mqtt_server(149, 132, 182, 132); 

WiFiClient espClient;
// Variable to set esp8266 as client to publish on broker
PubSubClient client(espClient);

// buffer JSON - MQTT -> Model
char buff[512];

// buffer JSON - MQTT -> Data
char val_sensor[512];
char ping[256];

//variabile start/off send by MKR1000
boolean invia_dati = false;

//variabile accepeted
boolean accepted = false;

//Capacity of JSON
//Model
int capacity_model = 512;
// Data
int capacity_data = 512;

boolean sleep;

char msg_id_buf[3];
// ------------------------------- SENSOR -------------------------------------

//Temperature sensor
//For conversion of raw data to C
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
byte i;
byte present = 0;
byte data[12];
byte addr[8];
double val1;
double Fract_double;
OneWire ds(D3);  // on pin 10
int esp_temp;

// Vibration
int vibration_pin = D2; // define the percussion Sensor Interface
int val2; // define numeric variables val
int esp_vibration;

//Tilt 
int tilt_pin = D6; // define the percussion Sensor Interface
int val4; // define numeric variables val
int esp_tilt;

//Heartbeat
int heart_pin = A0;
double alpha = 0.75;
double change = 0.0;
double minval = 0.0;
int esp_heart;
double val5;

//Proximity 
long esp_distance;
int triggerPort = D5;
int echoPort = D7;
long val6;

//Switch button
int call_pin = D1; // define the key switch sensor interface
int esp_call;
int val7;

// ----------------------------------------------------------------------------

// GLOBAL ID OF ESP
String esp_ids = WiFi.macAddress();
char esp_id_connect[2]; 


//tempo per la connessione (20 sec)
#define interval 20000
long previousMills = 0;  

#define interval_save 30000
long previousMills_save = 0;  

// connection 

int esp_connect_array[50];
int esp_check_array[50];
int cont = 0;

DynamicJsonDocument command_action(1024);
char buff_id[30];

// define flag to catch error value from ESP8266
boolean flagVibration = false;
boolean flagTilt = false;
boolean flagHeart = false;
boolean flagDead = false;
boolean flagDistance = false;
boolean flagCall = false;

boolean flagTemp = false;

// Count esp
int countHeart = 0;
int countTemp = 0;

#endif
