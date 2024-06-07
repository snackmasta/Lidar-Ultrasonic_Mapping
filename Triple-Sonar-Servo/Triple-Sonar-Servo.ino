#include <SoftwareSerial.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN_1  12  // Sonar 1
#define ECHO_PIN_1     11
#define TRIGGER_PIN_2  10  // Sonar 2
#define ECHO_PIN_2     9
#define TRIGGER_PIN_3   8  // Sonar 3
#define ECHO_PIN_3     7
#define SONAR_NUM 3
#define MAX_DISTANCE 400 

NewPing sonar[SONAR_NUM] = {   
  NewPing(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE),
  NewPing(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE)};

Servo myservo1;  // create Servo1
Servo myservo2;  // create Servo2
Servo myservo3;  // create Servo3

int pos = 0;    // variable to store the servo position
int it = 1; // Number of iterations for averaging distance
int dist;
float SensorData;

void setup() {
  // Step 1: Initialize hardware serial port (serial debug port)
  Serial.begin(9600);
  // wait for serial port to connect. Needed for native USB port only
  while (!Serial);
  myservo1.attach(4);  // attaches the servo1 on pin 4 to the Servo object
  myservo2.attach(5);  // attaches the servo2 on pin 5 to the Servo object
  myservo3.attach(6);  // attaches the servo3 on pin 6 to the Servo object
}

void loop() {
  for (pos = 90; pos >=0; pos -= 10) { // goes from 90 degrees to 0 degrees per 10 derajat
    // in steps of 1 degree
    myservo1.write(pos);  myservo2.write(pos);  myservo3.write(pos);              // tell servo to go to position in variable 'pos'
    sonar3titik();                    //panggil fungsi sonar 3 titik
    //delay(10);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 0; pos <= 90; pos += 10) { // goes from 0 degrees to 90 degrees per 10 derajat
    myservo1.write(pos); myservo2.write(pos); myservo3.write(pos);              // tell servo to go to position in variable 'pos'
    sonar3titik();                    //panggil fungsi sonar 3 titik
    //delay(10);                       // waits 15 ms for the servo to reach the position
  }
}

void sonar3titik() {
  int sonarFiltered[SONAR_NUM];
  for (uint8_t i = 0; i < SONAR_NUM; i++) 
  { SensorData = sonar[i].ping_cm();
    sonarFiltered[i] = (int)SensorData;  
  }
  
  Serial.print(pos);
  Serial.print(","); 
  Serial.print(sonarFiltered[0]);
  Serial.print(",");
  Serial.print(sonarFiltered[1]);
  Serial.print(",");
  Serial.println(sonarFiltered[2]);  
  delay(250);

}