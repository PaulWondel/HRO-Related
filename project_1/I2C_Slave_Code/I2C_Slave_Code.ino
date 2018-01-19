#include <Wire.h>

// Pins shiftregister
int clockPin = 11;
int dataPin = 12;
int latchPin = 10;

// Set debug mode with Serial
const boolean DEBUG_MODE = true;

// For buttons to request the lift
const int BUTTON_UP_PIN = 2;
const int BUTTON_DOWN_PIN = 4;
const int BUTTON_UP_LED_PIN = 3;
const int BUTTON_DOWN_LED_PIN = 5;
int goingUpRequested = 0;
int goingDownRequested = 0;

// For LED to simulate door opening/closing
const int DOOR_LED_PIN = 6;

// For ir detect lift arrived
// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-ir-obstacle-sensor-tutorial-and-manual/
const int IR_PIN = 8;
int liftArrived = 0;      // false

// For displaying lift position on LED display
int liftPosition = 0;
byte liftDisplay = 03;

// For receiving command for save to open door
int openDoor = 0;

// For data to send to master
byte sendingData[2];

int mijnVerdieping = 0;

void setup() {
  if (DEBUG_MODE) {
    Serial.begin(9600);                     // Begin serial communication
  }

  // For get lift
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  // For led buttons
  pinMode(BUTTON_UP_LED_PIN, OUTPUT);
  pinMode(BUTTON_DOWN_LED_PIN, OUTPUT);

  // For LED to simulate door opening/closing
  pinMode(DOOR_LED_PIN, OUTPUT);

  // For ir detect lift arrived
  pinMode(liftArrived, INPUT_PULLUP);

  // For Display shiftregister
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  // For led door
  pinMode(IR_PIN, INPUT_PULLUP);

  // For initialization
  int address = determineAddress();         // Run initialization to determine the location of the segment
  Serial.print("address: ");
  Serial.println(address);

  // For communication
  Wire.begin(address + 8);                  // Start I2C communication // https://www.arduino.cc/en/Reference/Wire inside NOTE: addresses should start from 8
  Wire.onReceive(getLiftRelatedData);       // Set the event handler to handle data incoming from the master
  Wire.onRequest(sendDataToMaster);         // Set the event handler to hande data sending to the master
}

void loop() {
  readSensor();
  button();
  displayDigit(liftPosition);
  if(openDoor && (liftPosition == mijnVerdieping)) {
    digitalWrite(DOOR_LED_PIN, HIGH);
  }
  Serial.print(String(sendingData[0]));
  Serial.println(String(sendingData[1]));
  delay(200);
}

void readSensor() {
  if (digitalRead(IR_PIN) == LOW) {
    sendingData[0] = true ;
    Serial.println("Sensor = 1");
  } 
  else {
    sendingData[0] = false;
    Serial.println("Sensor = 0");
  }
}

void button() {

  // Read sensor when buttons aren't pushed yet
  if (goingUpRequested == LOW) {
    if (digitalRead(BUTTON_UP_PIN) == LOW) {
      goingUpRequested = HIGH;
      digitalWrite(BUTTON_UP_LED_PIN, HIGH);
    }
  }

  if (goingDownRequested == LOW) {
    if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
      goingDownRequested = HIGH;
      digitalWrite(BUTTON_DOWN_LED_PIN, HIGH);
    }
  }

  //Update sending data
  if (goingUpRequested == LOW && goingDownRequested == LOW) {
    sendingData[1] = 0;
  } else if (goingUpRequested == HIGH && goingDownRequested == LOW) {
    sendingData[1] = 1;
  } else if (goingUpRequested == LOW && goingDownRequested == HIGH) {
    sendingData[1] = 2;
  } else {
    sendingData[1] = 3;
  }
}

void updateShiftRegister()
{
  //Zet de latchpin op low zodat er data kan worden doorgegeven
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, liftDisplay);
  digitalWrite(latchPin, HIGH);
}

void displayDigit(int liftPosition)
{
  switch (liftPosition) {
    case (0):
      liftDisplay = 3;
      updateShiftRegister();
      break;
    case (1):
      liftDisplay = 159;
      updateShiftRegister();
      break;
    case (2):
      liftDisplay = 37;
      updateShiftRegister();
      break;
    case (3):
      liftDisplay = 13;
      updateShiftRegister();
      break;
    case (4):
      liftDisplay = 153;
      updateShiftRegister();
      break;
    case (5):
      liftDisplay = 73;
      updateShiftRegister();
      break;
    case (6):
      liftDisplay = 65;
      updateShiftRegister();
      break;
    case (7):
      liftDisplay = 31;
      updateShiftRegister();
      break;
    case (8):
      liftDisplay = 1;
      updateShiftRegister();
      break;
    case (9):
      liftDisplay = 9;
      updateShiftRegister();
      break;
  }
}

/*********************** I2C CODE ***********************/

// NOTE: Dont add Serial.prints or complex code in I2C functions.
void getLiftRelatedData(int numBytes) {
  // Get liftPosition and show it on LED display
  liftPosition = Wire.read();           // Receive byte containing current lift position
  openDoor = Wire.read();               // Receive byte containing whether the door should open
}

void sendDataToMaster() {
  // Send goingUpButton, goingDownButton and liftArrived (ir data)
  Wire.write(sendingData, 2);
}

/*********************** INITIALIZE FLOOR CODE ***********************/

int determineAddress() {
  // Initializes the segment by determinging the location of the segment
  mijnVerdieping = 0;                                            // Set current selected location on 0
  // Show the currenly selected location on the led display 
  displayDigit(mijnVerdieping);
  while (true) {
    if (DEBUG_MODE) {
      Serial.print("selectedFloor: ");
      Serial.println(mijnVerdieping);
    }
    keepReadingButtonUpAndDownUntilPressed();                        // Read the buttons on the segment
    if (goingUpRequested) {                                      // If the floor up button is pressed
      if (DEBUG_MODE) {
        Serial.println("goingUpButtonPressed");
      }
      mijnVerdieping++;                                              // Increase the currently selected floor by one
      goingUpRequested = 0;                                      // Reset the floor up button
      if (mijnVerdieping > 9) {                                      // If the currently selected floor is more then 9
        mijnVerdieping = 0;                                          // Reset the currently selected floor to 0
      }
      // Show the currenly selected location on the led display 
      displayDigit(mijnVerdieping);
    }
    else if (goingDownRequested) {                               // If the floor down button is pressed
      if (DEBUG_MODE) {
        Serial.println("Floor selected (goingDownButtonPressed)");
      }
      // Turn off led display to show successful initialization
      liftDisplay = 255;
      updateShiftRegister();
      delay(2000);                                                   // Wait 2 seconds
      goingDownRequested = 0;                                    // Reset the floor down button
      return mijnVerdieping;                                         // Return the selected floor
    }
    delay(1000);
  }
}

void keepReadingButtonUpAndDownUntilPressed() {
  // Reads the buttons until the are pressed
  if (!goingUpRequested) {                              // If button up is not pressed
    goingUpRequested = !digitalRead(BUTTON_UP_PIN);      // Read button up
  }
  if (!goingDownRequested) {                            // If button down is not pressed
    goingDownRequested = !digitalRead(BUTTON_DOWN_PIN);  // Read button down
  }
}
