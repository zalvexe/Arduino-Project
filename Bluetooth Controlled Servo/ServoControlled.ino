#include <SoftwareSerial.h>
#include<Servo.h>
  int bluetoothTx = 10; //10th pin
  int bluetoothRx = 11; //11th pin
  SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
  Servo myservo;

  void setup() {
   myservo.attach(9); //servo to 9th pin
   Serial.begin(9600); 
   bluetooth.begin(9600); // setup bluetooth to android
  }
  
  void loop() {
    if(bluetooth.available()>0){ 
      int servopos = bluetooth.read(); // getting the input rotation number
      Serial.println(servopos);
      myservo.write(servopos); // rotate servo
      }
  }
