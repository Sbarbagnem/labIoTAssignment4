// update status variables of MKR1000
void updateHome() {

  // temperatura
  if(temperature <= -40){
    flagHardTemp = true;
  } else {
    flagHardTemp = false;
  }
  
  if(!flagHardTemp){
    if (temperature > 30 and countTemp < 2) {
      countTemp ++;
      flagTemp = false;
    } else if (temperature > 30 and countTemp >= 2) {
      // set flag for catch event
      flagTemp = true;
      writeDB = true;
    } else if (temperature <= 30 and countTemp > 0) {
      countTemp --;
      flagTemp = false;
      writeDB = false;
    }
  }


  // wifi
  if (wifi > - 25 and countWifi < 2) {
    countWifi ++;
    flagWifi = false;
  } else if (wifi > - 25 and countWifi <= 2) {
    flagWifi = true;
    writeDB = true;
  } else if (wifi <= - 25 and countWifi > 0) {
    countWifi --;
    flagWifi = false;
    writeDB = false;
  }

  // distance
  if(distance == 0){
    flagHardDist = true;
  } else {
    flagHardDist = false;
  }

  if(!flagHardDist){
    if (distance < 10 and countIntrusion < 2) {
      countIntrusion ++;
      flagIntrusion = false;
    } else if (distance < 10 and countIntrusion >= 2) {
      flagIntrusion = true;
      writeDB = true;
    } else if (distance >= 10 and countIntrusion > 0) {
      countIntrusion --;
      flagIntrusion = false;
      writeDB = false;
    }
  }

  // flame
  if (flame == 1 and countFlame < 2) {
    countFlame ++;
    flagFlame = false;
  } else if (flame == 1 and countFlame >= 2) {
    flagFlame = true;
    writeDB = true;
  } else if (flame == 0 and countFlame > 0) {
    countFlame --;
    flagFlame = false;
    writeDB = false;
  }

  // light
  if(light < 4){
    flagHardLight = true;
  } else {
    flagHardLight = false;
  }

  if(!flagHardLight){
    if (light < 40 and countLight < 2) {
      countLight ++;
      flagLight = false;
    } else if (light < 40 and countLight >= 2) {
      flagLight = true;
      writeDB = true;
    } else if (light >= 40 and countLight > 0) {
      countLight --;
      flagLight = false;
      writeDB = false;
    }
  }

  if(sound == 9){
    flagHardSound = true;
  } else {
    flagHardSound = false;
  }

  if(!flagHardSound){
    // sound
    if (sound > 40 and countSound < 2) {
      countSound ++;
      flagSound = false;
    } else if (sound > 40 and countSound >= 2) {
      flagSound = true;
      writeDB = true;
    } else if (sound <= 40 and countSound > 0) {
      countSound --;
      flagSound = false;
      writeDB = false;
    }
  }
}

// update array to store values and then make mean
void updateArrayValue(){
   
  int index = 0;
  inArray(arr_distance, distance, -999);
  inArray(arr_light, light, -999);
  inArray(arr_sound, sound, -999);
  inArray(arr_wifi, wifi, -999);
  index = inArray(arr_temperature, temperature, -999);
  
  // se ho salvato tot valori dei sensori allora faccio le medie da salvare in DB
  if(index == countCycle){

    distance = meanArray(arr_distance);
    light = meanArray(arr_light);
    wifi = meanArray(arr_wifi);
    temperature = meanArray(arr_temperature);
    sound = meanArray(arr_sound);

    writeDB = true;
  }
}

void cleanVarHome() {

  flagTemp = false;
  flagWifi = false;
  flagIntrusion = false;
  flagFlame = false;
  flagLight = false;
  flagSound = false;

  countTemp = 0;
  countWifi = 0;
  countIntrusion = 0;
  countFlame = 0;
  countLight = 0;
  countSound = 0;
}
