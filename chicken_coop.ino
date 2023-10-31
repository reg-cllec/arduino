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
int maxSteps = 20;
int lightSensorCloseThreashold = 100;// delay 30 minutes after 100;
int lightSensorOpenThreashold = 400; //夏天(晴天800, 阴天500) 冬天(晴天400, 阴天 200)
int lightSensorCounter = 0;
int lightSensorDelaySeconds = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello setup");
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(doorClosePin, INPUT_PULLUP);
  pinMode(doorOpenPin, INPUT_PULLUP);
  pinMode(manualClosePin, INPUT_PULLUP);
  pinMode(manualOpenPin, INPUT_PULLUP);
}

void loop() {
  Serial.println(analogRead(lightSensor));
  // Serial.println(lightSensorCounter);
  // Serial.println(steps);

  // Serial.print("doorClosePin:");
  // Serial.println(digitalRead(doorClosePin));
  // Serial.print("doorOpenPin:");
  // Serial.println(digitalRead(doorOpenPin));
  // Serial.print("manualClosePin:");
  // Serial.println(digitalRead(manualClosePin));
  // Serial.print("manualOpenPin:");
  // Serial.println(digitalRead(manualOpenPin));

  if(digitalRead(manualClosePin) == HIGH || digitalRead(manualOpenPin) == HIGH){
    Serial.println("manual");
    manual();    // Check manual switches
  } else {
    Serial.println("automatic");
    automatic(); // Call the automatic function
  }
}

void automatic() {  
  delay(1000);
  // delay xxx minutes before close door
  Serial.println(analogRead(lightSensor));
  lightSensorCounter = analogRead(lightSensor) < lightSensorCloseThreashold ? lightSensorCounter + 1 : lightSensorCounter;
  lightSensorCounter = analogRead(lightSensor) > lightSensorOpenThreashold ? 0 : lightSensorCounter;
  if (digitalRead(doorClosePin) == LOW && lightSensorCounter > 1800) {
    lightSensorCounter = 0;
    operateDoor(true, false); // Close the door
  } else if ( digitalRead(doorOpenPin) == LOW && analogRead(lightSensor) > lightSensorOpenThreashold) {
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
    // Check for manual operation or automatic
    if (isManual) {
      if (digitalRead(closeDoorFlag ? manualClosePin : manualOpenPin) == LOW || // open or close switch released
          (closeDoorFlag && digitalRead(doorClosePin) == HIGH) || // door is fully closed and the limit switch is triggered
          (!closeDoorFlag && digitalRead(doorOpenPin) == HIGH) || // door is fully opened and the limit switch is triggered
          steps == (closeDoorFlag ? 0 : maxSteps)) { // steps reach limit
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
    delay(500); // pause motor for xxx sec

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
  delay(1000);
}

void manual() {
  delay(1000);
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
