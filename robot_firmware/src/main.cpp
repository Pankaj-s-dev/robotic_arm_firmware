#include <Arduino.h>
#include "./../lib/ESP32_Servo/ESP32Servo.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <../.pio/libdeps/esp32dev/AsyncTCP-esphome/src/AsyncTCP.h>
#include <../lib/web_serial/src/WebSerial.h>  

/*-------- WiFi Creditionals --------*/
const char* ssid = "ESP32_Access_point";
const char* password = "ESP32@123";

/*---Function decelearation---*/
void home_robot();
void jog_executor();
void smooth_drive(int starting_angle, int stoping_angle, int finishing_angle, bool inc_dec);
void smooth_trajectory_driver();
void web_serial_pos_state();
void trajectroy_executor();
bool readStringUntil(String& input, char until_c, size_t char_limit);


/*
0.17sec/60deg 

2.8mills /1deg will assume 2mills/deg

*/

AsyncWebServer server(80);  //Async webserver for web serial

/*---- Servo constructor -----*/
Servo J0; 
Servo J1;
Servo J2;
Servo J3;
Servo J4;
Servo J5;

/*----- Servo PWM Pin INIT -----*/
int J0_pwm_pin = 32;  
int J1_pwm_pin = 33;
int J2_pwm_pin = 25;
int J3_pwm_pin = 26;
int J4_pwm_pin = 27;
int J5_pwm_pin = 14;

/*--------- Variable INIT for Execution ---------*/
int J0_POS, J1_POS, J2_POS, J3_POS, J4_POS, J5_POS;
String read_msg = "";
const char *incomming = "";
int execution_type = 0;
int servo_speed = 0;
int jog_pos = 0;
int servo_id = 0;

/*----- Message callback of WebSerial -----*/
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "home"){
    WebSerial.println("Homing robot...");
    home_robot();
  }
}

/*----------- Jog executer -----------*/
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
    WebSerial.println("Error Occured.....");
    break;
  }
}

void smooth_drive(int starting_angle, int stoping_angle, int finishing_angle, bool inc_dec){
  int counter = 2;
  if (inc_dec)
  {
    for (starting_angle; starting_angle <= stoping_angle; starting_angle += counter)
    {
      J0.write(starting_angle);
      delay(counter);
      counter+=2;
    }
    J0.write(finishing_angle);
    return;
  }
  else
  {
    for (starting_angle ; starting_angle >= stoping_angle; starting_angle=-counter)
    {
      J0.write(starting_angle);
      delay(counter);
      counter+=2;
    }
    J0.write(finishing_angle);
    return;
    
  }
}

void smooth_trajectory_driver(){
  int J0_diff = J0_POS-J0.read();
  J0_diff=J0_diff/2;
  if (J0_diff > 0){
    smooth_drive(J0.read(), J0_diff, J0_POS, true);
  }
  else if (J0_diff < 0)
  {
    J0_diff = J0_diff*-1;
    smooth_drive(J0.read(), J0_diff/2, J0_POS, false);
  }
  
}

void trajectroy_executor(){
  J0.write(J0_POS);
  J1.write(J1_POS);
  J2.write(J2_POS);
  J3.write(J3_POS);
  J4.write(J4_POS);
  J5.write(J5_POS);
  delay(10);
  WebSerial.println("Trajectory executed");
  Serial.println("ok");
}

/*---------- PWM Initialization ----------*/
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
  //------ Attaching PWM Pin ------
  J0.attach(J0_pwm_pin, 500, 2400);
  J1.attach(J1_pwm_pin, 500, 2400);
  J2.attach(J2_pwm_pin, 500, 2400);
  J3.attach(J3_pwm_pin, 500, 2400); 
  J4.attach(J4_pwm_pin, 500, 2400);
  J5.attach(J5_pwm_pin, 500, 2400);
}

/*---------- Updating posiiton on web serial ----------*/
void web_serial_pos_state(){
  WebSerial.print("J0 Pos : "); WebSerial.println(J0_POS);
  WebSerial.print("J1 Pos : "); WebSerial.println(J1_POS);
  WebSerial.print("J2 Pos : "); WebSerial.println(J2_POS);
  WebSerial.print("J3 Pos : "); WebSerial.println(J3_POS);
  WebSerial.print("J4 Pos : "); WebSerial.println(J4_POS);
  WebSerial.print("J5 Pos : "); WebSerial.println(J5_POS);
}

/*---------- Robot home point ----------*/
void home_robot(){
  J0_POS = 90;
  J1_POS = 130;
  J2_POS = 130;
  J3_POS = 180;
  J4_POS = 90;
  J5_POS = 100;
  WebSerial.println("Homing Done !");
  trajectroy_executor();
}

/*---------- Reading data until given limit ----------*/
bool readStringUntil(String& input, char until_c, size_t char_limit) {
  WebSerial.print("reading unitll");
  input = "";
  while (Serial.available()) {
    
    char c = Serial.read();
    WebSerial.print(c);
    input += c;
    if (c == until_c) {
      return true;
    }
    if (input.length() >= char_limit) {
      WebSerial.println();
      return true;
    }
  }
  WebSerial.println();
  return false;
}

/*-------------------Void setup-------------------*/
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Initialization has been started");

  PWM_INIT();

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  Serial.println("Initializition has been done !");

  Serial.print("AP IP address: ");
  Serial.println(IP);
  WebSerial.begin(&server);

  /* Attach Message Callback */
  WebSerial.msgCallback(recvMsg);

  server.begin();
  delay(500);
  home_robot();
}


void loop(){
  if (Serial.available()){
    readStringUntil(read_msg, '\n', 40);
    incomming = read_msg.c_str();
    WebSerial.print(" Recived MSG :");
    WebSerial.println(read_msg);
    sscanf(incomming, "[%i]", &execution_type);

    if (execution_type == 1)  // Robotic arm Homing
    {
      home_robot();
    }
    else if (execution_type == 2)  // jog mode.
    {
      sscanf(incomming, "[%i][%i][%i][%i]", &execution_type, &jog_pos, &servo_speed, &servo_id);
      jog_executor();
    }
    else if (execution_type == 3 or execution_type == 4)  // teach mode and run mode.
    {
      sscanf(incomming, "[%i][%i][%i][%i][%i][%i][%i][%i]",&execution_type, &J0_POS, &J1_POS, &J2_POS, &J3_POS, &J4_POS, &J5_POS, &servo_speed);
      trajectroy_executor();
    }
    
    // web_serial_pos_state(); // Updating POS on Web.
  }
}

