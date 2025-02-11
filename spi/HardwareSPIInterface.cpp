#include "HardwareSPIInterface.h"

HardwareSPIInterface::HardwareSPIInterface(spi_inst *spiInst,
                                           const uint8_t dataPin,
                                           const uint8_t clockPin,
                                           const uint chipSelectPin,
                                           const uint dataCommandPin)
        : SPIInterface(chipSelectPin, dataCommandPin),
          spiInst(spiInst),
          dataPin(dataPin),
          clockPin(clockPin) {}

void HardwareSPIInterface::init() const {
    SPIInterface::init();

    spi_init(spiInst, 10 * 1000 * 1000);
    gpio_set_function(clockPin, GPIO_FUNC_SPI);
    gpio_set_function(dataPin, GPIO_FUNC_SPI);
}

void HardwareSPIInterface::write(uint8_t *bytes, const uint32_t len) const {
    spi_write_blocking(spiInst, bytes, len);
}