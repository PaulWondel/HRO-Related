int sensorPin = A0; // select the input pin for LDR
int sensorValue = 0; // variable to store the value coming from the sensor

int LED = 5;

void setup() {
  Serial.begin(9600); //sets serial port for communication
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  //Serial.println(sensorValue); //prints the values coming from the sensor on the screen
  kleur();
  delay(100);
}

void kleur() {
  if((sensorValue>=310) && (sensorValue<=330)){
    Serial.println("Zwart");
  }
  else { 
    Serial.println("geen Zwart");
  }
}

