#include <ESP8266WiFi.h>

#include <WiFiClient.h>

//TCP client
WiFiClient wifiClient;

//config
#define SSID "scream"
#define PASSWD "s741852scream"
#define TCP_IP "192.168.1.8"
#define TCP_PORT 8888

#define deviceID 22

//pin config
int in_msg = D0;
int out_msg = D2;
int wifi_connect = D5;


char _buffer[4];

//Initial
void setup() {

   pinMode(in_msg, OUTPUT);
   pinMode(wifi_connect, OUTPUT);
   pinMode(out_msg, INPUT_PULLUP);

   Serial.begin(9600);
   while (!Serial)
    ;

   Serial.println("Connecting...");
   
   // check for the presence of the shield
   if ( WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");

    while(true);
   }

   // Wait for connection
   while ( WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWD);
    Serial.println("Retry");
    delay(100);
   }
   
   Serial.println("Conneted");
   wifiClient.connect(TCP_IP,TCP_PORT);

   //set client ID
   _buffer[0] = 'C';

   //first char is character, second and third are ID
   String(deviceID, HEX).toCharArray((_buffer + 1), 4);
    
   //send to server
   wifiClient.write((const uint8_t *)_buffer, 4);

   //clear buffer
   for (int i = 1; i < sizeof(_buffer); i ++) {
    _buffer[i] = {0};
   }
   
   
}

static char buffer[32];
static int i = 0;
static unsigned long currentTimer = millis();
void loop() {

  
  //Trigger event by pressed button for 0.2 sec
  int first = digitalRead(out_msg);
  delay(200);
  int second = digitalRead(out_msg);
   
  if(!first){
    if(first != second){
      
      //first char is ID
      String(deviceID, HEX).toCharArray(_buffer, 4);

      //event trigger
      _buffer[2] = '1';
      wifiClient.write((const uint8_t *)_buffer, 4);

      //clear buffer
      for(int i = 1; i < sizeof(_buffer); i++){
        _buffer[i] = {0};
      }
    }
  }

  //If no incoming data, .available() = -1.  
  if (wifiClient.available() > 0) {
    wifiClient.read((unsigned char *)buffer, 32);

    if ( buffer[0] == '1') {
      digitalWrite(in_msg, HIGH);
      Serial.println(buffer[0]);
    }
    else if (buffer[0] == '0') {
      digitalWrite(in_msg, LOW);
    }

    //clear buffer
    for (int i = 1; i < sizeof(buffer); i++) {
      buffer[i] = {0};
    }
  }
  
  if(millis() - currentTimer >= 10000) {
    if(!wifiClient.connected()){

      //reconnect
      setup();
    } else {
      currentTimer = millis();
    }
  }
}
