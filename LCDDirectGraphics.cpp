#include "LCDDirectGraphics.h"

void LCDDirectGraphics::clear(color background) {
    setWindows(0, 0, getWidth(), getHeight());
    sendDataRepeated(background, getWidth() * getHeight());
}

void LCDDirectGraphics::draw(uint8_t x, uint8_t y, bitmap image) {
    uint8_t x0 = x;
    uint8_t y0 = y;
    uint8_t x1 = x + image.width - 1;
    uint8_t y1 = y + image.height - 1;
    setRectWindow(x0, y0, x1, y1);

    applyTransformation(x0, y0);
    applyTransformation(x1, y1);

    uint8_t minX = x0 < x1 ? x0 : x1;
    uint8_t minY = y0 < y1 ? y0 : y1;

    beginTransmission();
    enableData();
    for (int dY = 0; dY < getRotation() % 180 == 0 ? image.height : image.width; ++dY) {
        for (int dX = 0; dX < getRotation() % 180 == 0 ? image.width : image.height; ++dX) {
            uint8_t pixelX = minX + dX;
            uint8_t pixelY = minY + dY;
            reverseTransformation(pixelX, pixelY);
            color pixel = image.data[pixelY * image.width + pixelX];
            write(pixel >> 8);
            write(pixel);
        }
    }
    endTransmission();
}

void LCDDirectGraphics::drawPixel(uint8_t x, uint8_t y, color color) {
    applyTransformation(x, y);
    setWindows(x, y, x + 1, y + 1);
    sendDataWord(color);
}

void LCDDirectGraphics::fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, color color) {
    setRectWindow(x0, y0, x1, y1);

    sendDataRepeated(color, (x1 + 1 - x0) * (y1 + 1 - y0));
}

void
LCDDirectGraphics::fillBorderedRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t borderWidth, color inner,
                                    color border) {
    setRectWindow(x0, y0, x1, y1);

    uint8_t width = getRotation() % 180 == 0 ? (x1 + 1 - x0) : (y1 + 1 - y0);
    uint8_t height = getRotation() % 180 == 0 ? (y1 + 1 - y0) : (x1 + 1 - x0);

    sendDataRepeated(border, width * borderWidth + borderWidth);
    for (int i = 0; i < height - 2 * borderWidth; ++i) {
        sendDataRepeated(inner, width - 2 * borderWidth);
        sendDataRepeated(border, 2 * borderWidth);
    }
    sendDataRepeated(border, width * borderWidth - borderWidth);
}
