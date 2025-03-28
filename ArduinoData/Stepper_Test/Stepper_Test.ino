#include <Stepper.h>

const int steps_per_rev = 200; //Set to 200 for NIMA 17
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

Stepper motor(steps_per_rev, IN1, IN2, IN3, IN4);


void setup()
{
  motor.setSpeed(120);
  Serial.begin(115200);
}

void loop() 
{
  Serial.println("Rotating Clockwise...");
  motor.step(steps_per_rev);
  delay(2000);

  Serial.println("Rotating Anti-clockwise...");
  motor.step(-steps_per_rev);
  delay(2000);
}