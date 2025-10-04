

int switchState = 0;

void setup() {
  // put your setup code here, to run once:

  // setting up pins being used for input or output pins 2-5
  pinMode(3, OUTPUT); // PWm-Digital pin 3 for output
  pinMode(4, OUTPUT); // PWM-Digital pin 4 for output
  pinMode(5, OUTPUT); // PWM-Digital pin 5 for output
  pinMode(2, INPUT);  // PWM-Digital pin 2 for input

}

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(2); // this reads the current state of the switch, that is, what is the voltage level?

  if(switchState == LOW){  // button is not pressed and switch == 0/LOW
    digitalWrite(3, HIGH); // pin 3 ON
    digitalWrite(4, LOW);  // pin 4 OFF  
    digitalWrite(5, LOW);  // pin 5 OFF
  }else{ // button is pressed
    digitalWrite(3, LOW);  // pin 3 OFF
    digitalWrite(4, LOW);  // pin 4 OFF
    digitalWrite(5, HIGH); // pin 5 ON

    delay(250);            // wait for 1/4 of a secoond
    // toggle the LEDs
    digitalWrite(4,HIGH);  // pin 4 OFF
    digitalWrite(5,LOW);   // pin 4 ON
    delay(250);            // wait for 1/4 of a second
  }

}
