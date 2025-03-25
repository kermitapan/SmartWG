
// Define pins for sensors and actuators
const int ultrasoundTriggerPin = 7;
const int ultrasoundEchoPin = 8;
const int soilMoistureSensorPin = A0; // Analog pin
const int pumpPin = 9; // BJT connected to a digital pin
const int photoresistorPin = A1; // Analog pin for light sensing

// RTC setup

// Threshold values (customize these based on your requirements)
const int soilDryThreshold = 300; // Threshold for soil moisture
const int eveningLightThreshold = 400; // Threshold for low light level
const float minWaterSurfaceDistance = 30.0; // Minimum water level in cm

// Function prototypes
float getWaterLevel();
int getSoilMoisture();
int getLightLevel();
void checkAndWaterPlant();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Setup pin modes
  pinMode(ultrasoundTriggerPin, OUTPUT);
  pinMode(ultrasoundEchoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(soilMoistureSensorPin, INPUT);
  pinMode(photoresistorPin, INPUT);

  // Ensure pump is off initially
  digitalWrite(pumpPin, LOW);
}

void loop() {
  checkAndWaterPlant();
  delay(10000); // Check every 10 seconds
}

void checkAndWaterPlant() {
  float waterLevel = getWaterLevel();
  int soilMoisture = getSoilMoisture();
  int lightLevel = getLightLevel();

  // Check if it's the right time to water
  //DateTime now = rtc.now();
  bool isEvening = (lightLevel < eveningLightThreshold);

  // Watering conditions: soil is dry, enough water, and it's evening
  if (soilMoisture < soilDryThreshold && waterLevel < minWaterLevel && isEvening) {
    digitalWrite(pumpPin, HIGH); // Start the pump
    delay(2*60*1000); // Run pump for 2 mins
    digitalWrite(pumpPin, LOW); // Stop the pump
    Serial.println("Watered the plant.");
  } else {
    Serial.println("No watering needed.");
  }
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
  float distance = duration * 0.034 / 2; // Calculate distance
  return distance;
}

int getSoilMoisture() {
  return analogRead(soilMoistureSensorPin);
}

int getLightLevel() {
  return analogRead(photoresistorPin);
}
