/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const uint64_t addresses[][2] = {0xE0E0F1F1E0LL, 0xF1F1F0F0E0LL};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(0x60);
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  Serial.println("Sent : " + String(text));
  delay(5);
  radio.startListening();
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  else{
    Serial.println("Recieved None....");
  }
  radio.stopListening();
}
