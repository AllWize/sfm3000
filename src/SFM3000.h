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

#ifndef SFM3000_h
#define SFM3000_h

#include "Arduino.h"

enum SFM3000_REGISTERS {
  SFM3000_READ_FLOW = 0x1000,
  SFM3000_SOFT_RESET = 0x2000,
  SFM3000_READ_SERIAL_HIGH = 0x31AE,
  SFM3000_READ_SERIAL_LOW = 0x31AF
};

#define SFM3000_DEFAULT_ADDRESS   0x40
#define SFM3000_POLYNOMIAL        0x0131
#define SFM3000_OFFSET            32000.0
#define SFM3000_SCALE             140.0
#define SFM3000_MIN_FLOW          -200
#define SFM3000_MAX_FLOW          200

class SFM3000 {

    public:

        SFM3000();
        SFM3000(uint8_t address);
        void begin();
        uint32_t getSerial();
        float getMeasurement();
        void reset();

    protected:

        bool _check(uint8_t * data, uint8_t num, uint8_t checksum);
        bool _write(uint16_t command);
        bool _read(uint16_t * value);
        bool _readWithDelay(uint16_t * value, uint8_t retries = 20, uint32_t delay = 10000);

        uint8_t _address = SFM3000_DEFAULT_ADDRESS;
        uint16_t _current_command = 0;

};

#endif
