#include "LCDController.h"

LCDController::LCDController(spi_inst *spiInst, uint8_t dataPin, uint8_t clockPin, uint8_t chipSelectPin,
                             uint8_t dataCommandPin, uint8_t resetPin, uint8_t backlightPin, uint8_t xOffset,
                             uint8_t yOffset)
        : spiInst(spiInst), dataPin(dataPin), clockPin(clockPin), chipSelectPin(chipSelectPin),
          dataCommandPin(dataCommandPin), resetPin(resetPin), backlightPin(backlightPin), xOffset(xOffset),
          yOffset(yOffset) {
    backlightPWMSlice = pwm_gpio_to_slice_num(backlightPin);
    backlightPWMChannel = pwm_gpio_to_channel(backlightPin);
}

void LCDController::setupPins() const {
    // SPI Config
    spi_init(spiInst, 10 * 1000 * 1000);
    gpio_set_function(clockPin, GPIO_FUNC_SPI);
    gpio_set_function(dataPin, GPIO_FUNC_SPI);

    // GPIO Config
    gpio_init(resetPin);
    gpio_init(dataCommandPin);
    gpio_init(chipSelectPin);

    gpio_set_dir(resetPin, GPIO_OUT);
    gpio_set_dir(dataCommandPin, GPIO_OUT);
    gpio_set_dir(chipSelectPin, GPIO_OUT);

    gpio_put(resetPin, true);
    gpio_put(chipSelectPin, true);
    gpio_put(dataCommandPin, false);


    // PWM Config
    gpio_set_function(backlightPin, GPIO_FUNC_PWM);
    pwm_set_wrap(backlightPWMSlice, 100);
    pwm_set_chan_level(backlightPWMSlice, backlightPWMChannel, brightness);
    pwm_set_clkdiv(backlightPWMSlice, 50);
    pwm_set_enabled(backlightPWMSlice, true);
}

void LCDController::applyDefaultConfig() const {
    beginTransmission();

    enableCommand();
    write(0x11);//Sleep exit 
    sleep_ms(120);
    write(0x21);

    write(0xB1);
    enableData();
    write(0x05);
    write(0x3A);
    write(0x3A);

    enableCommand();
    write(0xB2);
    enableData();
    write(0x05);
    write(0x3A);
    write(0x3A);

    enableCommand();
    write(0xB3);
    enableData();
    write(0x05);
    write(0x3A);
    write(0x3A);
    write(0x05);
    write(0x3A);
    write(0x3A);

    enableCommand();
    write(0xB4);
    enableData();
    write(0x03);

    enableCommand();
    write(0xC0);
    enableData();
    write(0x62);
    write(0x02);
    write(0x04);

    enableCommand();
    write(0xC1);
    enableData();
    write(0xC0);

    enableCommand();
    write(0xC2);
    enableData();
    write(0x0D);
    write(0x00);

    enableCommand();
    write(0xC3);
    enableData();
    write(0x8D);
    write(0x6A);

    enableCommand();
    write(0xC4);
    enableData();
    write(0x8D);
    write(0xEE);

    enableCommand();
    write(0xC5);  /*VCOM*/
    enableData();
    write(0x0E);

    enableCommand();
    write(0xE0);
    enableData();
    write(0x10);
    write(0x0E);
    write(0x02);
    write(0x03);
    write(0x0E);
    write(0x07);
    write(0x02);
    write(0x07);
    write(0x0A);
    write(0x12);
    write(0x27);
    write(0x37);
    write(0x00);
    write(0x0D);
    write(0x0E);
    write(0x10);

    enableCommand();
    write(0xE1);
    enableData();
    write(0x10);
    write(0x0E);
    write(0x03);
    write(0x03);
    write(0x0F);
    write(0x06);
    write(0x02);
    write(0x08);
    write(0x0A);
    write(0x13);
    write(0x26);
    write(0x36);
    write(0x00);
    write(0x0D);
    write(0x0E);
    write(0x10);

    enableCommand();
    write(0x3A);
    enableData();
    write(0x05);

    enableCommand();
    write(0x36);
    enableData();
    write(0xA8);

    enableCommand();
    write(0x29);
}

inline void LCDController::beginTransmission() const {
    gpio_put(chipSelectPin, false);
}

inline void LCDController::enableCommand() const {
    gpio_put(dataCommandPin, false);
}

inline void LCDController::enableData() const {
    gpio_put(dataCommandPin, true);
}

inline void LCDController::endTransmission() const {
    gpio_put(chipSelectPin, true);
}

inline void LCDController::write(uint8_t byte) const {
    write(&byte, 1);
}

inline void LCDController::write(uint8_t *bytes, uint32_t len) const {
    spi_write_blocking(spiInst, bytes, len);
}

void LCDController::reset() const {
    gpio_put(resetPin, false);
    sleep_ms(50);
    gpio_put(resetPin, true);
    sleep_ms(200);
}

void LCDController::setBrightness(uint8_t value) {
    if (value > 100) value = 100;
    brightness = value;
    pwm_set_chan_level(backlightPWMSlice, backlightPWMChannel, brightness);
}

void LCDController::setWindows(uint8_t xStart, uint8_t yStart, uint8_t xEnd, uint8_t yEnd) const {
    uint16_t x0 = xStart + xOffset;
    uint16_t y0 = yStart + yOffset;
    uint16_t x1 = xEnd - 1 + xOffset;
    uint16_t y1 = yEnd - 1 + yOffset;

    beginTransmission();
    enableCommand();
    write(0x2a);
    enableData();
    write(x0 >> 8);
    write(x0);
    write(x1 >> 8);
    write(x1);

    enableCommand();
    write(0x2b);
    enableData();
    write(y0 >> 8);
    write(y0);
    write(y1 >> 8);
    write(y1);

    enableCommand();
    write(0x2C);
    endTransmission();
}

void LCDController::sendCommand(uint8_t command) const {
    beginTransmission();
    enableCommand();
    write(command);
    endTransmission();
}

void LCDController::sendDataByte(uint8_t data) const {
    beginTransmission();
    enableData();
    write(data);
    endTransmission();
}

void LCDController::sendDataWord(uint16_t data) const {
    beginTransmission();
    enableData();
    write(data >> 8);
    write(data);
    endTransmission();
}

void LCDController::sendDataBytes(uint8_t *data, uint32_t len) const {
    beginTransmission();
    enableData();

    write(data, len);

    endTransmission();
}

void LCDController::sendDataRepeated(uint16_t data, uint32_t len, uint32_t blockSize) const {
    beginTransmission();
    enableData();

    uint8_t dataArray[blockSize * 2];
    for (int i = 0; i < blockSize; ++i) {
        dataArray[2 * i] = data >> 8;
        dataArray[2 * i + 1] = data;
    }

    uint32_t numBlocks = len / blockSize;
    for (uint32_t i = 0; i < numBlocks; ++i) {
        write(dataArray, blockSize * 2);
    }
    write(dataArray, (len % blockSize) * 2);

    endTransmission();
}
