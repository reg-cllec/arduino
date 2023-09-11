#include <Stepper.h>
const int stepsPerRevolution=100;
Stepper myStepper(stepsPerRevolution,8,9,10,11);
void setup() {
  // put your setup code here, to run once:
  myStepper.setSpeed(300);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ClockWise");
  myStepper.step(stepsPerRevolution);
  delay(1000);

  Serial.println("CounterClockWise");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
}
