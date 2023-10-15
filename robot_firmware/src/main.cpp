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
Servo J5;

int J0_pwm_pin = 32;  
int J1_pwm_pin = 33;
int J2_pwm_pin = 25;
int J3_pwm_pin = 26;
int J4_pwm_pin = 27;
int J5_pwm_pin = 14;

int J0_POS, J1_POS, J2_POS, J3_POS, J4_POS, J5_POS;
String line = "";
const char *incomming = "";
int execution_type = 0;
int servo_speed = 0;
int jog_pos = 0;
int servo_id = 0;


void jog_executor()
{
  switch (servo_id)
  {
  case 1:
    J0.write(jog_pos);
    break;
  case 2:
    J1.write(jog_pos);
    break;
  case 3:
    J2.write(jog_pos);
    break;
  case 4:
    J3.write(jog_pos);
    break;
  case 5:
    J4.write(jog_pos);
    break;
  case 6:
    J5.write(jog_pos);
    break;
  default:
    Serial.println("not ok");
    WebSerial.println("not woking");
    break;
  }
}

void trajectroy_executor(){
  // int J0_diff = J0_POS-J0.read();
  // J0_diff=J0_diff/2;
  // for (int8_t i = 0; i < J0_diff; i++)
  // {
    
  // }
  J0.write(J0_POS);
  J1.write(J1_POS);
  J2.write(J2_POS);
  J3.write(J3_POS);
  J4.write(J4_POS);
  J5.write(J5_POS);
  Serial.println("ok");
}

void PWM_INIT(){
    // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  // standard 50 hz servo
  J0.setPeriodHertz(50);
	J1.setPeriodHertz(50);    
  J2.setPeriodHertz(50);
	J3.setPeriodHertz(50);
  J4.setPeriodHertz(50);
	J5.setPeriodHertz(50);
  
  J0.attach(J0_pwm_pin, 500, 2400);
  J1.attach(J1_pwm_pin, 500, 2400);
  J2.attach(J2_pwm_pin, 500, 2400);
  J3.attach(J3_pwm_pin, 500, 2400); 
  J4.attach(J4_pwm_pin, 500, 2400);
  J5.attach(J5_pwm_pin, 500, 2400);
}

void web_serial_pos_state(){
  WebSerial.println(J0_POS);
  WebSerial.println(J1_POS);
  WebSerial.println(J2_POS);
  WebSerial.println(J3_POS);
  WebSerial.println(J4_POS);
  WebSerial.println(J5_POS);
  WebSerial.println(servo_speed);
}

void home_robot_1(){
  J0_POS = 90;
  J1_POS = 100;
  J2_POS = 90;
  J3_POS = 70;
  J4_POS = 90;
  J5_POS = 100;
  trajectroy_executor();
}

void home_robot(){
  J0_POS = 90;
  J1_POS = 130;
  J2_POS = 130;
  J3_POS = 180;
  J4_POS = 90;
  J5_POS = 100;
  trajectroy_executor();
}

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initialization has been started");

  PWM_INIT();

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  Serial.println("Initializition has been done !");

  Serial.print("AP IP address: ");
  Serial.println(IP);
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  /* Attach Message Callback */
  WebSerial.msgCallback(recvMsg);
  server.begin();
  delay(1000);
  home_robot();
}

void loop(){
  if (Serial.available()){
    line = Serial.readString();
    incomming = line.c_str();
    WebSerial.println(line);
    sscanf(incomming, "[%i]", &execution_type);

    if (execution_type == 1)  // Homing of robot requested
    {
      home_robot();
    }
    else if (execution_type == 2)  // jog mode
    {
      sscanf(incomming, "[%i][%i][%i][%i]", &execution_type, &jog_pos, &servo_speed, &servo_id);
      jog_executor();
      // Serial.println("ok");
    }
    else if (execution_type == 3 or execution_type == 4)  // teach mode and run mode
    {
      sscanf(incomming, "[%i][%i][%i][%i][%i][%i][%i][%i]",&execution_type, &J0_POS, &J1_POS, &J2_POS, &J3_POS, &J4_POS, &J5_POS, &servo_speed);
      trajectroy_executor();
    }
    
    //web_serial_pos_state();
  }
}

