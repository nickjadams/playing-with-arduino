/*
 * Car control system.  
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

int speedLeftPin = 3;
int speedRightPin = 6;

int controlOneA = 4;
int controlTwoA = 5;
int controlThreeA = 7;
int controlFourA = 8;

int currentSpeed = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(speedLeftPin, OUTPUT);
  pinMode(controlOneA, OUTPUT);  
  pinMode(controlTwoA, OUTPUT);
  pinMode(speedRightPin, OUTPUT);
  pinMode(controlThreeA, OUTPUT);
  pinMode(controlFourA, OUTPUT);

  // Disable the motors on startup
  allStop();
  
  // reset the speed to maximum but dont start the motors.
  currentSpeed=255;
}

// the loop routine runs over and over again forever:
void loop() {
  moveForward(currentSpeed);
  delay(1000);                   
  //speedUp();  
  moveBackwards(currentSpeed);
  delay(1000);
  spinRight(currentSpeed);
  delay(1000);
  spinLeft(currentSpeed);
  delay(1000);
  allStop();
  delay(1000);
  currentSpeed = 255;
}


/*
 * Move forward at the specified speed. If insurficient 
 * speed is provided then the motor will stall. 
 */
void moveForward(int speed){
  currentSpeed = speed;
  digitalWrite(controlOneA, HIGH);  
  digitalWrite(controlTwoA, LOW); 
  digitalWrite(controlThreeA, HIGH); 
  digitalWrite(controlFourA, LOW); 
  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
  digitalWrite(led, HIGH); // Light the LED when the motors are enabled.
}

/*
 * Move backwards at the specified speed. If insurficient 
 * speed is provided then the motor will stall. 
 */
void moveBackwards(int speed){
  currentSpeed = speed;
  digitalWrite(controlOneA, LOW);  
  digitalWrite(controlTwoA, HIGH); 
  digitalWrite(controlThreeA, LOW); 
  digitalWrite(controlFourA, HIGH); 
  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
  digitalWrite(led, HIGH); // Light the LED when the motors are enabled.
}

/*
 * Spin the car right
 */
void spinRight(int speed){
  currentSpeed = speed;
  analogWrite(speedLeftPin, 0);
  analogWrite(speedRightPin, 0);
  digitalWrite(controlOneA, HIGH);  
  digitalWrite(controlTwoA, LOW); 
  digitalWrite(controlThreeA, LOW); 
  digitalWrite(controlFourA, HIGH); 
  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
  digitalWrite(led, HIGH); // Light the LED when the motors are enabled.
}

/*
 * spin the car left
 */
void spinLeft(int speed){
  currentSpeed = speed;
  analogWrite(speedLeftPin, 0);
  analogWrite(speedRightPin, 0);
  digitalWrite(controlOneA, LOW);  
  digitalWrite(controlTwoA, HIGH); 
  digitalWrite(controlThreeA, HIGH); 
  digitalWrite(controlFourA, LOW); 
  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
  digitalWrite(led, HIGH); // Light the LED when the motors are enabled.
}
 
/* 
 * Stop the motors
 */
void allStop(){
  currentSpeed = 0;
  analogWrite(speedLeftPin, 0);
  analogWrite(speedRightPin, 0);
  digitalWrite(led, LOW);
}

void speedUp(){
  if (currentSpeed < 245){
    currentSpeed += 10;
  } else {
    currentSpeed = 255;
  }

  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
}

void slowDown(){
  if (currentSpeed > 10) {
    currentSpeed -= 10;
  } else {
    currentSpeed = 0;
  }

  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
}
