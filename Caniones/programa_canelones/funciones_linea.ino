





void leer_linea()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtrrc.read(sensorValues); instead of unsigned int position = qtrrc.readLine(sensorValues);
  unsigned int position = qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
//  for (unsigned char i = 0; i < NUM_SENSORS; i++)
//  {
//    Serial.print(sensorValues[i]);
//    Serial.print('\t');
//  }
//  //Serial.println(); // uncomment this line if you are using raw values
//  Serial.println(position); // comment this line out if you are using raw values
//  
//  delay(250);
}


int horizontal=0;
int detectar_horizontal(){
  for(unsigned char i=0; i<NUM_SENSORS; i++){
    if(sensorValues[i]>800) horizontal=1;
    else{
      horizontal=0;
      break;
    }
  }
  return horizontal;
}

