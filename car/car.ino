/*
 * Car control system. 
 * Nick J Adams
 * 
 * The motors are connected to two H-Bridge 1A Motor drivers (SN754410), 
 * the ICs are connected in parallel on the left and on the right. The 
 * first chip powers the front left motor and the front right motor, the 
 * second chip powers the rear left motor and the rear right motor. 
 * In this control system the two left motors and two right motors are 
 * controled together. This code therefore works equally well for both
 * two and four wheeled vehicles.
 * 
 * The code makes use of PciManager (https://code.google.com/p/arduino-pcimanager/)
 * to handle interrupts.   
 */

#include <PciManager.h>
#include <PciListenerImp.h>
 
// Pin assignments
int led = 13;
int speedLeftPin = 3;
int speedRightPin = 6;
int controlOneA = 4;
int controlTwoA = 5;
int controlThreeA = 7;
int controlFourA = 8;
int frontLeftBumper = 9; // Need to confirm pin
int frontRightBumber = 10; // Need to confirm pin
int rearLeftBumper = 11; // Need to confirm pin
int rearRightBumber = 12; // Need to confirm pin

// Global Variables
int currentSpeed = 0;

// Interrupts
PciListenerImp frontLeftListener(frontLeftBumper, frontLeftHit);
PciListenerImp frontRightListener(frontRightBumper, frontRightHit);
PciListenerImp rearLeftListener(rearLeftBumper, rearLeftHit);
PciListenerImp rearRightListener(rearRightBumper, rearRightHit);


/*
 * This method is run once when the Arduino is powered or
 * it's reset putton is pressed. 
 * 
 * This method configures mode of the pins, it ensures all the motors 
 * start in a stopped state and it assignes an initial value to the 
 * speed variable. 
 */
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

  // Configure the Interrupts
  PciManager.registerListener(frontLeftBumper, &frontLeftListener);
  PciManager.registerListener(frontRightBumber, &frontRightListener);
  PciManager.registerListener(rearLeftBumber, &rearLeftListener);
  PciManager.registerListener(rearRightBumper, &rearRightListener);
}

/*
 * This is the main guts of the program, it will run this method over 
 * and over until the power is removed or the reset puttion is pressed. 
 *
 * This is where the behaviour of the car is defined. it makes use of the delay 
 * function to carryout the specified behaviour for the given amount of time. 
 */
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
  if (speed > 255){
    currentSpeed = 255;
  } else if (speed <= 0){
    currentSpeed = 0;
  } else {
    currentSpeed = speed;
  }
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
  if (speed > 255){
    currentSpeed = 255;
  } else if (speed <= 0){
    currentSpeed = 0;
  } else {
    currentSpeed = speed;
  }
  digitalWrite(controlOneA, LOW);  
  digitalWrite(controlTwoA, HIGH); 
  digitalWrite(controlThreeA, LOW); 
  digitalWrite(controlFourA, HIGH); 
  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
  digitalWrite(led, HIGH); // Light the LED when the motors are enabled.
}

/*
 * Spin the car right at the specified speed
 */
void spinRight(int speed){
  if (speed > 255){
    currentSpeed = 255;
  } else if (speed <= 0){
    currentSpeed = 0;
  } else {
    currentSpeed = speed;
  }
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
 * spin the car left at the specified speed
 */
void spinLeft(int speed){
  if (speed > 255){
    currentSpeed = 255;
  } else if (speed <= 0){
    currentSpeed = 0;
  } else {
    currentSpeed = speed;
  }
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

/*
 * Increase the speed by 10 (about 4%), will increase speed to maximum 255 then stop. 
 */
void speedUp(){
  if (currentSpeed < 245){
    currentSpeed += 10;
  } else {
    currentSpeed = 255;
  }

  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
}

/* 
 * Decrease the speed by 10 (about 4%), will decrease the speed to 0 then stop. 
 */
void slowDown(){
  if (currentSpeed > 10) {
    currentSpeed -= 10;
  } else {
    currentSpeed = 0;
  }

  analogWrite(speedLeftPin, currentSpeed);
  analogWrite(speedRightPin, currentSpeed);
}

/*
 * An interrupt routine to handle a crash on the front left side of the 
 * vehicle.
 * If the vehicle hits front left then lets move to the right a bit and then continue forward 
 */
void frontLeftHit(byte changeKind) {
  // Stop, reverse, spin right, forward, spin left, forward
  allStop();
  moveBackwards(255);
  delay(1000));			// Check the duration of this
  spinRight(255);
  delay(200);			// Check the duration of this
  moveForward(255);
  delay(500);			// Check the duration of this
  spinLeft(255);	
  delay(200);			// Check the duration of this
  moveForward(255);
  // No delay, assume that if there is a front hit then the car was moving forward. 
}

/*
 * An interrupt routine to handle a crash on the front right side of the
 * vehicle.
 */
void frontRightHit(byte changeKind) {
  // Stop, reverse, spin left, forward, spin right, forward
  allStop();
  moveBackwards(255);
  delay(1000));                 // Check the duration of this
  spinLeft(255);
  delay(200);                   // Check the duration of this
  moveForward(255);
  delay(500);                   // Check the duration of this
  spinRight(255);
  delay(200);                   // Check the duration of this
  moveForward(255);
  // No delay, assume that if there is a front hit then the car was moving forward.

}

/*
 * An interrupt routine to handle a crash on the rear left side of the
 * vehicle.
 */
void rearLeftHit(byte changeKind) {
  // Stop, forward, spin left, reverse, spin right, reverse
  allStop();
  moveForward(255);
  delay(1000));                 // Check the duration of this
  spinLeft(255);
  delay(200);                   // Check the duration of this
  moveBackwards(255);
  delay(500);                   // Check the duration of this
  spinRight(255);
  delay(200);                   // Check the duration of this
  moveBackwards(255);
  // No delay, assume that if there is a rear hit then the car was moving backwards.
}

/*
 * An interrupt routine to handle a crash on the rear right side of the
 * vehicle.
 */
void rearRightHit(byte changeKind) {
  // Stop, forward, spin right, reverse, spin left, reverse
  allStop();
  moveForward(255);
  delay(1000));                 // Check the duration of this
  spinRight(255);
  delay(200);                   // Check the duration of this
  moveBackwards(255);
  delay(500);                   // Check the duration of this
  spinLeft(255);
  delay(200);                   // Check the duration of this
  moveBackwards(255);
  // No delay, assume that if there is a rear hit then the car was moving backwards.
}

