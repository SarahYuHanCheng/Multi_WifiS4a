#include <ESP8266WiFi.h>

#include <WiFiClient.h>

//TCP client
WiFiClient wifiClient;

//config
#define SSID "server連到的網路名稱"
#define PASSWD "網路密碼"
#define TCP_IP "192.168.1.0" //server被分配到的IP
#define TCP_PORT 8888

#define ID 1 //1~94




//pin config
//input
int analog_value = A0;
int use_analog = D8; //D8 IO, 10k Pull-down, SS
int press_btn = D2;
//output
int in_msg = D0;
int wifi_connect = D5;

 int deviceID;
char _buffer[4];

//Initial
void setup() {

   pinMode(in_msg, OUTPUT);
   pinMode(wifi_connect, OUTPUT);

   pinMode(analog_value, INPUT);
   pinMode(use_analog, INPUT);
   pinMode(press_btn, INPUT_PULLUP);
   
   deviceID = ID + 32;
   Serial.begin(9600);
   while (!Serial)
    ;
   // Wait for connection
   while ( WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWD);
    Serial.print("Retry Connecting...SSID:");
    Serial.println(SSID);
    delay(100);
   }
   Serial.println("Conneted to AP");
   
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
//   String(deviceID, HEX).toCharArray((_buffer + 1), 4);
     _buffer[1]=deviceID;
   //send to server
   wifiClient.write(&_buffer[0], 2);

   //clear buffer
   for (int i = 0; i < sizeof(_buffer); i ++) {
    _buffer[i] = {0};
   }
   
   
}

static char buffer[32];
static int i = 0;
static unsigned long currentTimer = millis();
void loop() {

  _buffer[0]=deviceID;
  //Trigger event by pressed button for 0.2 sec
  int first = digitalRead(press_btn);
  delay(200);
  int second = digitalRead(press_btn);
   
  if(!first){
    //press_btn
    if(first != second){
      _buffer[1] = '1'; 
    }
    //use Analog pin
    if(digitalRead(use_analog)){ 
    int a_value = analogRead(analog_value);
    a_value = a_value/8;
    _buffer[2]=a_value;
    wifiClient.write(&_buffer[0], 4);
    }
  }else if(digitalRead(use_analog)){
    int a_value = analogRead(analog_value);
    a_value = a_value/8; // 1024 -> 128 for ascii
    _buffer[2]=a_value;
    
    // send data
    wifiClient.write(&_buffer[0], 4);
    }
    
    
    //clear buffer
      for(int i = 1; i < sizeof(_buffer); i++){
        _buffer[i] = {0};
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
  
  if(millis() - currentTimer >= 50000) {
    if(!wifiClient.connected()){
      //reconnect
      setup();
    } else {
      currentTimer = millis();
    }
  }
}
