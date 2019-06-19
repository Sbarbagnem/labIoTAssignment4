// Temperature sensor 
double temp(){

  if ( !ds.search(addr)) {
      ds.reset_search();
      
  }
  // If you want to know family device uncomment the Serial.print  
  if ( addr[0] == 0x10) {
      //Serial.print("Device is a DS18S20 family device.\n");
  }
  else if ( addr[0] == 0x28) {
      //Serial.print("Device is a DS18B20 family device.\n");
  }
  else {
      //Serial.print("Device family is not recognized: 0x");
      //Serial.println(addr[0],HEX);
      return -1000;
  }
 
  ds.reset();
  ds.select(addr);
  // start conversion, with parasite power on at the end
  ds.write(0x44,1);         
  
  delay(1000);     
  present = ds.reset();
  ds.select(addr);  
  // Read Scratchpad  
  ds.write(0xBE);         
  // we need 9 bytes
  for ( i = 0; i < 9; i++) {           
    data[i] = ds.read();
  }
  //Conversion of raw data to C
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  // test most sig bit
  SignBit = TReading & 0x8000;  
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; 
  }
  Tc_100 = (6 * TReading) + TReading / 4;    
  // separate off the whole and fractional portions
  Whole = Tc_100 / 100;  
  Fract = Tc_100 % 100;
  
  if (SignBit) // If its negative
  {
     Serial.print("-");
  }
  
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print("\n");
  //End conversion to C
  Fract_double =(double)Fract/100;
  val1 = (double)Whole + Fract_double;
  return val1;
}

// Vibration sensor
int vibration(){
  val2 = digitalRead (vibration_pin);
  return val2;
}

// Tilt sensor
int tilt(){
  val4 = digitalRead (tilt_pin);
  return val4;
}

// Heartbeat sensor
double heart() {
  static double oldValue = 0;
  static double oldChange = 0;
  
  float rawValue = analogRead (heart_pin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  val5 = value;
  oldValue = value;
  delay(100);
  return val5; 

}

// Proximity sensor
long distance()
{
  // bring the trigger output low
  digitalWrite( triggerPort, LOW);
  delayMicroseconds(2);
  // send a pulse of 10microsec on trigger
  digitalWrite(triggerPort, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPort, LOW);
  // takes the signal in the echo port as an input and finds a distance (r) via a calculation
  long duration = pulseIn( echoPort, HIGH);
  val6 = 0.034 * duration / 2;
  delay(100);
  return val6;
}

// call 
int call(){
  val7 = digitalRead (call_pin); 
  return val7;
}
