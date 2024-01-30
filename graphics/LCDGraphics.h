#ifndef LCDLIBRARY_LCDGRAPHICS_H
#define LCDLIBRARY_LCDGRAPHICS_H

#include "../LCDController.h"
#include "../utils.h"

class LCDGraphics : public LCDController {
public:
    LCDGraphics(SPIInterface *spi,
                uint resetPin,
                uint backlightPin,
                uint16_t hwXOffset,
                uint16_t hwYOffset,
                uint16_t hwWidth,
                uint16_t hwHeight,
                Rotation rotation);

    virtual void clear(color background) = 0;

    virtual void draw(uint8_t x, uint8_t y, bitmap image) = 0;

    virtual void drawPixel(uint8_t x, uint8_t y, color color) = 0;

    virtual void fillRect(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY, color color) = 0;

    virtual void fillBorderedRect(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY,  uint8_t borderWidth, color inner,
                                  color border) = 0;
};

#endif //LCDLIBRARY_LCDGRAPHICS_H
