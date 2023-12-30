#ifndef LCDLIBRARY_LCDDIRECTGRAPHICS_H
#define LCDLIBRARY_LCDDIRECTGRAPHICS_H

#include "LCDGraphics.h"

class LCDDirectGraphics : public LCDGraphics {
public:
    LCDDirectGraphics(spi_inst *spiInst, uint dataPin, uint clockPin, uint chipSelectPin, uint dataCommandPin,
                      uint resetPin, uint backlightPin, uint8_t xOffset, uint8_t yOffset, uint8_t width,
                      uint8_t height)
            : LCDGraphics(spiInst, dataPin, clockPin, chipSelectPin, dataCommandPin, resetPin, backlightPin, xOffset,
                          yOffset, width, height) {}

    void clear(color background) override;

    void draw(uint8_t x, uint8_t y, bitmap image) override;

    void drawPixel(uint8_t x, uint8_t y, color color) override;

    void fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, color color) override;

    void fillBorderedRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t borderWidth, color inner,
                          color border) override;
};


#endif //LCDLIBRARY_LCDDIRECTGRAPHICS_H
