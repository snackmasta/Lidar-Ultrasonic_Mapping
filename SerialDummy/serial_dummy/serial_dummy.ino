#include <ESP8266WiFi.h>

int angle = 0;
int sonar = 100;
int lidar = 50; // Misalkan jarak tetap 100 untuk simulasi

void setup() {
  // Memulai komunikasi serial dengan baud rate 9600
  Serial.begin(9600);
}

void loop() {
  // Mencetak nilai sudut dan jarak ke serial monitor
  Serial.print(angle);
  Serial.print(",");
  Serial.print(sonar);
  Serial.print(",");
  Serial.print(lidar);
  Serial.println();

  // Meningkatkan nilai sudut
  angle += 10;

  // Jika nilai sudut sudah mencapai 360, reset kembali ke 0
  if (angle >= 360) {
    angle = 0;
  }

  // Menunggu 100 milidetik sebelum mencetak nilai berikutnya
  delay(500);
}
