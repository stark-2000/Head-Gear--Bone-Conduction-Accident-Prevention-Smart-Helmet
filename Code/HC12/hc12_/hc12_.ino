#include <SoftwareSerial.h>

SoftwareSerial HC12(5, 6); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
}
int i = 0;
void loop() {
  // If HC-12 has data
  Serial.println(HC12.readStringUntil("\n"));      // Send the data to Serial monitor
  // If Serial monitor has data
  HC12.println("Hi da!!");      // Send that data to HC-12
  delay(100);
}
