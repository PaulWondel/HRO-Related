#include <Servo.h>

Servo servoLeft;          // Define left servo
Servo servoRight;         // Define right servo

int sensorPin = A0; // select the input pin for LDR
int sensorValue = 0; // variable to store the value coming from the sensor

int LED = 5;

int a = 190, b = 210;
boolean colorState;

void setup() { 
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  //servoLeft.attach(2);  // Set left servo to digital pin 10
  //servoRight.attach(3);  // Set right servo to digital pin 9
  enableServo();
} 

void loop() {
  digitalWrite(LED, HIGH);
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  Serial.println(sensorValue); //prints the values coming from the sensor on the screen
  kleur();
  
  if(colorState == true){
    drive();
  }
  else {
    disableServo();
  }
  delay(100);
}

boolean kleur() {
  if((sensorValue>=a) && (sensorValue<=b)){
    Serial.println("Zwart");
    colorState = true;
    return colorState;
  }
  else { 
    Serial.println("geen Zwart");
    colorState = false;
    return colorState;
  }
}

void drive() {            // Loop through motion tests
  forward();             // Example: move forward
  delay(2000);           // Wait 2000 milliseconds (2 seconds)
  reverse();
  delay(2000);
  turnRight();
  delay(1000);
  turnLeft();
  delay(1000);
  stopRobot();
  delay(2000);
}

// Motion routines for forward, reverse, turns, and stop
void forward() {
  enableServo();
  servoLeft.write(0);
  servoRight.write(180);
  Serial.println("Naar Voren");
}

void reverse() {
  enableServo();
  servoLeft.write(180);
  servoRight.write(0);
  Serial.println("Naar Achteren");
}

void turnRight() {
  enableServo();
  servoLeft.write(180);
  servoRight.write(180);
  Serial.println("Naar Rechts");
}
void turnLeft() {
  servoLeft.write(0);
  servoRight.write(0);
  Serial.println("Naar Links");
}

void stopRobot() {
  //servoLeft.write(90);
  //servoRight.write(90);
  Serial.println("Stop");
  disableServo();
}

void enableServo(){
  servoLeft.attach(2);  // powers servo object
  servoRight.attach(3);
}

void disableServo(){
  servoLeft.detach();    //removes the servo object, effectively disabling the servo and removing its power.
  servoRight.detach();   //emoves the servo object, effectively disabling the servo and removing its power.
}

