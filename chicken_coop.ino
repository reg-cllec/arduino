// Pin declarations
int enA = 10;
int in1 = 9;
int in2 = 8;
int lightSensor = A0;
int doorClosePin = 2;
int doorOpenPin= 4;
int manualClosePin = 5;
int manualOpenPin = 6;

int steps = 0; // Global variable to keep track of motor steps

void setup() {
  steps = 0;
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(doorClosePin, INPUT_PULLUP);
  pinMode(doorOpenPin, INPUT_PULLUP);
  pinMode(manualClosePin, INPUT_PULLUP);
  pinMode(manualOpenPin, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(manualClosePin) == HIGH || digitalRead(manualOpenPin) == HIGH){
    manual();    // Check manual switches
  } else {
    automatic(); // Call the automatic function
  }
}

void automatic() {  
  if (digitalRead(doorClosePin) == LOW && analogRead(lightSensor) < 500) {
    delay(1000);
    operateDoor(true, false); // Close the door
  } else if ( digitalRead(doorOpenPin) == LOW && analogRead(lightSensor) > 500) {
    delay(1000);
    operateDoor(false, false); // Open the door
  }
}

void operateDoor(bool closeDoorFlag, bool isManual) {
  int dirPin1, dirPin2;

  if (closeDoorFlag) {
    dirPin1 = LOW;
    dirPin2 = HIGH;
  } else {
    dirPin1 = HIGH;
    dirPin2 = LOW;
  }

  while (true) {
    // Check for manual operation
    if (isManual) {
      if (digitalRead(closeDoorFlag ? manualClosePin : manualOpenPin) == LOW ||
          (closeDoorFlag && digitalRead(doorClosePin) == HIGH) ||
          (!closeDoorFlag && digitalRead(doorOpenPin) == HIGH) ||
          steps == (closeDoorFlag ? 0 : 3)) {
          // Manual operation switch is turned off, stop the motor
          // door close/open switch pin is triggered, or the door is fully closed/opened with full steps, stop the motor
          break;
      }
    } else {
      // Check for automatic operation
      if ((closeDoorFlag && digitalRead(doorClosePin) == HIGH) ||
          (!closeDoorFlag && digitalRead(doorOpenPin) == HIGH) ||
          steps == (closeDoorFlag ? 0 : 3)) {
        // door close/open switch pin is triggered, or the door is fully closed/opened with full steps, stop the motor
        break;
      }
    }

    // Continue running the motor
    analogWrite(enA, 255);
    digitalWrite(in1, dirPin1);
    digitalWrite(in2, dirPin2);
    delay(1000); // Adjust the delay as needed based on your motor's speed

    // Update the steps based on the motor direction
    steps += closeDoorFlag ? -1 : 1;

    // Safety check to prevent motor from running indefinitely
    if (steps < 0) {
      steps = 0;
      break; // Prevent negative step values
    } else if (steps > 3) {
      steps = 3;
      break; // Prevent step values greater than 3
    }
  }

  // Stop the motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(3000);
}

void manual() {
  int manualCloseSwitchState = digitalRead(manualClosePin);
  int manualOpenSwitchState = digitalRead(manualOpenPin);

  if (manualCloseSwitchState == HIGH && manualOpenSwitchState == LOW) {
    operateDoor(true, true); // Close the door manually
  } else if (manualOpenSwitchState == HIGH && manualCloseSwitchState == LOW) {
    operateDoor(false, true); // Open the door manually
  }
}
