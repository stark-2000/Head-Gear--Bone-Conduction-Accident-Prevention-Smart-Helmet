#include<SPI.h>                   // spi library for connecting nrf
#include<RF24.h>                  // nrf library
//#include<nRF2401.h>
#include "printf.h"

RF24 radio(9, 10);  // ce, csn pins    

int i = 0;

void setup(void) {

  while (!Serial) ;
  Serial.begin(9600) ;     // start serial monitor baud rate
  Serial.println("Starting.. Setting Up.. Radio on..") ; // debug message
  if (!radio.begin()) {
    Serial.println(F("radio hardware not responding!"));
    while (!radio.begin()) {} // hold program in infinite loop to prevent subsequent errors
  }
  radio.setPALevel(RF24_PA_MIN) ;   // set power level
  radio.setChannel(0x77) ;            // set chanel at 76
  const uint64_t pipe[2] = {0xE0E0F1F1E0LL, 0xF1F1F0F0E0LL} ;    // pipe address same as sender i.e. raspberry pi
  //radio.openReadingPipe(1, pipe[0]) ;        // start reading pipe 
  radio.openWritingPipe(0xE8E8F0F0E1LL) ;        // start writing pipe 
  radio.enableDynamicPayloads() ;
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();
  //radio.powerUp() ;  

  printf_begin();
  radio.printDetails();
  
  if (radio.isChipConnected()){
    Serial.println("YAY! the connections are all correct");        
  }
  else{
    while (!radio.isChipConnected()) Serial.println("Check connection");
  }
}


void loop(void) {
//  radio.startListening() ;        // start listening forever
//  char receivedMessage[32] = {0} ;   // set incmng message for 32 bytes
//  if (radio.available()) {       // check if message is coming
//    radio.read(receivedMessage, sizeof(receivedMessage));    // read the message and save
//    Serial.println(receivedMessage) ;    // print message on serial monitor 
//    Serial.println("Turning off the radio.") ;   // print message on serial monitor
//    radio.stopListening() ;   // stop listening radio
//    String stringMessage(receivedMessage) ;     // change char to string
//    delay(1000);    // delay of 1 second 
//  }
  const String str = " Hello RPI " +String(i++);
  int str_len = str.length() + 1;

  char text[str_len];

  str.toCharArray(text,str_len);

  radio.write(&text,str_len);
  Serial.println(text);
  delay(100);
}
