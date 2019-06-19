// update status variables of ESP8266
void update_data(){
  // temp
  if (esp_temp > 30 and countTemp < 2) {
    countTemp ++;
    flagTemp = false;
  } else if (esp_temp > 30 and countTemp >= 2) {
    flagTemp = true;    
  } else if (esp_temp <= 30 and countTemp > 0) {
    countTemp --;
    flagTemp = false;
  }
}
