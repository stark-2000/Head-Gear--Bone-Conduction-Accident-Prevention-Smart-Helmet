//nRF24L01 communication 2 ways Arduino B

#include <SPI.h>
#include <RF24.h>
#include <printf.h>
RF24 radio(9, 10); // CE, CSN
const byte addresses [][6] = {"00001", "00002"};    //Setting the two addresses. One for transmitting and one for receiving

void setup() 
{
  Serial.begin(9600);
  mpu_init();
  radio.begin();                            //Starting the radio communication
  radio.openWritingPipe(addresses[0]);      //Setting the address at which we will send the data
  radio.openReadingPipe(1, addresses[1]);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);            //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  printf_begin();
  radio.printDetails();
}

void loop() 
{
  delay(5);
  radio.startListening();                    //This sets the module as receiver
  if (radio.available())                     //Looking for incoming data
  {
    char button_stateA[32] = "";
    radio.read(&button_stateA, sizeof(button_stateA));
    Serial.println(button_stateA);
  }
  delay(10);
  
  radio.stopListening();                           //This sets the module as transmitter
  MPU_6050();
}
