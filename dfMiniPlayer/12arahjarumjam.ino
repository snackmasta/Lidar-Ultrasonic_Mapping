#include <Wire.h>
#include <GY26Compass.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

GY26_I2C_Compass compass(0x70);
float localDeclinationAngle = 0.0; 
float compassAngle;

SoftwareSerial mySerial(2, 3); //RX 3, TX 2

// Function to convert angle to compass direction
String getCompassDirection(float angle) {
    if ((angle >= 0 && angle < 15) || (angle >= 345 && angle <= 360)) return "Jam 12";
    else if (angle >= 15 && angle < 45) return "Jam 1";
    else if (angle >= 45 && angle < 75) return "Jam 2";
    else if (angle >= 75 && angle < 105) return "Jam 3";
    else if (angle >= 105 && angle < 135) return "Jam 4";
    else if (angle >= 135 && angle < 165) return "Jam 5";
    else if (angle >= 165 && angle < 195) return "Jam 6";
    else if (angle >= 195 && angle < 225) return "Jam 7";
    else if (angle >= 225 && angle < 255) return "Jam 8";
    else if (angle >= 255 && angle < 285) return "Jam 9";
    else if (angle >= 285 && angle < 315) return "Jam 10";
    else if (angle >= 315 && angle < 345) return "Jam 11";
    return "Tidak Diketahui";
}

void playAudioBasedOnDirection(float angle) {
    if ((angle >= 0 && angle < 15) || (angle >= 345 && angle <= 360)) mp3_play(12);
    else if (angle >= 15 && angle < 45) mp3_play(1);
    else if (angle >= 45 && angle < 75) mp3_play(2);
    else if (angle >= 75 && angle < 105) mp3_play(3);
    else if (angle >= 105 && angle < 135) mp3_play(4);
    else if (angle >= 135 && angle < 165) mp3_play(5);
    else if (angle >= 165 && angle < 195) mp3_play(6);
    else if (angle >= 195 && angle < 225) mp3_play(7);
    else if (angle >= 225 && angle < 255) mp3_play(8);
    else if (angle >= 255 && angle < 285) mp3_play(9);
    else if (angle >= 285 && angle < 315) mp3_play(10);
    else if (angle >= 315 && angle < 345) mp3_play(11);
}

void setup() {
    Serial.begin(9600); // Start serial monitor
    mySerial.begin(9600); // Start serial communication with DFPlayer Mini
    mp3_set_serial(mySerial); // Set DFPlayer Mini serial
    delay(500); 
    mp3_set_volume(30);
    Wire.begin(); // Start I2C connectivity
    compass.setDeclinationAngle(localDeclinationAngle);
}

void loop() {
    compassAngle = compass.getCompassAngle();
    String direction = getCompassDirection(compassAngle);
    playAudioBasedOnDirection(compassAngle);
    
    Serial.println("=========================================");
    Serial.print("COMPASS ANGLE:  "); Serial.println(compassAngle);
    Serial.print("DIRECTION: "); Serial.println(direction);
    Serial.println("=========================================");
    Serial.println();
    
    delay(1000); // Delay for 1 second
}
