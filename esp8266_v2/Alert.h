// update status variables of ESP8266
void update_data(){
  // vibration
  if (esp_vibration == 1) {
    flagVibration = true;
  } else {
    flagVibration = false;
  }

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

  // flame
  if (esp_flame == 1 and countFlame < 2) {
    countFlame ++;
    flagFlame = false;
  } else if (esp_flame == 1 and countFlame >= 2) {
    flagFlame = true;
  } else if (esp_flame == 0 and countFlame > 0) {
    countFlame --;
    flagFlame = false;
  }
}
