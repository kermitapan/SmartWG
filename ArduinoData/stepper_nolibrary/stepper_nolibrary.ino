// defines pins
#define stepPin 45
#define dirPin 46 
#define enaPin 50 

const int StepsPerRev = 1600;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enaPin,OUTPUT);
  digitalWrite(enaPin, LOW);
}

void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 10*StepsPerRev; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(400);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(400); 
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 10*StepsPerRev; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(400);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(400);
  }
  delay(1000);
}
