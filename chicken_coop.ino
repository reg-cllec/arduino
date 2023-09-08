
//Locking, Automatic Chicken Coop Door 
//By Seth Johnson  Land To House llc 2016

//This section is for initializing things:

// Initialize the motor values to the pins 8, 9, 10. 
//Pins 8 and 9 send high or low to the motor controller.
//pin 10 enables motor one on the motor controller. 
int enA = 10;
int in1 = 9;
int in2 = 8;


//Initialize "lightSensor" as the value from pin A0 and read in the value. This is the photoresistor.
int lightSensor = analogRead(A0);
//The lightVal will hold the value of lightsensor in this variable
int lightVal = 0;

//These are the pins for the reed switches
// reed1Pin is the lower switch on the door. This is digital pin 2
int reed1Pin = 2;
//reed2Pin is the top switch on the door. This is digital pin 4
int reed2Pin = 4;
//These are the variables that hold the state of the reed switches
int doorClosedSwitchState = 0;
int doorOpenSwitchState = 0;

//This only runs once.
void setup()
{
  // set the motor control pins as outputs. This means pins 8, 9, 10 are outputs to the l298n motor controller.
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
//read the state of switch 1 (the bottom one) and place it in doorClosedSwitchState
doorClosedSwitchState = digitalRead(reed1Pin);
//read the state of switch 2 (the top one) and place it in doorOpenSwitchState
doorOpenSwitchState = digitalRead(reed2Pin);
//this is important to make sure that the door starts up when the program gains power.
//if doorOpenSwitchState is not high then go to the while statement
if (doorOpenSwitchState != HIGH)
{
  // this function will run the motor down as long as switch 1 has not been triggered
    while (doorOpenSwitchState != HIGH) 
    {
      // turn on motor and set speed to 255 
      analogWrite(enA, 255);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      //read the state of the switch again to make sure that it has not been triggered
      doorClosedSwitchState = digitalRead(reed1Pin);
      //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
      doorOpenSwitchState = digitalRead(reed2Pin);   

      delay(2000);
      // once doorOpenSwitchState has been triggered turn off the motor
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      delay(2000);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);

    }
    // once doorOpenSwitchState has been triggered turn off the motor
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

//this runs over and over
void loop() 
{
  //read the light sensor and place it in lightval
  lightVal = analogRead(lightSensor);
  //read the state of switch 1 (the bottom one) and place it in doorClosedSwitchState
  doorClosedSwitchState = digitalRead(reed1Pin);
    //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
  doorOpenSwitchState = digitalRead(reed2Pin);
  //the lightSensor is read and placed into lightVal. if its less than 200 and doorOpenSwitchState is 
  //equal to high then go to the motor down code. But if the light is greater than 200 and the doorClosedSwitchState
  //is equal to high then call motor up code
 if (doorOpenSwitchState = HIGH && lightVal < 800) 
  {
    delay(1000);
    closeDoor();
  } 
  else if (doorClosedSwitchState = HIGH && lightVal > 800)
  {
    delay(1000);
    openDoor();
  }
}

void manual() {

}
void automatic() {
   //read the light sensor and place it in lightval
  lightVal = analogRead(lightSensor);
  //read the state of switch 1 (the bottom one) and place it in doorClosedSwitchState
  doorClosedSwitchState = digitalRead(reed1Pin);
    //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
  doorOpenSwitchState = digitalRead(reed2Pin);
  //the lightSensor is read and placed into lightVal. if its less than 200 and doorOpenSwitchState is 
  //equal to high then go to the motor down code. But if the light is greater than 200 and the doorClosedSwitchState
  //is equal to high then call motor up code
 if (doorOpenSwitchState = HIGH && lightVal < 800) 
  {
    delay(1000);
    closeDoor();
  } 
  else if (doorClosedSwitchState = HIGH && lightVal > 800)
  {
    delay(1000);
    openDoor();
  }
}

void closeDoor()
{
  //Read the state of switch 1 (the Bottom one) and place it in doorClosedSwitchState
  doorClosedSwitchState = digitalRead(reed1Pin);
  //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
  doorOpenSwitchState = digitalRead(reed2Pin);

  //If doorOpenSwitchState is high and the light is dark then continue
  if (doorOpenSwitchState = HIGH && lightVal < 800)
  //wait 2 seconds
  delay(2000);
  //read the state of switch 1 (the bottom one) and place it in doorClosedSwitchState
  doorClosedSwitchState = digitalRead(reed1Pin);
  //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
  doorOpenSwitchState = digitalRead(reed2Pin);
  
  while (doorClosedSwitchState != HIGH) {
    // turn on motor and set speed to 255 
    analogWrite(enA, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW); 
    //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
    doorClosedSwitchState = digitalRead(reed1Pin);
    //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
    doorOpenSwitchState = digitalRead(reed2Pin);  
  }
  //wait 1 second before turning off the motor to let the locks engage at the bottom
  delay(10);
  // now turn off motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void openDoor()
{
  //read the state of switch 1 (the bottom one) and place it in doorOpenSwitchState
  doorClosedSwitchState = digitalRead(reed1Pin);
  //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
  doorOpenSwitchState = digitalRead(reed2Pin);

  //if doorClosedSwitchState is high and the light is bright then continue
  if (doorOpenSwitchState != HIGH && lightVal > 800)
  {

    //read the state of switch 1 (the bottom one) and place it in doorClosedSwitchState
    doorClosedSwitchState = digitalRead(reed1Pin);
    //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
    doorOpenSwitchState = digitalRead(reed2Pin);
    //delay 2 seconds
    delay(2000);
    //while doorOpenSwitchState is not high turn on the motor up
    while (doorOpenSwitchState != HIGH) 
    {
      // this function will run the motor as long as switch 2 has not been triggered
      // turn on motor and set speed to 255 
      analogWrite(enA, 255);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      //read the state of switch 1 (the bottom one) and place it in doorOpenSwitchState
      doorClosedSwitchState = digitalRead(reed1Pin);
      //read the state of switch 2 (the top one) and place it in doorOpenSwitchState
      doorOpenSwitchState = digitalRead(reed2Pin);   
    }
    // now turn off motor
    delay(10);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}