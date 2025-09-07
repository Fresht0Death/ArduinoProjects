
// Room temp sensor that tracks hottest/coldest (°F) and shows LED bands (°C)

#include <Arduino.h>

const int sensorPin = A0;
const float baselineTemp = 20.0;
float lowestTemp = 74; // starting off at room temp
float highestTemp = 74; // startif off at room temp
bool minmaxInitialized = false;



    

// function to update min and max temps
void checkMinMax(float* high, float* low, float* currTemp){

    if(*currTemp > *high)      *high = *currTemp;
    else if(*currTemp < *low)  *low = *currTemp;
    
}


void setup() {
  // put your setup code here, to run once:



  Serial.begin(9600); // open a serial port

  for(int pinNumber = 2; pinNumber < 5; pinNumber++){ // 
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);
  Serial.print("Sensor Value: ");
  Serial.print(sensorVal);

  // convert the ADC reading to voltage
  float voltage = (sensorVal/1024.0) * 5.0;

  Serial.print(", Volts: ");
  Serial.print(voltage);
  Serial.print(", degrees C: ");
  // convert the voltage to temp in degrees using info from arduino.cc/kitdatasheets
  float temperature = (voltage - .5) * 100;
  float tempFar = (9.0/5.0) * temperature + 32.0; // coversion to Far
  Serial.print(temperature);
  Serial.print(", degrees F: ");
  Serial.println(tempFar);

if (!minmaxInitialized) {
  lowestTemp  = tempFar;
  highestTemp = tempFar;
  minmaxInitialized = true;
}





  if(temperature < baselineTemp + 2){
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }else if(temperature >= baselineTemp + 2 && temperature < baselineTemp + 4){
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }else if(temperature >= baselineTemp + 4 && temperature < baselineTemp + 6){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }else if(temperature >= baselineTemp + 6){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }


  checkMinMax(&highestTemp, &lowestTemp, &tempFar);

  Serial.print("Overall Max temp(F):\t");
  Serial.println(highestTemp);
  Serial.print("Overall Low temp(F):\t");
  Serial.println(lowestTemp);




  delay(3500);



}
