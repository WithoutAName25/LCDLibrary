#include "LCDDirectGraphics.h"

void LCDDirectGraphics::clear(const color background) {
    setWindows(0, 0, getWidth() - 1, getHeight() - 1);
    sendDataRepeated(background, getWidth() * getHeight());
}

void LCDDirectGraphics::draw(const uint8_t x, const uint8_t y, const bitmap image) {
    setWindows(x, y, x + image.width - 1, y + image.height - 1);

    spi->beginTransmission();
    spi->enableData();
    for (int i = 0; i < image.width * image.height; ++i) {
        const color pixel = image.data[i];
        spi->write(pixel >> 8);
        spi->write(pixel);
    }
    spi->endTransmission();
}

void LCDDirectGraphics::drawPixel(const uint8_t x, const uint8_t y, const color color) {
    setWindows(x, y, x, y);
    sendDataWord(color);
}

void LCDDirectGraphics::fillRect(const uint16_t firstX, const uint16_t firstY, const uint16_t lastX, const uint16_t lastY, const color color) {
    setWindows(firstX, firstY, lastX, lastY);

    sendDataRepeated(color, (lastX + 1 - firstX) * (lastY + 1 - firstY));
}

void
LCDDirectGraphics::fillBorderedRect(const uint16_t firstX, const uint16_t firstY, const uint16_t lastX, const uint16_t lastY,
                                    const uint8_t borderWidth, const color inner, const color border) {
    setWindows(firstX, firstY, lastX, lastY);

    const uint16_t width = lastX + 1 - firstX;
    const uint16_t height = lastY + 1 - firstY;

    sendDataRepeated(border, width * borderWidth + borderWidth);
    for (int i = 0; i < height - 2 * borderWidth; ++i) {
        sendDataRepeated(inner, width - 2 * borderWidth);
        sendDataRepeated(border, 2 * borderWidth);
    }
    sendDataRepeated(border, width * borderWidth - borderWidth);
}
