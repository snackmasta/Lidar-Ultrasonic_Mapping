#include <Servo.h>
#include <NewPing.h>
#include <GY26Compass.h>

#define TRIGGER_PIN  12 
#define ECHO_PIN     11  
#define MAX_DISTANCE 200 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo myservo;  
GY26_I2C_Compass compass(0x70);

int it = 10; // Number of iterations for averaging distance
float localDeclinationAngle = 0.0; 
float compassAngle;

void setup() {
  myservo.attach(9); 
  Serial.begin(9600);
  delay(3000); // Allow sensors to stabilize
  Wire.begin();       // Start I2C connectivity
  compass.setDeclinationAngle(localDeclinationAngle);
}

void loop() {
  int i = 0;
  int t = 0;
  int totalDistance = 0;

  for (i = 0; i <= 180; i++) {
    myservo.write(i);
    delay(20); // Give time for the servo to move
    
    totalDistance = 0;
    for (t = 0; t < it; t++) {
      unsigned int uS = sonar.ping_cm();
      totalDistance += uS;
      delay(10);
    }
    
    int averageDistance = totalDistance / it;
    float compassAngle = compass.getCompassAngle();

    // Convert compassAngle to an integer by truncating
    int compassAngleInt = (int)compassAngle;

    Serial.print(compassAngleInt);
    Serial.print(",");
    Serial.println(averageDistance);

  }

  // Add a delay before the next sweep
  delay(500);
}
