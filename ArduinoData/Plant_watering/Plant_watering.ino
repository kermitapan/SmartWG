// Adafruit_DHT library is inserted
#include "DHT.h"

// Define the pin numbers
const int waterSensorPin = A5; // The digital pin where the water sensor is connected
const int pumpRelayPin = 4;   // The digital pin controlling the relay for the pump

// Here the respective input pin can be declared
#define DHTPIN 2     
 
// The sensor is initialized
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int h;
int t;

void setup() {
  pinMode(waterSensorPin, INPUT);   // Set the water sensor pin as an input
  pinMode(pumpRelayPin, OUTPUT);    // Set the relay pin as an output

  // Measurement is started
  dht.begin();

  digitalWrite(pumpRelayPin, LOW);  // Ensure pump is off initially
  Serial.begin(9600);
}

void loop() {

   // Humidity is measured
  h = dht.readHumidity();
  // temperature is measured
  t = dht.readTemperature();

  // Checking if the measurements have passed without errors
  // if an error is detected, a error message is displayed here
  if (isnan(h) || isnan(t)) {
    Serial.println("Error reading the sensor");
    return;
  }

  // Read the water level
  int waterLevel = analogRead(waterSensorPin); 

 
  // Output to serial console
  Serial.println("-----------------------------------------------------------");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.println("Temperature: ");
  Serial.print(t);
  Serial.print(" C ");
  Serial.println("Water Level: ");
  Serial.print(waterLevel);

  //Serial.println("-----------------------------------------------------------");
  //Serial.println(" ");


  if(waterLevel >= 120){
    // Water level is sufficienct, turn on the pump
    Serial.println("Enough Water --> Pump Running");
    digitalWrite(pumpRelayPin, HIGH);
  }

  else{
    // Water level is high, turn off the pump
    digitalWrite(pumpRelayPin, LOW);
    Serial.println("Not enough water --> Pump not Running, fill water ");
  }
   

  delay(1000); // Delay for a bit to avoid rapid switching
}
