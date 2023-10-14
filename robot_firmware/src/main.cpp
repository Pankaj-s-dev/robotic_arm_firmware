#include <Arduino.h>
#include "./../lib/ESP32_Servo/ESP32Servo.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <../.pio/libdeps/esp32dev/AsyncTCP-esphome/src/AsyncTCP.h>
#include <../lib/web_serial/src/WebSerial.h>  
using namespace std;
#include <iostream>
#include <string.h>

// Constants
const char* ssid = "ESP32_Access_point";
const char* password = "ESP32@123";


AsyncWebServer server(8090);
// Called when receiving any WebSocket message



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

int J0_POS, J1_POS, J2_POS, J3_POS, J4_POS, J5_POS;
String line = "";
const char *incomming = "";
const char *execution_type = "";

const uint16_t port = 8090;
const char * host = "192.168.4.2";
bool connection_complted = false;

u_int32_t prev_millis = 0;
u_int32_t current_millis = 0;

void incoming_data_reset(){
  J0_POS = J1_POS = J2_POS = J3_POS = J4_POS = J5_POS = 0;
  line = "";
  incomming = "";
  execution_type = "";
}

void jog_executor(int pos, int servo_speed, int servo_id)
{
  switch (servo_id)
  {
  case 1:
    J0.write(pos);
    break;
  
  case 2:
    J1.write(pos);
    break;
  case 3:
    J2.write(pos);
    break;
  case 4:
    J3.write(pos);
    break;
  case 5:
    J4.write(pos);
    break;
  case 6:
    gripper.write(pos);
    break;
  default:
    Serial.println("Wrong servo id");
    break;
  }
}


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

  Serial.println("Initializition has been done !");

  // Connect to access point
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  

}

void loop()
  {
  WiFiClient client;

  if (client.connect(host, port)) {
    if (!connection_complted){
      line = client.readString();
      Serial.println(line);
      delay(1);
      client.write("ok");
      connection_complted = true;
    }

    line = client.readString();
    if (line.length() == 0){
      Serial.print("No data");
      Serial.println(line);
      client.flush();
      client.clearWriteError();
      client.stop();
    }
    else{
      Serial.println(line);
      incomming = line.c_str();
      int joint_pos, speed, servo_id;
      sscanf(incomming, "%s{%d}{%d}{%d}", execution_type, &joint_pos, &speed, &servo_id);
      client.write("ok");
    }
  
    
    
    // if (execution_type == "j"){
    //   jog_executor(joint_pos, speed, servo_id);
    //   Serial.print("Jog execution requested");
    //   Serial.println(line);
    //   client.write("ok");
    // }
    // else if (execution_type == "t"){
    //   String line = client.readString();
    //   incomming = line.c_str();
    //   sscanf(incomming, "%s{%d}{%d}{%d}{%d}{%d}{%d}{%d}", execution_type, &J0_POS, &J1_POS, &J2_POS, &J3_POS, &J4_POS, &J5_POS, &speed);
    //   Serial.print("Trajectory execution requested");
    //   Serial.println(line);
    //   client.write("ok");
    // }
    incoming_data_reset();
    delay(1);
  }

  else{
    connection_complted = false;
  }
  }

