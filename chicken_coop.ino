// Pin declarations
int enA = 10;
int in2 = 9;
int in1 = 8;
int lightSensor = A0;
int doorClosePin = 2;
int doorOpenPin= 3;
int manualClosePin = 4;
int manualOpenPin = 5;

int steps = 0; // Global variable to keep track of motor steps
int maxSteps = 15;
int lightSensorCloseThreashold = 200;
int lightSensorOpenThreashold = 800;
int lightSensorCounter = 0;
int lightSensorDelaySeconds = 0;

void setup() {

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

  if (digitalRead(doorClosePin) == LOW && analogRead(lightSensor) < lightSensorCloseThreashold) {
    delay(1000);
    lightSensorCounter = 0;
    operateDoor(true, false); // Close the door
  } else if ( digitalRead(doorOpenPin) == LOW && analogRead(lightSensor) > lightSensorOpenThreashold) {
    delay(1000);
    lightSensorCounter = 0;
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
          steps == (closeDoorFlag ? 0 : maxSteps)) {
          // Manual operation switch is turned off, stop the motor or
          // door close/open switch pin is triggered, or the door is fully closed/opened with full steps, stop the motor
          break;
      }
    } else {
      // Check for automatic operation
      if ((closeDoorFlag && digitalRead(doorClosePin) == HIGH) ||
          (!closeDoorFlag && digitalRead(doorOpenPin) == HIGH) ||
          steps == (closeDoorFlag ? 0 : maxSteps)) {
        // door close/open switch pin is triggered, or the door is fully closed/opened with full steps, stop the motor
        break;
      }
    }

    // Continue running the motor
    analogWrite(enA, 255);
    digitalWrite(in1, dirPin1);
    digitalWrite(in2, dirPin2);
    delay(500); // keep motor running for xxx sec, Adjust the delay as needed based on your motor's speed

    analogWrite(enA, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    delay(500); // pause motor for 1 sec

    // Update the steps based on the motor direction
    steps += closeDoorFlag ? -1 : 1;

    // Safety check to prevent motor from running indefinitely
    if (steps < 0) {
      steps = 0;
      break; // Prevent negative step values
    } else if (steps > maxSteps) {
      steps = maxSteps;
      break; // Prevent step values greater than maxSteps
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
    steps = maxSteps;
    operateDoor(true, true); // Close the door manually
  } else if (manualOpenSwitchState == HIGH && manualCloseSwitchState == LOW) {
    steps = 0;
    operateDoor(false, true); // Open the door manually
  }
}
