#include <Arduino.h>
#include "./../lib/ESP32_Servo/ESP32Servo.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <../.pio/libdeps/esp32dev/AsyncTCP-esphome/src/AsyncTCP.h>
#include <../lib/web_serial/src/WebSerial.h>  

// Constants
const char* ssid = "ESP32_Access_point";
const char* password = "ESP32@123";


AsyncWebServer server(80);
// Called when receiving any WebSocket message

/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}


Servo J0; 
Servo J1;
Servo J2;
Servo J3;
Servo J4;
Servo gripper;

int J0_pwm_pin = 32;  
int J1_pwm_pin = 33;
int J2_pwm_pin = 25;
int J3_pwm_pin = 26;
int J4_pwm_pin = 27;
int gripper_pwm_pin = 14;

int pos = 90;

const uint16_t port = 8090;
const char * host = "192.168.4.2";

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Initialization has been started");

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	gripper.setPeriodHertz(50);    // standard 50 hz servo
  
  J0.attach(J0_pwm_pin, 500, 2400);
  J1.attach(J1_pwm_pin, 500, 2400);
  J2.attach(J2_pwm_pin, 500, 2400);
  J3.attach(J3_pwm_pin, 500, 2400); 
  J4.attach(J4_pwm_pin, 500, 2400);
  gripper.attach(gripper_pwm_pin, 500, 2400);

  pinMode(2, OUTPUT);
  Serial.println("Initializition has been done !");


    // Connect to access point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  WebSerial.begin(&server);
    /* Attach Message Callback */
  WebSerial.msgCallback(recvMsg);
  server.begin();
  WebSerial.print(F("IP address: "));
  WebSerial.println(WiFi.softAPIP());
}

void loop()
  {

  J0.write(pos);
  J1.write(pos);
  J2.write(pos);
  J3.write(pos);
  J4.write(pos);
  gripper.write(pos);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);

  WiFiClient client;
  if (client.connect(host, port)) {
        WebSerial.println("Connected to server successful!");
        client.print("Hello from ESP32!");
        delay(1000);
        return;
    }

  
  


  }

