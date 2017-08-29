#include <ESP8266WiFi.h>

#include <WiFiClient.h>

//TCP client
WiFiClient wifiClient;

//config 修改這裡即可
#define SSID "server連到的網路名稱"
#define PASSWD "網路密碼"
#define TCP_IP "server被分配到的IP" //例:192.168.1.0
#define TCP_PORT 8888
#define deviceID 23 //1~31 以16進位轉換,所以此為23，server會顯示17


//pin config 以下不必更動
int in_notify = D0;
int out_msg = D4;
int wifi_connect = D5;


char _buffer[4];

//Initial
void setup() {

   pinMode(in_notify, OUTPUT);
   pinMode(wifi_connect, OUTPUT);
   pinMode(out_msg, INPUT_PULLUP);

   Serial.begin(9600);
   while (!Serial)
    ;
   // Wait for connection
   while ( WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWD);
    Serial.print("Retry Connecting...SSID:");
    Serial.println(SSID);
    delay(100);
   } Serial.println("Conneted to AP");
   
   // check for the presence of the shield
   if ( WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
   }
   
   while(!wifiClient.connect(TCP_IP,TCP_PORT)){
      Serial.print("Retry Connecting...Server port:");
      Serial.println(TCP_PORT);
    };
    Serial.println("Conneted to Server");
   
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
      digitalWrite(in_notify, HIGH);
      Serial.println(buffer[0]);
    }
    else if (buffer[0] == '0') {
      digitalWrite(in_notify, LOW);
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
