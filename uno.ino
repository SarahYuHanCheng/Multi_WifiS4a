//used for communicating with wifi
#include <SoftwareSerial.h>

//SoftwareSerial(rxPin, txPin) : rx stands for receive, tx stands for transmit
SoftwareSerial wifiToServer(3, 2);

//custom hardware pins
byte rx = 0;  //AO
byte tx = 1;  //A1
byte msg_in_pin = 2;   //incoming wifi tx
byte msg_out_pin = 3;  //incoming wifi rx

//Save the last two incoming ID
int msg_ID = 0;
int old_msg_ID = 0;


typedef enum {
  digital
} pinType;

typedef struct pin {
  pinType type;       //type of pin
  int state;          //state of output
};

pin arduinoPins[14];  //array of struct holding 0-13 pins info
static int char_count = 0;
unsigned long lastDataReceivedTime = millis();
char output[10];

void setup(){
  //initial UNO
  Serial.begin(38400);
  Serial.flush();

  //initial Wifi
  wifiToServer.begin(9600);
  wifiToServer.flush();
  delay(10);
  
  //set config of pins
  configurePins();
}

void loop(){
  recvWifi();
  delay(10);
}

// receive incoming data
void recvWifi(){
  if((char_count = wifiToServer.available()) > 0){
    delay(10);
    for(int i = 0; i < char_count; i++){
      output[i] = wifiToServer.read();
      
      if(i == 1) {

        //String to HEX
        msg_ID = strtoul(output, NULL , 16);
      }

    }

    delay(30);

    // Check incoming ID, same ID can't inturrept itself in 10 sec.
    if(msg_ID != old_msg_ID){
      old_msg_ID = msg_ID;
      lastDataReceivedTime = millis();
      scratchBoardSensorReport(msg_in_pin, old_msg_ID);
    } else {
      if(10000 < (millis() - lastDataReceivedTime)){
        lastDataReceivedTime = millis();
	scratchBoardSensorReport(msg_in_pin, old_msg_ID);
      }
      
    }

    //clear buffer
    for(int i = 1 ; i < sizeof(output); i++){
      output[i] = {0};
    }
  }
  delay(20); 
  
}

//initial config of pins
void configurePins(){
  for(int index = 7; index < 14;index++){
    arduinoPins[index].type = digital;
    arduinoPins[index].state = 0;
    pinMode(index, OUTPUT);
    digitalWrite(index, LOW);
  }
}

//PicoBoard protocol, 2 bytes per sensor
void scratchBoardSensorReport(byte sensor, int value){
  Serial.write( B10000000
                  | ((sensor & B1111) << 3)
                  | ((value >> 7) & B111));
  Serial.write( value & B1111111);
}


