#include "LCDGraphics.h"

LCDGraphics::LCDGraphics(SPIInterface *spi,
                         const uint resetPin,
                         const uint backlightPin,
                         const uint16_t hwXOffset,
                         const uint16_t hwYOffset,
                         const uint16_t hwWidth,
                         const uint16_t hwHeight,
                         const Rotation rotation)
        : LCDController(spi,
                        resetPin,
                        backlightPin,
                        hwXOffset,
                        hwYOffset,
                        hwWidth,
                        hwHeight,
                        rotation) {}
