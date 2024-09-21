/***************************************************************************
 Aries - MAX30102 oximetry / heart rate integrated sensor library
  
  Copyright (C) 2021  CDAC(T). All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/

#ifndef VEGA_MAX30102_H
#define VEGA_MAX30102_H

#include <stdint.h>

#define CIRCULAR_BUFFER_XS
#include "CircularBuffer.h"
#include "MAX30102_Registers.h"

#define DEFAULT_MODE                MAX30102_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30102_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH         MAX30102_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT     MAX30102_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT      MAX30102_LED_CURR_50MA
#define EXPECTED_PART_ID            0x15
#define RINGBUFFER_SIZE             16

#define I2C_BUS_SPEED               400000UL

typedef struct {
    uint16_t ir;
    uint16_t red;
} SensorReadout;

class VEGA_MAX30102 {
public:
    VEGA_MAX30102();
    bool begin();
    void setMode(Mode mode);
    void setLedsPulseWidth(LEDPulseWidth ledPulseWidth);
    void setSamplingRate(SamplingRate samplingRate);
    void setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
    void setHighresModeEnabled(bool enabled);
    void update();
    bool getRawValues(uint16_t *ir, uint16_t *red);
    void resetFifo();
    void startTemperatureSampling();
    bool isTemperatureReady();
    float retrieveTemperature();
    void shutdown();
    void resume();
    uint8_t getPartId();

private:
    CircularBuffer<SensorReadout, RINGBUFFER_SIZE> readoutsBuffer;

    uint8_t readRegister(uint8_t address);
    void writeRegister(uint8_t address, uint8_t data);
    void burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length);
    void readFifoData();
};

#endif
