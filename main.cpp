#include <Arduino.h>
#include <ESP32Servo.h>

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

void setup() {
  Serial.begin(9600);
  delay(10);
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

void loop()
  {

  J0.write(pos);
  J1.write(pos);
  J2.write(pos);
  J3.write(pos);
  J4.write(pos);
  gripper.write(pos);
  }

