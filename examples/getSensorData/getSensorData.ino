/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file getSensorData.ino
 * @brief M5Unit IMU Pro Mini Get Sensor Data Demo.
 * @version 0.2
 * @date 2024-07-02
 *
 * @Hardwares:M5Unit IMU Pro Mini
 * @Platform Version: Arduino M5Stack Board Manager v2.0.7
 * @Dependent Library:
 * M5_IMU_PRO: https://github.com/m5stack/M5Unit-IMU-Pro-Mini
 * Adafruit BMP280 Library: https://github.com/adafruit/Adafruit_BMP280_Library
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5GFX: https://github.com/m5stack/M5GFX
 *
 */

#include <M5Unified.h>
#include "M5_IMU_PRO.h"

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
}

void loop(void) {
    // put your main code here, to run repeatedly:
    float x, y, z;

    if (bmi270.accelerationAvailable()) {
        bmi270.readAcceleration(x, y, z);

        Serial.print("accel: \t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    if (bmi270.gyroscopeAvailable()) {
        bmi270.readGyroscope(x, y, z);

        Serial.print("gyro: \t");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.print(z);
        Serial.println();
    }

    if (bmi270.magneticFieldAvailable()) {
        int16_t mx, my, mz = 0;
        bmi270.readMagneticField(mx, my, mz);

        Serial.print("mag: \t");
        Serial.print(mx);
        Serial.print('\t');
        Serial.print(my);
        Serial.print('\t');
        Serial.print(mz);
        Serial.println();
    }

    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();
    delay(500);
}
