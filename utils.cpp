#include "utils.h"

color getRGBColor(const uint8_t r, const uint8_t g, const uint8_t b) {
    return (r & 0xF8) << 8 | (g & 0xFC) << 3 | (b >> 3) & 0x1F;
}
