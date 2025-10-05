
// import Servo library
#import <Servo.h>

// Create a servo object
Servo myServo;

// Initializa potentionmeter pin and its value
int const potPin = A0;
int potVal;

// create angle variable for servo motot
int angle;

void setup() {


  // put your setup code here, to run once:
  // attach servo object to pin #9 to recieve information on how it should  move
  myServo.attach(9);


  // begin serial monitor at 9600 bit/sec
  Serial.begin(9600);


}

void loop() {


  // put your main code here, to run repeatedly:
  // read potentiomneter value and print it to serial monitor
  potVal = analogRead(potPin);
  Serial.print("potVal: ");
  Serial.print(potVal);


  // calculate angle with pot val as the range to be superset a
  angle = map(potVal, 0, 1023, 0 ,179);
  Serial.print(", angle: "); // print angle to serial monitor
  Serial.print(angle);
  Serial.print("\n");

  // move the servo angle degrees every 5 milliseconds update position
  myServo.write(angle);
  delay(5);

}
