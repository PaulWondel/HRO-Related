int sensorPin = A0; // select the input pin for LDR
int sensorPin2 = A5;
int sensorValue = 0; // variable to store the value coming from the sensor
int sensorValue2 = 0;

void setup() {
  Serial.begin(9600); //sets serial port for communication
}

void loop() {
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  sensorValue2 = analogRead(sensorPin2);
  Serial.print(sensorValue); //prints the values coming from the sensor on the screen
  Serial.print(" & ");
  Serial.print(sensorValue2);
  Serial.println("");
  delay(100); 
} 
