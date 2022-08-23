
// TODO: implement a clearing mechanism for SIM Memory of Messages. when your sim storage is full,
// your GET will not work.

// TODO: Change the _SS_MAX_RX_BUFF to 256 to increase RX buffer.
// in SoftwareSerial.h
// Connect GPS Tx to Rx1 of pro mini
// VCC to 3.3v
// GND to GND
// Connect GSM Rx to 3
// Tx to 2
// GND to GND
// Before uploading, disconnect the TX of GPS

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 8, 4, 5);

#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define motor 7
#define buzzer 6
const int speed1 = 0;
String RF;

// Initialize software serial to communicate with GSM module
SoftwareSerial mySerial(2, 3); //TX,RX wrt GSM Module

//Initialize GPS to communicate with GPS module
TinyGPS gps;

// Variable to hold latitude data to be sent to server.
float latitude;

// Variable to hold longitude data to be sent to server.
float longitude;

// Configure your Device ID here, This will be root of this Device info
char DEVICEID[10] = "TN07AZ2829";
char vehicletype[10] = "Bike";

// Prototype for gpsdump method
void gpsdump(TinyGPS &gps);

/* 
Function name: setup
Parameter name: None
P.Datatype: Not Applicable
P.Description: None
F.Description: Executes only once. Initialization is done here in this method.
*/

void setup() 
{
mySerial.begin(9600); // Setting the baud rate of GSM Module
Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino)
delay(1000);

pinMode(buzzer, OUTPUT);
pinMode(motor, OUTPUT);
digitalWrite(motor, LOW);

#ifdef RH_HAVE_SERIAL
   // Serial.begin(115200);    // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
        // Serial.println("init failed");
#else
  ;
#endif


// Establish HTTP connection in GSM module.
setupHttpInit();
}

/* 
Function name: loop
Parameter name: none
P.Datatype: Not applicable
P.Description: None
F.Description: Main loop of the software
*/

void loop() 
{
    int ab = 0; 
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.recv(buf, &buflen)) 
    {
     //driver.printBuffer("Got:", buf, buflen);
     String rcv;
     for(int i=0; i < buflen; i++)
     {
       rcv += (char)buf[i];
     }
     //Serial.println(rcv);
     RF = rcv;
    }
    
if (RF.equals("B")== 1)
{
  digitalWrite(buzzer, HIGH);
}

if (RF.equals("C")== 1)
{
  int ab = 1;
}

if(ab == 0 && speed1 > 30)
{
  digitalWrite(buzzer, HIGH);
  digitalWrite(motor, HIGH);
}

