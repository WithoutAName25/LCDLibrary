#include "LCDController.h"

LCDController::LCDController(SPIInterface *spi, uint8_t resetPin, uint8_t backlightPin, uint8_t xOffset,
                             uint8_t yOffset)
        : spi(spi), resetPin(resetPin), backlightPin(backlightPin), xOffset(xOffset),
          yOffset(yOffset) {
    backlightPWMSlice = pwm_gpio_to_slice_num(backlightPin);
    backlightPWMChannel = pwm_gpio_to_channel(backlightPin);
}

void LCDController::setupPins() const {
    // Reset Config
    gpio_init(resetPin);
    gpio_set_dir(resetPin, GPIO_OUT);
    gpio_put(resetPin, true);

    // Backlight Config
    gpio_set_function(backlightPin, GPIO_FUNC_PWM);
    pwm_set_wrap(backlightPWMSlice, 100);
    pwm_set_chan_level(backlightPWMSlice, backlightPWMChannel, brightness);
    pwm_set_clkdiv(backlightPWMSlice, 50);
    pwm_set_enabled(backlightPWMSlice, true);
}

void LCDController::applyDefaultConfig() const {
    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x01); // Software reset
    spi->endTransmission();

    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x11); // Exit sleep mode
    spi->endTransmission();

    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x3A); // Set color mode
    spi->enableData();
    spi->write(0x05); // 16 bit / pixel
    spi->endTransmission();

    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x21); // Inversion on, then 10 ms delay (supposedly a hack?)
    spi->endTransmission();

    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x13); // Normal display on, then 10 ms delay
    spi->endTransmission();

    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x29); // Main screen turn on, then wait 500 ms
    spi->endTransmission();
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

    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x2a);
    spi->enableData();
    spi->write(x0 >> 8);
    spi->write(x0);
    spi->write(x1 >> 8);
    spi->write(x1);

    spi->enableCommand();
    spi->write(0x2b);
    spi->enableData();
    spi->write(y0 >> 8);
    spi->write(y0);
    spi->write(y1 >> 8);
    spi->write(y1);

    spi->enableCommand();
    spi->write(0x2C);
    spi->endTransmission();
}

void LCDController::sendCommand(uint8_t command) const {
    spi->beginTransmission();
    spi->enableCommand();
    spi->write(command);
    spi->endTransmission();
}

void LCDController::sendDataByte(uint8_t data) const {
    spi->beginTransmission();
    spi->enableData();
    spi->write(data);
    spi->endTransmission();
}

void LCDController::sendDataWord(uint16_t data) const {
    spi->beginTransmission();
    spi->enableData();
    spi->write(data >> 8);
    spi->write(data);
    spi->endTransmission();
}

void LCDController::sendDataBytes(uint8_t *data, uint32_t len) const {
    spi->beginTransmission();
    spi->enableData();

    spi->write(data, len);

    spi->endTransmission();
}

void LCDController::sendDataRepeated(uint16_t data, uint32_t len, uint32_t blockSize) const {
    spi->beginTransmission();
    spi->enableData();

    uint8_t dataArray[blockSize * 2];
    for (int i = 0; i < blockSize; ++i) {
        dataArray[2 * i] = data >> 8;
        dataArray[2 * i + 1] = data;
    }

    uint32_t numBlocks = len / blockSize;
    for (uint32_t i = 0; i < numBlocks; ++i) {
        spi->write(dataArray, blockSize * 2);
    }
    spi->write(dataArray, (len % blockSize) * 2);

    spi->endTransmission();
}
