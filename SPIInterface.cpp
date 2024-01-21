#include <stdio.h>
#include "SPIInterface.h"
#include "spi.pio.h"

void SPIInterface::init() const {
    gpio_init(dataCommandPin);
    gpio_init(chipSelectPin);

    gpio_set_dir(dataCommandPin, GPIO_OUT);
    gpio_set_dir(chipSelectPin, GPIO_OUT);

    gpio_put(chipSelectPin, true);
    gpio_put(dataCommandPin, false);
}

HardwareSPIInterface::HardwareSPIInterface(spi_inst *spiInst,
                                           uint8_t dataPin,
                                           uint8_t clockPin,
                                           uint chipSelectPin,
                                           uint dataCommandPin)
        : spiInst(spiInst),
          dataPin(dataPin),
          clockPin(clockPin),
          SPIInterface(chipSelectPin, dataCommandPin) {}

void HardwareSPIInterface::init() const {
    SPIInterface::init();

    spi_init(spiInst, 10 * 1000 * 1000);
    gpio_set_function(clockPin, GPIO_FUNC_SPI);
    gpio_set_function(dataPin, GPIO_FUNC_SPI);
}

void HardwareSPIInterface::write(uint8_t *bytes, uint32_t len) const {
    spi_write_blocking(spiInst, bytes, len);
}

PIOSPIInterface::PIOSPIInterface(PIO pio,
                                 uint8_t dataPin,
                                 uint8_t clockPin,
                                 uint chipSelectPin1,
                                 uint dataCommandPin1)
        : SPIInterface(chipSelectPin1, dataCommandPin1),
          pio(pio),
          dataPin(dataPin),
          clockPin(clockPin) {
    sm = pio_claim_unused_sm(pio, true);
}

void PIOSPIInterface::waitIdle() const {
    uint32_t sm_stall_mask = 1u << (sm + PIO_FDEBUG_TXSTALL_LSB);
    pio->fdebug = sm_stall_mask;
    while (!(pio->fdebug & sm_stall_mask));
}

void PIOSPIInterface::putData(uint8_t data) const {
    while (pio_sm_is_tx_fifo_full(pio, sm))
        ;
    *(volatile uint8_t*)&pio->txf[sm] = data;
}

void PIOSPIInterface::init() const {
    SPIInterface::init();

    uint offset = pio_add_program(pio, &pio_spi_program);

    pio_gpio_init(pio, dataPin);
    pio_gpio_init(pio, clockPin);
    pio_sm_set_consecutive_pindirs(pio, sm, dataPin, 1, true);
    pio_sm_set_consecutive_pindirs(pio, sm, clockPin, 1, true);
    pio_sm_config c = pio_spi_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, clockPin);
    sm_config_set_out_pins(&c, dataPin, 1);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    sm_config_set_clkdiv(&c, 1);
    sm_config_set_out_shift(&c, false, true, 8);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void PIOSPIInterface::write(uint8_t *bytes, uint32_t len) const {
    for (int i = 0; i < len; ++i) {
        putData(bytes[i]);
    }
}
