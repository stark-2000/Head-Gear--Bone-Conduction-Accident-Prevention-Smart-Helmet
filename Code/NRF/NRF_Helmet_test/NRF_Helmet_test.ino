//nRF24L01 communication 2 ways Arduino A

#include <SPI.h>
#include <RF24.h>
#include <printf.h>

RF24 radio(9, 10); // CE, CSN
const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving


void setup() 
{
  Serial.begin(9600);
  radio.begin();                           //Starting the radio communication
  radio.openWritingPipe(addresses[1]);     //Setting the address at which we will send the data
  radio.openReadingPipe(1, addresses[0]);  //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN); //You can set it as minimum or maximum depending on the distance between the transmitter and receiver. 
  printf_begin();
  radio.printDetails();
}

void loop() 
{ 
  delay(5);
  radio.stopListening();                             //This sets the module as transmitter
  byte button_stateA = byte("Hello");
  Serial.println(button_stateA);
  radio.write(&button_stateA, sizeof(button_stateA));  //Sending the data
  delay(500);
  
  radio.startListening();                            //This sets the module as receiver
  if (radio.available())
  {
  byte button_stateB = byte("");
  radio.read(&button_stateB, sizeof(button_stateB)); //Reading the data
  Serial.println(button_stateB);
  }
}
