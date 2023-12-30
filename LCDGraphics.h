#ifndef LCDLIBRARY_LCDGRAPHICS_H
#define LCDLIBRARY_LCDGRAPHICS_H

#include "LCDController.h"
#include "utils.h"

enum Rotation {
    Degree_0 = 0,
    Degree_90 = 90,
    Degree_180 = 180,
    Degree_270 = 270
};

class LCDGraphics : public LCDController {
private:
    uint8_t width;
    uint8_t height;
    bool mirrorX = false;
    bool mirrorY = false;
    Rotation rotation = Rotation::Degree_0;

protected:
    void applyTransformation(uint8_t &x, uint8_t &y) const;

    void reverseTransformation(uint8_t &x, uint8_t &y) const;

    void setRectWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) const;

public:
    LCDGraphics(
            spi_inst *spiInst,
            uint dataPin,
            uint clockPin,
            uint chipSelectPin,
            uint dataCommandPin,
            uint resetPin,
            uint backlightPin,
            uint8_t xOffset,
            uint8_t yOffset,
            uint8_t width,
            uint8_t height
    ) : LCDController(
            spiInst,
            dataPin,
            clockPin,
            chipSelectPin,
            dataCommandPin,
            resetPin,
            backlightPin,
            xOffset,
            yOffset
    ),
        width(width),
        height(height) {}

    virtual void clear(color background) = 0;

    virtual void draw(uint8_t x, uint8_t y, bitmap image) = 0;

    virtual void drawPixel(uint8_t x, uint8_t y, color color) = 0;

    virtual void fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, color color) = 0;

    virtual void fillBorderedRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t borderWidth, color inner,
                                  color border) = 0;

    [[nodiscard]] uint8_t getWidth() const;

    [[nodiscard]] uint8_t getHeight() const;

    [[nodiscard]] bool isMirrorX() const {
        return mirrorX;
    }

    void setMirrorX(bool value) {
        mirrorX = value;
    }

    [[nodiscard]] bool isMirrorY() const {
        return mirrorY;
    }

    void setMirrorY(bool value) {
        mirrorY = value;
    }

    [[nodiscard]] Rotation getRotation() const {
        return rotation;
    }

    void setRotation(Rotation value) {
        rotation = value;
    }
};

#endif //LCDLIBRARY_LCDGRAPHICS_H
