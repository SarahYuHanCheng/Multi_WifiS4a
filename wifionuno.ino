#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

//config
#define SSID "scream"
#define PASSWD "s741852scream"
#define TCP_IP "192.168.1.8"
#define TCP_PORT 8888

#define deviceID 1

// hardware pin config
int wifi_connect = 5;

char _buffer[3];


//Initial
void setup() {
  Serial.begin(9600);
  while(!Serial)
    ;
  Serial.println("Connecting...");

  // check for the presence of the shield
  if ( WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
  }
  while( WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWD);
    Serial.println("Retry");
    delay(100);
  }
  Serial.println("Connected");
  wifiClient.connect(TCP_IP,TCP_PORT);

  //send to server
  _buffer[0] = 'X'; 
  String(deviceID, HEX).toCharArray((_buffer + 1), 2);
  wifiClient.write((const uint8_t *)_buffer, 2);

  // clear buffer
  for (int i = 1; i < sizeof(_buffer); i++) {
    _buffer[i] = {0};
  }
  

}

static char buffer[32];
static unsigned long currentTimer = millis();
static int char_count = 0;

void loop() {
  
  //incoming data format: [M][action]
  if ((char_count = Serial.available()) > 0) {
    buffer[0] = 'M';
    for(int i = 1; i < char_count + 1; i++){
      buffer[i] = Serial.read();
    }
    buffer[char_count + 2] = '\0';

    //clear buffer
    for(int i = 1; i < sizeof(buffer); i++) {
      buffer[i] = {0};
    }

    //If incoming serial buffer contains substring "end".
    if (strstr(buffer, "end")) {
      wifiClient.stop();
      while (1)
        ;
    }
    
  }
  
  //Incoming data from server
  if ((char_count = wifiClient.available()) > 0) {
   
    currentTimer = millis();
    wifiClient.read((unsigned char *)buffer, 32);
    Serial.println(buffer);
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
