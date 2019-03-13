/*

  Sensirion SFM3000 Low Pressure Drop Digital Flow Meter Library for Arduino
  Copyright (C) 2019 by AllWize
  Copyright (C) 2019 by Xose Pérez <xose at espurna dot io>

  Basic usage example

  The SFM3000 library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The SFM3000 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the SFM3000 library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "SFM3000.h"
SFM3000 sfm3000;

#if defined(ARDUINO_ARCH_SAMD)
#define DEBUG_SERIAL SerialUSB
#else
#define DEBUG_SERIAL Serial
#endif

// ----------------------------------------------------------------------------

#include <Wire.h>
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

// ----------------------------------------------------------------------------

void setup() {
    
    // Init DEBUG_SERIAL link to computer
    DEBUG_SERIAL.begin(115200);
    while (!DEBUG_SERIAL && millis() < 5000);
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println("SFM3000 example");
    DEBUG_SERIAL.println();

    // I2C bus scan
    i2cScan();

    // Init sensor communications
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