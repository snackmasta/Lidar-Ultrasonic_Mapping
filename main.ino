#include <SoftwareSerial.h>   // Header file of software serial port
#include <Servo.h>
#include <NewPing.h>

SoftwareSerial Serial1(2, 3); // Define software serial port name as Serial1 and define pin2 as RX & pin3 as TX

int dist;                     // Actual distance measurements of LiDAR
int check;                    // Save check value
int uart[9];                   // Save data measured by LiDAR
const int HEADER = 0x59;      // Frame header of data package
int it = 10; // Number of iterations for averaging distance

NewPing sonar(12, 11, 200);   // Set up NewPing with TRIGGER_PIN as 12, ECHO_PIN as 11, and MAX_DISTANCE as 200
Servo myservo;                 // Define Servo object

void setup() {
  myservo.attach(9);           // Attach servo to pin 9
  Serial.begin(9600);          // Set bit rate of serial port connecting Arduino with computer
  Serial1.begin(115200);       // Set bit rate of serial port connecting LiDAR with Arduino
  delay(3000);                 // Allow sensors to stabilize
}

void loop() {
  int i = 0;
  int t = 0;
  int totalDistance = 0;

  for (i = 0; i <= 180; i++) {
    myservo.write(i);          // Rotate servo to angle i
    delay(20);                  // Give time for the servo to move
    
    totalDistance = 0;
    for (t = 0; t < it; t++) {
      unsigned int uS = sonar.ping_cm(); // Get distance from the ultrasonic sensor
      totalDistance += uS;
      delay(10);
    }
    
    int averageDistance = totalDistance / it; // Calculate average distance

    Serial.print("Angle: ");
    Serial.print(i);
    Serial.print(", Sonar: ");
    Serial.print(averageDistance);
    Serial.print(", LiDAR ");
    Serial.print(dist);                // Output measure distance value of LiDAR
    Serial.print('\t');
    Serial.print('\n');

  }

  // Read data from LiDAR
  if (Serial1.available()) {                // Check if serial port has data input
    if (Serial1.read() == HEADER) {         // Assess data package frame header 0x59
      uart[0] = HEADER;
      if (Serial1.read() == HEADER) {       // Assess data package frame header 0x59
        uart[1] = HEADER;
        for (int l = 2; l < 9; l++) {       // Save data in array
          uart[l] = Serial1.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)) {     // Verify the received data as per protocol
          dist = uart[2] + uart[3] * 256;    // Calculate distance value
        }
      }
    }
  }

}