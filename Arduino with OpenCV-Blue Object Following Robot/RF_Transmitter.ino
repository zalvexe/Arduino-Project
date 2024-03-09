#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"}; //match this with the receiver
char incByte[1];

void setup()
{
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe(addresses[0]);
}

void loop()
{
   if (Serial.available() > 0) {
    incByte[0] = Serial.read();
    myRadio.write(incByte, sizeof(incByte));
    // Serial.print(incByte);
    // Serial.print("\n");
  }

}
