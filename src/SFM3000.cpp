/*

  Sensirion SFM3000 Low Pressure Drop Digital Flow Meter Library for Arduino
  Copyright (C) 2019 by AllWize
  Copyright (C) 2019 by Xose Pérez <xose at espurna dot io>

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
#include <Wire.h>

SFM3000::SFM3000(uint8_t address) {
    _address = address;
}

SFM3000::SFM3000() {
    SFM3000(SFM3000_DEFAULT_ADDRESS);
}

void SFM3000::begin() {
    Wire.begin(_address);
}

uint32_t SFM3000::getSerial() {
    
    uint16_t tmp;
    uint32_t serial;

    // Send read serial command
    if (!_write(SFM3000_READ_SERIAL_HIGH)) return 0;
    if (!_read(&tmp)) return 0;
    serial = ((uint32_t) tmp << 16);

    if (!_write(SFM3000_READ_SERIAL_LOW)) return 0;
    if (!_read(&tmp)) return 0;
    serial |= tmp;

    return serial;

}

float SFM3000::getMeasurement() {
    
    if (SFM3000_READ_FLOW != _current_command) {
        if (!_write(SFM3000_READ_FLOW)) return 0xFFFF;
    }
    
    uint16_t raw;
    if (!_readWithDelay(&raw)) return 0xFFFF;
    
    float flow = ((float) raw - SFM3000_OFFSET) / SFM3000_SCALE;
    if ((flow < SFM3000_MIN_FLOW) || (SFM3000_MAX_FLOW < flow)) return 0xFFFF;
    
    return flow;

}

void SFM3000::reset() {
	_write(SFM3000_SOFT_RESET);
    delay(100);
}

// ----------------------------------------------------------------------------

bool SFM3000::_check(uint8_t * data, uint8_t num, uint8_t checksum) {
    uint8_t crc = 0;
    for (uint8_t i=0; i<num; i++) {
        crc ^= data[i];
        for (uint8_t j=8; j>0; --j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ SFM3000_POLYNOMIAL;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return (crc == checksum);
}

bool SFM3000::_write(uint16_t command) {
    Wire.beginTransmission(_address);
    Wire.write(command >> 8);
    Wire.write(command & 0xFF);
    bool result = (Wire.endTransmission() == 0);
    if (result) {
        _current_command = command;
    }
    return result;
}

bool SFM3000::_read(uint16_t * value) {
    
    uint8_t data[2];
    uint8_t checksum;
    Wire.requestFrom(_address, (uint8_t) 3);
    data[0] = Wire.read();
    data[1] = Wire.read();
    checksum = Wire.read();

    bool result = _check(data, 2, checksum);
    if (result) {
        *value = (data[0] << 8) + data[1];
    }

    return result;

}

bool SFM3000::_readWithDelay(uint16_t * value, uint8_t retries, uint32_t delay) {
    bool result = false;
    while (retries--) {
        result = _read(value);
        if (result) break;
        delayMicroseconds(delay);
    }
    return result;
}
