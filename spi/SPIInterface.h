#ifndef PICOKEYBOARD_SPIINTERFACE_H
#define PICOKEYBOARD_SPIINTERFACE_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pio.h"

class SPIInterface {
private:
    uint chipSelectPin;
    uint dataCommandPin;

protected:
    virtual void waitIdle() const {}

public:
    SPIInterface(uint chipSelectPin, uint dataCommandPin) : chipSelectPin(chipSelectPin),
                                                            dataCommandPin(dataCommandPin) {}

    virtual void init() const;

    inline void beginTransmission() const {
        waitIdle();
        gpio_put(chipSelectPin, false);
    }

    inline void enableCommand() const {
        waitIdle();
        gpio_put(dataCommandPin, false);
    }

    inline void enableData() const {
        waitIdle();
        gpio_put(dataCommandPin, true);
    }

    inline void endTransmission() const {
        waitIdle();
        gpio_put(chipSelectPin, true);
    }

    inline void write(uint8_t byte) const {
        write(&byte, 1);
    }

    virtual void write(uint8_t *bytes, uint32_t len) const = 0;
};


#endif //PICOKEYBOARD_SPIINTERFACE_H
