#ifndef PICOKEYBOARD_SPIINTERFACE_H
#define PICOKEYBOARD_SPIINTERFACE_H

#include "hardware/pio.h"

class SPIInterface {
    uint chipSelectPin;
    uint dataCommandPin;

protected:
    virtual void waitIdle() const {}

public:
    SPIInterface(const uint chipSelectPin, const uint dataCommandPin) : chipSelectPin(chipSelectPin),
                                                                        dataCommandPin(dataCommandPin) {}

    virtual ~SPIInterface() = default;

    virtual void init() const;

    void beginTransmission() const {
        waitIdle();
        gpio_put(chipSelectPin, false);
    }

    void enableCommand() const {
        waitIdle();
        gpio_put(dataCommandPin, false);
    }

    void enableData() const {
        waitIdle();
        gpio_put(dataCommandPin, true);
    }

    void endTransmission() const {
        waitIdle();
        gpio_put(chipSelectPin, true);
    }

    void write(uint8_t byte) const {
        write(&byte, 1);
    }

    virtual void write(uint8_t* bytes, uint32_t len) const = 0;
};


#endif //PICOKEYBOARD_SPIINTERFACE_H
