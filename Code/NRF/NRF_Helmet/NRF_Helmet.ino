//nRF24L01 communication 2 ways Arduino A

#include <SPI.h>
#include <RF24.h>
#include <printf.h>
#include <SoftwareSerial.h>

RF24 radio(9, 10); // CE, CSN
SoftwareSerial HC12(5, 6); // HC-12 TX Pin, HC-12 RX Pin

const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving

const int alcohol = A0; 
const int battery = A1;

float ratio= 0 ;

void setup() 
{
  Serial.begin(9600);
  HC12.begin(9600);               // Serial port to HC12
  
  pinMode(battery,INPUT);
  pinMode(alcohol,INPUT);
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
  radio.stopListening();         
  float a_data = analogRead(alcohol);
  //char button_stateA[32] = "";
  if (a_data > 750){
    char button_stateA[32] = "dangerous driving now";
    //Serial.println(button_stateA);
    radio.write(&button_stateA, sizeof(button_stateA));  //Sending the data
    delay(10);
  }
  else if (a_data < 750 && a_data > 250){
    char button_stateA[32] = "mildly drunk drive carefully";
    //Serial.println(button_stateA);
    radio.write(&button_stateA, sizeof(button_stateA));  //Sending the data
    delay(10);
  }
  else{
    char button_stateA[32] = "not drunk, all good :)";
    //Serial.println(button_stateA);
    radio.write(&button_stateA, sizeof(button_stateA));  //Sending the data
    delay(10);
  }
  battery_level();
  
  // If HC-12 has data
  Serial.println(HC12.readStringUntil("\n"));      // Send the data to Serial monitor
  // If Serial monitor has data
  HC12.println("alcohol data = " + String(a_data) + "  Battery percentage = " + String(ratio*100));      // Send that data to HC-12
  
  radio.startListening();                            //This sets the module as receiver
  if (radio.available())
  {
    char button_stateB[32] = "";
    radio.read(&button_stateB, sizeof(button_stateB)); //Reading the data
    Serial.println(button_stateB);
  }
}
