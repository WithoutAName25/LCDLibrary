#ifndef LCDLIBRARY_PIOSPIINTERFACE_H
#define LCDLIBRARY_PIOSPIINTERFACE_H

#include "SPIInterface.h"

class PIOSPIInterface final : public SPIInterface {
    PIO pio;
    uint sm;
    uint8_t dataPin;
    uint8_t clockPin;

    void putData(uint8_t data) const;

protected:
    void waitIdle() const override;

public:
    PIOSPIInterface(PIO pio, uint8_t dataPin, uint8_t clockPin, uint chipSelectPin, uint dataCommandPin);

    void init() const override;

    void write(uint8_t *bytes, uint32_t len) const override;
};

#endif // LCDLIBRARY_PIOSPIINTERFACE_H
