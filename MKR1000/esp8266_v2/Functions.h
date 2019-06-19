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

// rileva se ci sono fiamme
int flame()
{
  val3 = digitalRead (pinFlame) ;
  // quando rileva la fiamma accende il led verde del sensore
  if (val3 == HIGH)
  {
    digitalWrite (Led, HIGH);
  }
  else
  {
    digitalWrite (Led, LOW);
  }
  return val3;
}
