#ifndef LCDLIBRARY_HARDWARESPIINTERFACE_H
#define LCDLIBRARY_HARDWARESPIINTERFACE_H


#include "SPIInterface.h"

class HardwareSPIInterface : public SPIInterface {
private:
    spi_inst *spiInst;
    uint8_t dataPin;
    uint8_t clockPin;

public:
    HardwareSPIInterface(spi_inst *spiInst, uint8_t dataPin, uint8_t clockPin, uint chipSelectPin, uint dataCommandPin);

    void init() const override;

    void write(uint8_t *bytes, uint32_t len) const override;
};


#endif //LCDLIBRARY_HARDWARESPIINTERFACE_H
