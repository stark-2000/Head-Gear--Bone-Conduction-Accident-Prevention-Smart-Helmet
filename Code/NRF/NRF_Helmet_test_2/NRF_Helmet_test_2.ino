//nRF24L01 communication 2 ways Arduino A

#include <SPI.h>
#include <RF24.h>
#include <printf.h>
#include <SoftwareSerial.h>

RF24 radio(9, 10); // CE, CSN
SoftwareSerial RPi(5, 6); // HC-12 TX Pin, HC-12 RX Pin

const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving

const int alcohol = A0; 
const int battery = A1;

float ratio= 0 ;

struct txData {
  float alc; 
  float bat;
};

struct rxData {
  float pitch; 
  float roll; 
  float del_p;
  float del_r;
  float gps_vel;
  String s;
};


txData tData;
rxData rData;

void setup() 
{
  Serial.begin(9600);
  RPi.begin(9600);               // Serial port to RPi
  
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
  float level = 5*analogRead(battery)/1023;
  
  ratio = (level/3.7);
  
  tData.bat = ratio*100;
  tData.alc = a_data;
  radio.write(&tData, sizeof(tData));  //Sending the data
  
  // If HC-12 has data
  if (RPi.available()){
    Serial.println(RPi.readStringUntil("\n"));      // Send the data to Serial monitor
    // If Serial monitor has data
    RPi.println("alcohol data = " + String(a_data) + "  Battery percentage = " + String(ratio*100));      // Send that data to HC-12
  }
  delay(10);
  radio.startListening();                            //This sets the module as receiver
  if (radio.available())
  {
    radio.read(&rData, sizeof(rData)); //Reading the data
    Serial.print(rData.pitch);
    Serial.print("  ");
    Serial.print(rData.roll);
    Serial.print("  ");
    Serial.print(rData.del_p);
    Serial.print("  ");
    Serial.print(rData.del_r);
    Serial.print("  ");    
    Serial.println(rData.gps_vel);
  }
}
