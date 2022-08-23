const int battery = A1;
void setup() {
  // put your setup code here, to run once:
  pinMode(battery,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float data = analogRead(battery);
  float v_data = 5*data/1023;
  Serial.println(v_data);
  delay(0.1);
}
