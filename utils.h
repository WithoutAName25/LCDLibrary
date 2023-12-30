#ifndef LCDLIBRARY_UTILS_H
#define LCDLIBRARY_UTILS_H

#include "pico/stdlib.h"

typedef uint16_t color;

struct bitmap {
    uint8_t width;
    uint8_t height;
    color data[];
};

/**
 * @brief Converts the RGB color values to a 16-bit RGB color
 *
 * This function takes three 8-bit color values (red, green, and blue)
 * and converts them into a 16-bit RGB color value. The RGB color value
 * is the combination of the red, green, and blue color components. The
 * returned 16-bit RGB color value has 5 bits for red, 6 bits for green,
 * and 5 bits for blue.
 *
 * @param r The red component value (8 bits)
 * @param g The green component value (8 bits)
 * @param b The blue component value (8 bits)
 * @return The 16-bit RGB color value
 */
color getRGBColor(uint8_t r, uint8_t g, uint8_t b);

#endif //LCDLIBRARY_UTILS_H
