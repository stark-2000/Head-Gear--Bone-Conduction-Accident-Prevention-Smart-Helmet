void battery_level(){
  float level = 5*analogRead(battery)/1023;
  ratio = (level/3.7);
  tData.bat = ratio*10;
//  Serial.print(level);
//  Serial.print("  ");
//  Serial.println(ratio);
//  
//  if (ratio >= 0 && ratio < 0.25){
//     char button_stateB[32] = "Battery is down!! need charging";
//     //Serial.println(button_stateB);
//     radio.write(&button_stateB, sizeof(button_stateB));   //Sending the data 
//     delay(10);
//  }
//  else if (ratio >= 0.25 && ratio < 0.5){
//     char button_stateB[32] = "Battery is 25 to 50 percent";
//     //Serial.println(button_stateB);
//     radio.write(&button_stateB, sizeof(button_stateB));   //Sending the data 
//     delay(10);
//  }
//  else if (ratio >= 0.5 && ratio < 0.75){
//     char button_stateB[32] = "Battery is at 50 to 75 percent";
//     //Serial.println(button_stateB);
//     radio.write(&button_stateB, sizeof(button_stateB));   //Sending the data 
//     delay(10);
//  }
//  else if (ratio >= 0.75){
//     char button_stateB[32] = "Battery is at 75 to 100 percent";
//     //Serial.println(button_stateB);
//     radio.write(&button_stateB, sizeof(button_stateB));   //Sending the data 
//     delay(10);
//  }
}
