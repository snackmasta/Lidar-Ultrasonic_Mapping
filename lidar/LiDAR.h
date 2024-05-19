#ifndef LIDAR_H
#define LIDAR_H

#include <SoftwareSerial.h>

class LiDAR {
public:
    LiDAR(int rxPin, int txPin);
    void begin(long baudRate);
    int readDistance();

private:
    SoftwareSerial lidarSerial;
    int uart[9];
    const int HEADER = 0x59;
};

#endif
