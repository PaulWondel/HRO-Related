#include <Servo.h>

Servo rightservo;
Servo leftservo;  // create servo object to control a servo
                // twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position

int sensorPin = 5; // select the input pin for LDR //PIN D1
int sensorPin2 = 4;//PIN D2
int sensorValue = 0; // variable to store the value coming from the sensor
int sensorValue2 = 0;


void setup() {
  Serial.begin(9600);
  leftservo.attach(0);  // attaches the servo on the pin to the servo object
                      // PIN D3
  rightservo.attach(2);

}

void loop(){
  sensor();
  sweepLeftWheel();
  sweepRightWheel();
  
}

void sweepLeftWheel() {
 for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    leftservo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println("Naar achteren");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    leftservo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println("Naar voren");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void sweepRightWheel() {
 for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    rightservo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println("Naar achteren");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    rightservo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println("Naar voren");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void sensor() {
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  sensorValue2 = analogRead(sensorPin2);
  Serial.print(sensorValue); //prints the values coming from the sensor on the screen
  Serial.print(" & ");
  Serial.print(sensorValue2);
  Serial.println("");
  delay(100); 
} 
