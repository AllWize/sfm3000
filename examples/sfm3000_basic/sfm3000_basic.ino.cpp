# 1 "/tmp/tmpL4m5yv"
#include <Arduino.h>
# 1 "/home/xose/workspace/allwize/pilot-efergy/sfm3000/examples/sfm3000_basic/sfm3000_basic.ino"
# 24 "/home/xose/workspace/allwize/pilot-efergy/sfm3000/examples/sfm3000_basic/sfm3000_basic.ino"
#include "SFM3000.h"
SFM3000 sfm3000;

#if defined(ARDUINO_ARCH_SAMD)
#define DEBUG_SERIAL SerialUSB
#else
#define DEBUG_SERIAL Serial
#endif



#include <Wire.h>
void i2cScan();
void setup();
void loop();
#line 36 "/home/xose/workspace/allwize/pilot-efergy/sfm3000/examples/sfm3000_basic/sfm3000_basic.ino"
void i2cScan() {
    Wire.begin();
    unsigned char nDevices = 0;
    for (unsigned char address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        unsigned char error = Wire.endTransmission();
        if (error == 0) {
            DEBUG_SERIAL.print("[I2C] Device found at address 0x");
            DEBUG_SERIAL.println(address, HEX);
            nDevices++;
        }
    }
    if (nDevices == 0) DEBUG_SERIAL.println("[I2C] No devices found\n");

}



void setup() {


    DEBUG_SERIAL.begin(115200);
    while (!DEBUG_SERIAL && millis() < 5000);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println("SFM3000 example");
    DEBUG_SERIAL.println();


    i2cScan();


    sfm3000.begin();
    DEBUG_SERIAL.print("SFM3000 DEBUG_SERIAL number: 0x");
    DEBUG_SERIAL.println(sfm3000.getSerial(), HEX);
    DEBUG_SERIAL.println();

}

void loop() {

    delay(1000);
    DEBUG_SERIAL.print("SFM3000 reading: ");
    DEBUG_SERIAL.println(sfm3000.getMeasurement());

}