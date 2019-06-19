// ritorna true se c'è l'intero nell'array
boolean inList(int arr[], int element){

  int len = (sizeof(arr) / sizeof(arr[0]));
  
  for(int i = 0; i < len; i++){
      if(arr[i] == element){
          return true;
      }
  }
  return false;
}

// aggiunge l'intero all'array nella prima posizione libera (==0)
void addElement(int arr[], int id){
  
  int len = (sizeof(arr) / sizeof(arr[0]));
  
  for (int i = 0; i < len; i++){
    if (arr[i] == 0){
      arr[i] = id;
      return;
    }
  }
}

// usato per salvare valori delle misurazioni e ritornare ultimo indice se pieno
int inArray(int arr[], int value, int element){
  
  int len = (sizeof(arr) / sizeof(arr[0]));
  int pos = 0;
  
  for(int i = 0; i < len; i++){
      if(arr[i] == element){
          arr[i] = int(value);
          return i;
      }
  }
  return len;
}

// resetto intero array a -999
void resetArray(int arr[]){

  int len = (sizeof(arr) / sizeof(arr[0]));
 
  for(int i = 0; i<len; i++){
    arr[i] = -999;
  }
}

// stampo elementi array
void printArray(int arr[]){

  int len = (sizeof(arr) / sizeof(arr[0]));
    
  for(int i = 0; i<len; i++){
    Serial.println(arr[i]);
  }
  Serial.println();
}

// calolo e ritorno media dell'array
int meanArray(int arr[]){

  int len = (sizeof(arr) / sizeof(arr[0]));
  
  int tot = 0;
  
  for(int i = 0; i < len; i++){
    tot += arr[i];
  }
  return  int(tot/len);
}
