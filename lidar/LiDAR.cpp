#include "LiDAR.h"

LiDAR::LiDAR(int rxPin, int txPin) : lidarSerial(rxPin, txPin) {}

void LiDAR::begin(long baudRate) {
    lidarSerial.begin(baudRate);
}

int LiDAR::readDistance() {
    int dist = -1; // Initialize dist with -1 to indicate an invalid reading
    int check;
    if (lidarSerial.available()) {
        if (lidarSerial.read() == HEADER) {
            uart[0] = HEADER;
            if (lidarSerial.read() == HEADER) {
                uart[1] = HEADER;
                for (int i = 2; i < 9; i++) {
                    uart[i] = lidarSerial.read();
                }
                check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
                if (uart[8] == (check & 0xff)) { // Verify the received data as per protocol
                    dist = uart[2] + uart[3] * 256; // Calculate distance value
                    // If the calculated distance is zero, it means an invalid reading
                    if (dist == 0) {
                        dist = -1;
                    }
                }
            }
        }
    }
    return dist;
}
  