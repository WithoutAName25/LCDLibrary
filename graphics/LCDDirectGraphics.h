#ifndef LCDLIBRARY_LCDDIRECTGRAPHICS_H
#define LCDLIBRARY_LCDDIRECTGRAPHICS_H

#include "LCDGraphics.h"

class LCDDirectGraphics final : public LCDGraphics {
public:
    LCDDirectGraphics(SPIInterface* spi,
                      const uint resetPin,
                      const uint backlightPin,
                      const uint8_t hwXOffset,
                      const uint8_t hwYOffset,
                      const uint8_t hwWidth,
                      const uint8_t hwHeight,
                      const Rotation rotation)
        : LCDGraphics(spi,
                      resetPin,
                      backlightPin,
                      hwXOffset,
                      hwYOffset,
                      hwWidth,
                      hwHeight,
                      rotation) {}

    void clear(color background) override;

    void draw(uint8_t x, uint8_t y, bitmap image) override;

    void drawPixel(uint8_t x, uint8_t y, color color) override;

    void fillRect(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY, color color) override;

    void fillBorderedRect(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY, uint8_t borderWidth,
                          color inner, color border) override;
};


#endif //LCDLIBRARY_LCDDIRECTGRAPHICS_H
