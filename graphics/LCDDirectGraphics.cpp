#include "LCDDirectGraphics.h"

void LCDDirectGraphics::clear(color background) {
    setWindows(0, 0, getWidth() - 1, getHeight() - 1);
    sendDataRepeated(background, getWidth() * getHeight());
}

void LCDDirectGraphics::draw(uint8_t x, uint8_t y, bitmap image) {
    setWindows(x, y, x + image.width - 1, y + image.height - 1);

    spi->beginTransmission();
    spi->enableData();
    for (int i = 0; i < image.width * image.height; ++i) {
        color pixel = image.data[i];
        spi->write(pixel >> 8);
        spi->write(pixel);
    }
    spi->endTransmission();
}

void LCDDirectGraphics::drawPixel(uint8_t x, uint8_t y, color color) {
    setWindows(x, y, x, y);
    sendDataWord(color);
}

void LCDDirectGraphics::fillRect(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY, color color) {
    setWindows(firstX, firstY, lastX, lastY);

    sendDataRepeated(color, (lastX + 1 - firstX) * (lastY + 1 - firstY));
}

void
LCDDirectGraphics::fillBorderedRect(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY,
                                    uint8_t borderWidth, color inner, color border) {
    setWindows(firstX, firstY, lastX, lastY);

    uint16_t width = lastX + 1 - firstX;
    uint16_t height = lastY + 1 - firstY;

    sendDataRepeated(border, width * borderWidth + borderWidth);
    for (int i = 0; i < height - 2 * borderWidth; ++i) {
        sendDataRepeated(inner, width - 2 * borderWidth);
        sendDataRepeated(border, 2 * borderWidth);
    }
    sendDataRepeated(border, width * borderWidth - borderWidth);
}
