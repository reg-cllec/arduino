// Pin declarations
int enA = 10;
int in1 = 9;
int in2 = 8;
int lightSensor = A0;
int autoClosePin = 2;
int autoOpenPin= 4;
int manualClosePin = 5;
int manualOpenPin = 6;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(autoClosePin, INPUT_PULLUP);
  pinMode(autoOpenPin, INPUT_PULLUP);
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
  if (digitalRead(autoClosePin) == LOW && analogRead(lightSensor) < 500) {
    delay(1000);
    operateDoor(true, false); // Close the door
  } else if ( digitalRead(autoOpenPin) == LOW && analogRead(lightSensor) > 500) {
    delay(1000);
    operateDoor(false, false); // Open the door
  }
}

void operateDoor(bool closeDoorFlag, bool isManual) {
  int dirPin1, dirPin2;

  if (closeDoorFlag) {
    dirPin1 = LOW;
    dirPin2 = HIGH;
  } else { // closeDoorFlag == false
    dirPin1 = HIGH;
    dirPin2 = LOW;
  }

  int counter =0;
  if (isManual) {
    // Check for the opposite condition when closeDoorFlag is false
    while (digitalRead(closeDoorFlag ? autoClosePin : autoOpenPin) != HIGH && digitalRead(closeDoorFlag ? manualClosePin : manualOpenPin) != LOW && counter < 3) {
      analogWrite(enA, 255);
      digitalWrite(in1, dirPin1);
      digitalWrite(in2, dirPin2);
      counter++;
      delay(1000);
    }
  } else {
    // Check for the opposite condition when closeDoorFlag is false
    while (digitalRead(closeDoorFlag ? autoClosePin : autoOpenPin) != HIGH && counter < 3) {
      analogWrite(enA, 255);
      digitalWrite(in1, dirPin1);
      digitalWrite(in2, dirPin2);
      counter ++;
      delay(1000);
    }
  }
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