if (RF.equals("A")== 1)
{
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  mySerial.println("AT+CMGS=\"+919791115591\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  mySerial.print("Your contact Sidharth has met with an accident at 12.962N,80.211E"); //text content
  mySerial.write(26);
  mySerial.println("ATD+ +919791115591;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  delay(20000);
  mySerial.println("ATH");
}


bool newdata = false;
unsigned long start = millis();
// Every 5 seconds we print an update
while (millis() - start < 5000)
{
if (Serial.available())

{
char c = Serial.read();
//Serial.print(c); // uncomment to see raw GPS data
if (gps.encode(c))
{
newdata = true;
//break; // uncomment to print new data immediately!
}
}
}

if (newdata)
{
gpsdump(gps);

// Send to Server
sendDataToServer(DEVICEID);
delay(500);
}

} //closing void loop

/* 
Function name: waitUntilResponse
Parameter name: delayMs
P.Datatype: int
P.Description: Delay until next event. Should be specified in milliseconds.
F.Description: Hold Execution until the GSM Module sends reponse.
*/

void waitUntilReponse(int delayMs)
{
// Delay in ms
while (mySerial.available() < 0) 
      { delay(delayMs); }
while (mySerial.available() > 0) 
      { Serial.write(mySerial.read()); }
}

/* 
Function name: setupHttpInit
Parameter name: None
P.Datatype : Not Applicable
P.Description: None
F.Description: Setting up HTTP and Bearer configuration for sending HTTP GET request.
*/

void setupHttpInit() 
{

//Serial.println(“Waiting for GSM to get ready”);
//wait till gsm responds ok
waitUntilReponse(500);

//Serial.println(“GSM is ready proceed to send commands”);
// Sending APN Settings

/*mySerial.write("AT+SAPBR =0,1\r\n");
delay(500);
waitUntilReponse(500);
delay(3000);
*/   
mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n"); //Setting the connection type to GPRS
delay(1000);
waitUntilReponse(500);

mySerial.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\r\n"); //Set the APN to “airtelgprs.com” for airtel or "www" for vodafone
delay(1000);
waitUntilReponse(500);

mySerial.println("AT+SAPBR=1,1\r\n"); //Enabling the GPRS
delay(3000);
waitUntilReponse(500);

//If Arduino switches off but GSM is on or some error occurs in initialising HTTP, the following AT command rectifies the error..
mySerial.write("AT+HTTPTERM\r\n"); //If HTTP mode already enabled, it would turn off and would be turned on in the next AT command (AT+HTTPINIT) 
delay(300);
waitUntilReponse(500);
delay(3000);

mySerial.println("AT+HTTPINIT\r\n"); //Enabling the HTTP mode
delay(300);
waitUntilReponse(500);

}

/* 
Function name: sendDataToServer
Parameter name: deviceId
P.Datatype: int
P.Description: Device ID to be sent to server for whose GPS Coordinates is being Uploaded.
F.Description: Setting up HTTP and Bearer configuration for sending HTTP GET request.
*/

void sendDataToServer(char deviceId[10]) 
{
mySerial.println("AT+HTTPINIT=?\r\n"); //Enabling HTTP mode
delay(300);
waitUntilReponse(500);

mySerial.println("AT+HTTPPARA=\"CID\",1\r\n"); //Setting up the HTTP bearer profile identifier
delay(300);

/*The below part is for When GSM Switches off but Arduino is on, Checks for ERROR response from software serial to AT+HTTPPARA command
  and runs setupHttpInit() fxn to setup GPRS and initialise HTTP
*/

const byte numChars = 50;
char receivedChars[numChars];
boolean newData1 = false;
char rc;
static byte ndx = 0;
char endMarker = '\n';
while (mySerial.available() > 0) 
      {
        rc = mySerial.read();
        if (rc != endMarker) 
            {
              receivedChars[ndx] = rc;
              ndx++;
              if (ndx >= numChars) 
              {ndx = numChars - 1;}
            }
        else 
            {
              receivedChars[ndx] = '\0'; // terminate the string
              ndx = 0;
              newData1 = true;
            }
        }
if (newData1 == true) 
          {
            Serial.println(receivedChars);
            if (receivedChars[0] == 'E' && receivedChars[1] == 'R')
                {
                  Serial.println("Rectifying Error...");
                  setupHttpInit();
                }
            newData1 = false;
          }
delay(3000);

mySerial.write("AT+HTTPPARA=\"URL\",\"http://ferrysmart2.000webhostapp.com/Ferry_Smart_Data.php?deviceid="); //Setting the url to the address of the webpage to be accessed
mySerial.print(deviceId);
mySerial.write("&vehicletype=");
mySerial.print(vehicletype);
mySerial.write("&lat=");
mySerial.print(latitude,9); 
mySerial.write("&lon=");
mySerial.print(longitude,9);
mySerial.write(":80\"\r\n");   
delay(500);
waitUntilReponse(500);

mySerial.write("AT+HTTPACTION=0\r\n"); //Starting the HTTP GET session
delay(3000);
waitUntilReponse(500);

mySerial.write("AT+HTTPREAD=0,10000\r\n"); //Acknowledgement for reading the data stored
delay(500);
waitUntilReponse(500);

}

/* 
Function name: gpsdump
Parameter name: &gps
P.Datatype : TinyGPS
P.Description: Pointer to the gps global variable that contains the GPS data.
F.Description: writes GPS data to latitude(Global variable), longitude(Global variable).
*/

void gpsdump(TinyGPS &gps)
{
unsigned long age;
gps.f_get_position(&latitude, &longitude, &age); //function which gets lat and lon from tinygps library
}
