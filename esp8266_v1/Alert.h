// update status variables of ESP8266
void update_data(){
  // vibration
  if (esp_vibration == 1) {
    flagVibration = true;
  } else {
    flagVibration = false;
  }

  // tilt
  if (esp_tilt == 1) {
    flagTilt = true;
  } else {
    flagTilt = false;
  }

  // heart
  if (((esp_heart >= 2 and esp_heart < 10) or esp_heart > 100) and countHeart < 2) {
    countHeart ++;
    flagHeart = false;
  } else if (((esp_heart >= 2 and esp_heart < 10) or esp_heart > 100) and countHeart >= 2) {
    flagHeart = true;  
  } else if ((esp_heart >= 10 and esp_heart < 100) and countHeart > 0) {
    countHeart --;
    flagHeart = false;
  }

  // dead 
  if (esp_heart < 2) {
    flagDead = true;
  } else {
    flagDead = false;
  }

 // distance
  if (esp_distance < 20) {
    flagDistance = true;
  } else {
    flagDistance = false;
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

  if(esp_call == 0){
    flagCall = true;
  } else{
    flagCall = false;
  }
  
}
