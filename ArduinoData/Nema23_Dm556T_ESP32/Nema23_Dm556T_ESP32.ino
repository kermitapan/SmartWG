#include <Arduino.h>
#include <Stepper.h>

const int stepPin = 22;  // Connect to DM556T PUL-
const int dirPin = 24;    // Connect to DM556T DIR-
const int enablePin = 26;  // Connect to DM556T ENA-
const int stepsPerRevolution = 4*800;

// Initialize the Stepper library
Stepper myStepper(stepsPerRevolution, stepPin, dirPin);


void setup() {
  // Set motor control pins as outputs
  // Set pin modes
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);  // Enable the driver (if needed)
  delay(1000);

   // Set the speed (rpm)
  myStepper.setSpeed(30);  // Set the speed to 60 RPMs. Adjust as necessary.

}



void loop() {

  // Step forward 360 degrees (full rotation)
  myStepper.step(stepsPerRevolution);

  // Wait a second
  delay(2000);

  // Step backward 360 degrees
  myStepper.step(-stepsPerRevolution);

  // Wait a second
  delay(2000);
 
}

