#include "LCDController.h"

LCDController::LCDController(SPIInterface *spi, uint8_t resetPin, uint8_t backlightPin, uint16_t hwXOffset,
                             uint16_t hwYOffset, uint16_t hwWidth, uint16_t hwHeight, Rotation rotation)
        : spi(spi), resetPin(resetPin), backlightPin(backlightPin), hwXOffset(hwXOffset),
          hwYOffset(hwYOffset), hwWidth(hwWidth), hwHeight(hwHeight), rotation(rotation) {
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

    updateMemoryDataAccessControl();

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

void LCDController::updateMemoryDataAccessControl() const {
    spi->beginTransmission();
    spi->enableCommand();
    spi->write(0x36); // Set Memory Data Access Control
    spi->enableData();
    uint8_t madctl = 0;
    switch (rotation) {
        case Degree_0:
            madctl = 0b00000000; // Normal mode
            break;
        case Degree_90:
            madctl = 0b10100000; // Y-Mirror, X-Y-Exchange
            break;
        case Degree_180:
            madctl = 0b11000000; // Y-Mirror, X-Mirror
            break;
        case Degree_270:
            madctl = 0b01100000; // X-Mirror, X-Y-Exchange
            break;
    }
    spi->write(madctl);
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

void LCDController::setRotation(Rotation newRotation) {
    rotation = newRotation;
    updateMemoryDataAccessControl();
}

void LCDController::setWindows(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY) const {
    uint16_t x0;
    uint16_t x1;
    uint16_t y0;
    uint16_t y1;
    switch (rotation) {
        case Degree_0:
            x0 = hwXOffset + firstX;
            x1 = hwXOffset + lastX;
            y0 = hwYOffset + firstY;
            y1 = hwYOffset + lastY;
            break;
        case Degree_90:
            x0 = 320 - hwYOffset - hwHeight + firstX;
            x1 = 320 - hwYOffset - hwHeight + lastX;
            y0 = hwXOffset + firstY;
            y1 = hwXOffset + lastY;
            break;
        case Degree_180:
            x0 = 240 - hwXOffset - hwWidth + firstX;
            x1 = 240 - hwXOffset - hwWidth + lastX;
            y0 = 320 - hwYOffset - hwHeight + firstY;
            y1 = 320 - hwYOffset - hwHeight + lastY;
            break;
        case Degree_270:
            x0 = hwYOffset + firstX;
            x1 = hwYOffset + lastX;
            y0 = 240 - hwXOffset - hwWidth + firstY;
            y1 = 240 - hwXOffset - hwWidth + lastY;
            break;
    }

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

uint16_t LCDController::getWidth() const {
    return rotation % 180 == 0 ? hwWidth : hwHeight;
}

uint16_t LCDController::getHeight() const {
    return rotation % 180 == 0 ? hwHeight : hwWidth;
}
