// --- Hardware mapping ---
const int POT_PIN   = A0;   // potentiometer input
const int LED1_PIN  = 2;    // bar LEDs
const int LED2_PIN  = 3;
const int LED3_PIN  = 4;
const int BLUE_LED  = 5;   // blue LED on 5 (PWM pin)

// --- State ---
int sensorVal = 0;


// -- Steppers --
int bigStepper = 0;
int step = 25;

// -- Pulse Status --
unsigned long pulseStart = 0;
bool pulsing = false;

void setup() {
  // initiate the serial input 9600 bits/sec
  Serial.begin(9600); 

  // set the leds to output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);


  // set leds to OFF
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(BLUE_LED, LOW);


  // checking if BLUE_LED worked
  // analogWrite(BLUE_LED, 128); 
  // delay(500);
  // analogWrite(BLUE_LED, 0);
}

void loop() {

  // Read pot (0..1023)
  sensorVal = analogRead(POT_PIN);



  // Map to percent (0..100) without calibration
  float percent = (sensorVal / 1023.0f) * 100.0f;

  // Debug by checking raw sensor value and calcuated percent value
  Serial.print("Raw="); Serial.print(sensorVal);
  Serial.print("  %="); Serial.println(percent, 1);

  // checks if BLUE_LED isn't not in pulsing state
  if(!pulsing) {

  // Simple 3-step bar on pins 2,3,4
  if (percent < 25.0f) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    analogWrite(BLUE_LED, 0);

  } else if (percent < 50.0f) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    analogWrite(BLUE_LED, 0);

  } else if(percent < 75.0f){
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW);
    analogWrite(BLUE_LED, 0);

  }else{
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    analogWrite(BLUE_LED, 0);

  }

  // Blue LED on 5 pulses when max percent reached so we change the state so this is possible an begin the process
  if (percent >= 95.0f) { // max state when percent > 95.0

    // set the state for pulsing as true to begin pusling process
    pulsing = true; 
    // this is our time 0
    pulseStart = millis();

    // this will be used to set up the rate of change of the pulsing (0-255)
    bigStepper = 0;
    step = 5;
    }
}
  if(pulsing){ // if pulse state has been inititated
    if (millis() - pulseStart < 5500){ // while the time elpased since beginning of pulse state to this loop is less than 5.5 keep looping

    // turn off 1-3 pins
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);

      // increment stepper
      bigStepper += step;
      if(bigStepper >= 255 || bigStepper <= 0) step = -step; // makes sure stepper is not overshooting max value possible
      //sets the Blue led to a light level equal to big stepper
      analogWrite(BLUE_LED, bigStepper);
      delay(15); // controls pulse speed

    }else{ // restart the process
      pulsing = false; // set pulse state back to OG state
      analogWrite(BLUE_LED, 0); // turn off Blue led
    }
  }
}

