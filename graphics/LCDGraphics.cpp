#include "LCDGraphics.h"

LCDGraphics::LCDGraphics(SPIInterface *spi,
                         uint resetPin,
                         uint backlightPin,
                         uint16_t hwXOffset,
                         uint16_t hwYOffset,
                         uint16_t hwWidth,
                         uint16_t hwHeight,
                         Rotation rotation)
        : LCDController(spi,
                        resetPin,
                        backlightPin,
                        hwXOffset,
                        hwYOffset,
                        hwWidth,
                        hwHeight,
                        rotation) {}
