/**
 * @file AHRS.ino
 * @author SeanKwok (shaoxiang@m5stack.com)
 * @brief M5Unit IMU Pro Mini Get roll, pitch, yaw Data Demo.
 * @version 0.1
 * @date 2023-11-02
 *
 *
 * @Hardwares:M5Unit IMU Pro Mini
 * @Platform Version: Arduino M5Stack Board Manager v2.0.7
 * @Dependent Library:
 * M5_IMU_PRO: https://github.com/m5stack/M5Unit-IMU-Pro-Mini
 * Adafruit BMP280 Library: https://github.com/adafruit/Adafruit_BMP280_Library
 * MadgwickAHRS: https://github.com/arduino-libraries/MadgwickAHRS
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5GFX: https://github.com/m5stack/M5GFX
 */

#include <M5Unified.h>
#include "M5_IMU_PRO.h"
#include "MadgwickAHRS.h"

Madgwick filter;

#define BIM270_SENSOR_ADDR 0x68
#define BMP280_SENSOR_ADDR 0x76

BMI270::BMI270 bmi270;
Adafruit_BMP280 bmp(&Wire);

void setup() {
    M5.begin();

    M5.Ex_I2C.begin();

    unsigned status = bmp.begin(BMP280_SENSOR_ADDR);
    if (!status) {
        Serial.println(
            F("Could not find a valid BMP280 sensor, check wiring or "
              "try a different address!"));
        Serial.print("SensorID was: 0x");
        Serial.println(bmp.sensorID(), 16);
        while (1) delay(10);
    }

    bmi270.init(I2C_NUM_0, BIM270_SENSOR_ADDR);
    filter.begin(20);  // 20hz
}

float roll  = 0;
float pitch = 0;
float yaw   = 0;

void loop(void) {
    if (bmi270.accelerationAvailable() && bmi270.gyroscopeAvailable()) {
        float ax, ay, az;
        float gx, gy, gz;
        bmi270.readAcceleration(ax, ay, az);
        bmi270.readGyroscope(gx, gy, gz);
        filter.updateIMU(gx, gy, gz, ax, ay, az);
        roll  = filter.getRoll();
        pitch = filter.getPitch();
        yaw   = filter.getYaw();

        Serial.printf("roll:%.2f, pitch:%.2f, yaw:%.2f\r\n", roll, pitch, yaw);
    }
}
