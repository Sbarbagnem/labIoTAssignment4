#ifndef VARIABLES_H
#define VARIABLES_H

// define string error event
#define errTemp "High temperature"
#define errWifi "Low connection"
#define errIntrusion "Intrusion detected"
#define errFlame "Flame detected"
#define errLight "Low light"
#define errSound "High sound"

// interval to save on db
#define interval_save 30000

// define flag to catch error value from MKR1000
boolean flagWifi = false;
boolean flagTemp = false;
boolean flagIntrusion = false;
boolean flagFlame = false;
boolean flagLight = false;
boolean flagSound = false;

// variables for hardware problem
boolean flagHardTemp = false;
boolean flagHardSound = false;
boolean flagHardLight = false;
boolean flagHardDist = false;


// define count to control error value
int countTemp = 0;
int countWifi = 0;
int countIntrusion = 0;
int countFlame = 0;
int countLight = 0;
int countSound = 0;

// count to write on DB every 12 rilevation
const int countCycle = 12;

// array to store value from MKR1000 and then save on DB only the mean
int arr_distance[countCycle]; 
int arr_light[countCycle]; 
int arr_sound[countCycle]; 
int arr_wifi[countCycle]; 
int arr_temperature[countCycle];

// variable for value from sensor on MKR1000
long distance, light, sound, wifi;
int flame;
float temperature;

// network connection
IPAddress ip(149, 132, 182, 70);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(149, 132, 2, 3);
IPAddress gateway(149, 132, 182, 1);
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

// client used for WebPage
WiFiClient  client;

// MySQL connection
IPAddress server_addr(149, 132, 182 , 206);
IPAddress mqtt_addr(149,132,182,132);
char user[] = MYSQL_USER;
char password[] = SECRET_MYSQL_PASS;
WiFiClient  clientDB;
MySQL_Connection conn((Client *)&clientDB);

// MQTT connection
WiFiClient  clientArduino;
PubSubClient clientPubSub(clientArduino);

// to set arduino as WebServer listening on port 80
WiFiServer server(80);

// variabili per il sensore di temperatura con grove
// B value of the thermistor
const int B = 4275;
// R0 = 100k
const int R0 = 100000;
const int pinTempSensor = A0;

//sensore di suono
const int pinAdc = A5;

// sensore di prossimità
int triggerPort = 6;
int echoPort = 7;

//flame sensor KY-026
// definisci l'interfaccia del LED
int Led = 13 ;
// definisci sensore digitale
int pinFlame = 1;
// define la variabile numerica val
int val ;
// variabile per leggere il valore analogico
float sensor;

int buzzer = 3;
int ledAllarm = 4; 
int ledAllarmEsp = 5;

int pinLight = A3;

// variable to write or not on DB
boolean writeDB = false;

String inData = "";
boolean onHome = false;

String jsonString;

char buff[512];

int nodeId[50];

char serverNode[] = "149.132.182.132";  // to change
int port = 8080;

HttpClient http = HttpClient(client, serverNode, port);

String postMessage = "";

String id_res;
char char_id_res[30];

char msg[300];
char body_post[50];
const int id_mkr = 1;

boolean body = false;

byte mkr_mac[6];
char buff_id[30];
String mac_address = "";

StaticJsonDocument<1024> value;
StaticJsonDocument<1024> req;
StaticJsonDocument<80> id_ping;
StaticJsonDocument<1024> json_value;
StaticJsonDocument<1024> node;

long previousMills_save = 0;

#endif
