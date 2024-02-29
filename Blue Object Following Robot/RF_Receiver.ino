#include "RF24.h" 
#include <SPI.h>

RF24 myRadio (7, 8); 
char incByte[1];
byte addresses[][6] = {"0"}; 
// Motor A connections
int enA = 9; 
int in1 = 10; //l1
int in2 = 6; //l2
// Motor B connections
int enB = 3;
int in3 = 5; //b1
int in4 = 4; //b2

void setup()
{
  Serial.begin(9600); // // opens serial port, sets data rate to 9600 bps
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  
}

void loop()
{
  if (myRadio.available() > 0) {
    // read the incoming byte:
    while(myRadio.available()){
      myRadio.read(incByte, sizeof(incByte)); //changes &incByte to incBYte
    }
    char dir = incByte[0];
    switch(dir)
    {
      case 'f':
         // forward
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        analogWrite(enA, 255);
        analogWrite(enB, 255);
        break;

      case 'b':
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        break;
      
      case 's':
      //stop
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        break;

      case 'r':
        // analogWrite(enA, 64);
        // analogWrite(enB, 64);

      //right
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        break;

      case 'l':
        // analogWrite(enA, 64);
        // analogWrite(enB, 64);
        //left
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        break;
    }
  }
}
