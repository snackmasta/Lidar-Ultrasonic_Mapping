#include "Wire.h"
#include "I2C.h"

#define MPU9250_IMU_ADDRESS 0x68
#define MPU9250_MAG_ADDRESS 0x0C

#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2G  0x00
#define ACC_FULL_SCALE_4G  0x08
#define ACC_FULL_SCALE_8G  0x10
#define ACC_FULL_SCALE_16G 0x18

#define TEMPERATURE_OFFSET 21 // As defined in documentation

#define INTERVAL_MS_PRINT 1000

#define G 9.80665

struct gyroscope_raw {
  int16_t x, y, z;
} gyroscope;

struct accelerometer_raw {
  int16_t x, y, z;
} accelerometer;

struct magnetometer_raw {
  int16_t x, y, z;

  struct {
    int8_t x, y, z;
  } adjustment;
} magnetometer;

struct temperature_raw {
  int16_t value;
} temperature;

struct {
  struct {
    float x, y, z;
  } accelerometer, gyroscope, magnetometer;

  float temperature;
} normalized;

unsigned long lastPrintMillis = 0;

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  I2CwriteByte(MPU9250_IMU_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS); // Configure gyroscope range
  I2CwriteByte(MPU9250_IMU_ADDRESS, 28, ACC_FULL_SCALE_2G);        // Configure accelerometer range

  I2CwriteByte(MPU9250_IMU_ADDRESS, 55, 0x02); // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin for raw data

  setMagnetometerAdjustmentValues();

  //Start magnetometer
  I2CwriteByte(MPU9250_MAG_ADDRESS, 0x0A, 0x12); // Request continuous magnetometer measurements in 16 bits (mode 1)
}

void loop()
{
  unsigned long currentMillis = millis();

  if (isImuReady()) {
    readRawImu();

    normalize(gyroscope);
    normalize(accelerometer);
    normalize(temperature);
  }

  if (isMagnetometerReady()) {
    readRawMagnetometer();

    normalize(magnetometer);
  }
