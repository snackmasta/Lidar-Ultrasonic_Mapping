#include <NewPing.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define TRIGGER_PIN  12  // Pin trigger untuk sensor ultrasonik
#define ECHO_PIN     11  // Pin echo untuk sensor ultrasonik
#define MAX_DISTANCE 2000 // Jarak maksimum untuk pengukuran (dalam cm)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Inisialisasi sensor ultrasonik
SoftwareSerial mySerial(2, 3); // Pin RX dan TX untuk DFPlayer Mini

void setup() {
  Serial.begin(115200); // Memulai komunikasi serial
  mySerial.begin(9600); // Memulai komunikasi serial untuk DFPlayer Mini
  mp3_set_serial(mySerial); // Mengatur komunikasi serial untuk DFPlayer Mini
  delay(5); // Memberikan waktu untuk inisialisasi
  mp3_set_volume(15); // Mengatur volume ke 15
}

void loop() {
  delay(50); // Jeda 50ms antara ping
  unsigned int distance = sonar.ping_cm(); // Mengukur jarak dalam cm
  Serial.print("Ping: ");
  Serial.print(distance); // Menampilkan jarak pada serial monitor
  Serial.println(" cm");

  // Logika untuk memainkan file audio berdasarkan jarak
  if (distance > 0 && distance <= 500) {
    mp3_play(1); // Memainkan file 0001.mp3
  } else if (distance > 500 && distance <= 1000) {
    mp3_play(2); // Memainkan file 0002.mp3
  } else if (distance > 1000 && distance <= 1500) {
    mp3_play(3); // Memainkan file 0003.mp3
  } else if (distance > 1500 && distance <= 2000) {
    mp3_play(4); // Memainkan file 0004.mp3
  }

  delay(5000); // Jeda untuk memberikan waktu file audio bermain sebelum melakukan ping lagi
}
