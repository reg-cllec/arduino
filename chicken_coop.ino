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
  if (digitalRead(autoClosePin) == LOW && analogRead(lightSensor) < 800) {
    delay(1000);
    operateDoor(true); // Close the door
  } else if ( digitalRead(autoOpenPin) == LOW && analogRead(lightSensor) > 800) {
    delay(1000);
    operateDoor(false); // Open the door
  }
}

void operateDoor(bool closeDoorFlag) {
  int dirPin1, dirPin2;

  if (closeDoorFlag) {
    dirPin1 = LOW;
    dirPin2 = HIGH;
  } else { // closeDoorFlag == false
    dirPin1 = HIGH;
    dirPin2 = LOW;
  }

  // Check for the opposite condition when closeDoorFlag is false
  while (digitalRead(closeDoorFlag ? autoClosePin : autoOpenPin) != HIGH) {
    analogWrite(enA, 255);
    digitalWrite(in1, dirPin1);
    digitalWrite(in2, dirPin2);
    delay(1000);
  }
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void manual() {
  int manualCloseSwitchState = digitalRead(manualClosePin);
  int manualOpenSwitchState = digitalRead(manualOpenPin);

  if (manualCloseSwitchState == HIGH && manualOpenSwitchState == LOW) {
    operateDoor(true); // Close the door manually
  } else if (manualOpenSwitchState == HIGH && manualCloseSwitchState == LOW) {
    operateDoor(false); // Open the door manually
  }
}
