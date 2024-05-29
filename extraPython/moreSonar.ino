#include <SoftwareSerial.h>
#include "TFMini.h"
#include <NewPing.h>
#include <GY26Compass.h>

#define TRIGGER_PIN1  12  // First sonar sensor trigger pin
#define ECHO_PIN1     11  // First sonar sensor echo pin
#define TRIGGER_PIN2  10  // Second sonar sensor trigger pin
#define ECHO_PIN2     9   // Second sonar sensor echo pin
#define TRIGGER_PIN3  8   // Third sonar sensor trigger pin
#define ECHO_PIN3     7   // Third sonar sensor echo pin
#define MAX_DISTANCE 200 
#define BUTTON_PIN   6    // Define the pin for the pushbutton

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); 
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); 
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE); 
GY26_I2C_Compass compass(0x70);
TFMini tfmini;
SoftwareSerial SerialTFMini(4, 5); // The only value that matters here is the first one, 2, Rx

int it = 10; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;
int dist1, dist2, dist3;
int logNumber = 1; // Initialize logNumber to 1

//... (existing code)

void loop() {
  //... (existing code)

  // Measure distances from all sonar sensors
  unsigned int uS1 = sonar1.ping_cm();
  unsigned int uS2 = sonar2.ping_cm();
  unsigned int uS3 = sonar3.ping_cm();

  // Calculate average distances
  int totalDistance = uS1 + uS2 + uS3;
  int averageDistance = totalDistance / 3;

  // Store distances in variables
  dist1 = uS1;
  dist2 = uS2;
  dist3 = uS3;

  //... (existing code)
}