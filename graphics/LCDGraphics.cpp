#include "LCDGraphics.h"

void LCDGraphics::applyTransformation(uint8_t &x, uint8_t &y) const {
    if (mirrorX) x = (rotation % 180 == 0 ? width : height) - x;
    if (mirrorY) y = (rotation % 180 == 0 ? height : width) - y;

    uint8_t xNew = x;
    uint8_t yNew = y;
    switch (rotation) {
        case Rotation::Degree_90 :
            xNew = y;
            yNew = height - x;
            break;
        case Rotation::Degree_180 :
            xNew = width - x;
            yNew = height - y;
            break;
        case Rotation::Degree_270 :
            xNew = width - y;
            yNew = x;
            break;
        default : // or case Rotation::Degree_0
            break;
    }
    x = xNew;
    y = yNew;
}

void LCDGraphics::reverseTransformation(uint8_t &x, uint8_t &y) const {
    uint8_t xNew = x;
    uint8_t yNew = y;
    switch (rotation) {
        case Rotation::Degree_90 :
            xNew = height - y;
            yNew = xNew;
            break;
        case Rotation::Degree_180 :
            xNew = width - x;
            yNew = height - y;
            break;
        case Rotation::Degree_270 :
            xNew = y;
            yNew = width - x;
            break;
        default : // or case Rotation::Degree_0
            break;
    }
    x = xNew;
    y = yNew;

    if (mirrorX) x = (rotation % 180 == 0 ? width : height) - x;
    if (mirrorY) y = (rotation % 180 == 0 ? height : width) - y;
}

void LCDGraphics::setRectWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) const {
    applyTransformation(x0, y0);
    applyTransformation(x1, y1);
    if (x0 > x1) {
        uint8_t tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1) {
        uint8_t tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    setWindows(x0, y0, x1 + 1, y1 + 1);
}

uint8_t LCDGraphics::getWidth() const {
    return rotation % 180 == 0 ? width : height;
}

uint8_t LCDGraphics::getHeight() const {
    return rotation % 180 == 0 ? height : width;
}
