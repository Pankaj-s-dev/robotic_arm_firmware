#include <Arduino.h>
#include "./../lib/ESP32_Servo/ESP32Servo.h"

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
  
  Serial.begin(115200);
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
}

void loop(){
  if (Serial.available()){
    Serial.println(Serial.readString());
    delay(100);
  }
}

