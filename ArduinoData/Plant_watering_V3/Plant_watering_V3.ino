
// Define pins for sensors and actuators
#define ultrasoundTriggerPin 3
#define ultrasoundEchoPin 1
#define soilMoistureSensorPin 36  // Analog pin
#define photoresistorPin 25       // Analog pin for light sensing

#define buzzerPin 12
#define pumpPin 4  // BJT connected to a digital pin

// Threshold values (customize these based on your requirements)
const int soilDryThreshold = 3000;            // Threshold for soil moisture
const int eveningLightThreshold = 400;       // Threshold for low light level
const float minWaterSurfaceDistance = 15.0;  // Minimum water level in cm

// Array to store averaged light levels
int lightLevels[4] = {0, 0, 0, 0};
unsigned long lastMeasurementTime = 0;
const long measurementInterval = 10 * 60 * 1000; // 10 minutes interval
const int numSamples = 10; // Number of samples for averaging
const int increaseThreshold = 50; // Threshold for detecting significant increase

//Irrigation Variables
const long irrigationInterval =  20 * 60 * 60 * 1000; // 20 hours irrigation block 
unsigned long lastIrrigationTime = 0;

//Buzzer Interval
const long buzzInterval = 10 * 60 * 1000;
unsigned long lastBuzzTime = 0;

// Function prototypes
float getWaterLevel();
int getSoilMoisture();
int getLightLevel();

bool isBucketEmpty();
bool isDay();
bool isMorning();
bool isDry();

void pumpWater();
void buzz();
void updateLightLevels();


void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Setup pin modes
  pinMode(ultrasoundEchoPin, INPUT);
  pinMode(soilMoistureSensorPin, INPUT);
  pinMode(photoresistorPin, INPUT);

  pinMode(ultrasoundTriggerPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Ensure pump is off initially
  digitalWrite(pumpPin, LOW);
}

void loop() {

  unsigned long currentMillis = millis();

  updateLightLevels();
  if (currentMillis - lastMeasurementTime >= measurementInterval){
    updateLightLevels();
    lastMeasurementTime = currentMillis;
  }

  while(isBucketEmpty()){
    if (currentMillis - lastBuzzTime >= buzzInterval && isDay()){  // Dont wake people up
      buzz();
      lastBuzzTime = currentMillis;
    }
  }

  if (currentMillis - lastIrrigationTime >= irrigationInterval && isMorning() && isDry()){
    pumpWater(5);
    lastIrrigationTime = currentMillis;
  }

  //delay(10*60*1000); // Loop Period
  delay(1000);
}

// Define Functions here

void updateLightLevels() {
  int sum = 0;

  // Take multiple samples and calculate the average
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(photoresistorPin);
    delay(50); // Delay to allow some time between samples
  }
  int average = sum / numSamples;

  // Shift all elements one position to the left
  for (int i = 0; i < 3; i++) {
    lightLevels[i] = lightLevels[i + 1];
  }
  // Update the last element with the current averaged light level
  lightLevels[3] = average;
}


void buzz() {  //3 short beeps, pause

  for (int l = 0; l < 2; l++) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
    delay(400);
  }
}

bool isDry() {
  if (getSoilMoisture() > soilDryThreshold){
    return true;
  }
  return false;
}

bool isDay() {
  if(lightLevels[3] > eveningLightThreshold){
    return true;
  }
  else{
    return false; 
  }

}

bool isMorning() {
 // Check if there is a significant natural increase in the light levels
  for (int i = 0; i < 3; i++) {
    if (lightLevels[i+1] - lightLevels[i] < increaseThreshold){
      return false; // If any increase is below the threshold, it's not morning
    }
  }
  return true; // All increases are above the threshold
}

bool isBucketEmpty() {
  if (getWaterLevel() > minWaterSurfaceDistance) {
    return true;
  }
  return false;
}

float getWaterLevel() {
  // Trigger ultrasound sensor
  digitalWrite(ultrasoundTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasoundTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasoundTriggerPin, LOW);

  // Read the echo pin
  long duration = pulseIn(ultrasoundEchoPin, HIGH);
  float distance = duration * 0.034 / 2;  // Calculate distance
  return distance;
}

int getSoilMoisture() {
  return analogRead(soilMoistureSensorPin);
}

int getLightLevel() {
  return analogRead(photoresistorPin);
}

void pumpWater(int minutes){

  unsigned long startMillis = millis();
  unsigned long currentMillis = startMillis;
  digitalWrite(pumpPin, HIGH);

  while(currentMillis - startMillis < minutes * 60 * 1000) {

    if(isBucketEmpty()){
      digitalWrite(pumpPin, LOW);
      break;
    }

    delay(1000);
    currentMillis = millis();
  }

  digitalWrite(pumpPin, LOW);
}
