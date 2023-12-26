#ifndef LCDLIBRARY_LIBRARY_H
#define LCDLIBRARY_LIBRARY_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"

class LCD {
private:
    spi_inst *spiInst;
    uint dataPin;
    uint clockPin;
    uint chipSelectPin;
    uint dataCommandPin;
    uint resetPin;
    uint backlightPin;
    uint brightness = 100;
    uint backlightPWMSlice;
    uint backlightPWMChannel;

    void setupPins() const;

    void applyDefaultConfig() const;

    inline void beginTransmission() const;

    inline void enableCommand() const;

    inline void enableData() const;

    inline void endTransmission() const;

    inline void write(uint8_t byte) const;

    inline void write(uint8_t *bytes, uint32_t len) const;


public:
    LCD(spi_inst *spiInst, uint dataPin, uint clockPin, uint chipSelectPin, uint dataCommandPin, uint resetPin,
        uint backlightPin);

    void init() const {
        setupPins();
        applyDefaultConfig();
    }

    void reset() const;

    void setBrightness(uint value);

    void sendCommand(uint8_t command) const;

    void sendDataByte(uint8_t data) const;

    void sendDataBytes(uint8_t *data, uint32_t len) const;

    void sendDataRepeated(uint16_t data, uint32_t len, uint32_t blockSize = 512) const;
};

#endif //LCDLIBRARY_LIBRARY_H
