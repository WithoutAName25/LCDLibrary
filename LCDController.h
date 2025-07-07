#ifndef LCDLIBRARY_LCDCONTROLLER_H
#define LCDLIBRARY_LCDCONTROLLER_H

#include "spi/SPIInterface.h"

enum Rotation { Degree_0 = 0, Degree_90 = 90, Degree_180 = 180, Degree_270 = 270 };

/**
 * \class LCDController
 *
 * \brief Controls an LCD display using SPI communication.
 *
 * This class provides methods to initialize the LCD display, send commands and data to the display, and control the
 * backlight brightness.
 *
 * It does not provide Methods for painting.
 */
class LCDController {
    uint8_t resetPin;
    uint8_t backlightPin;
    uint8_t brightness = 100;
    uint8_t backlightPWMSlice;
    uint8_t backlightPWMChannel;

    void setupPins() const;

    void applyDefaultConfig() const;

    void updateMemoryDataAccessControl() const;

protected:
    SPIInterface *spi;
    uint16_t hwXOffset;
    uint16_t hwYOffset;
    uint16_t hwWidth;
    uint16_t hwHeight;
    Rotation rotation;

public:
    LCDController(SPIInterface *spi, uint8_t resetPin, uint8_t backlightPin, uint16_t hwXOffset, uint16_t hwYOffset,
                  uint16_t hwWidth, uint16_t hwHeight, Rotation rotation);

    /**
     * \fn void LCDController::init() const
     *
     * \brief Initializes the LCDController.
     *
     * This function sets up the pins and applies the default configuration for the LCDController.
     */
    void init() const {
        spi->init();
        setupPins();
        applyDefaultConfig();
    }

    /**
     * \fn void LCDController::reset() const
     *
     * \brief Resets the LCD controller.
     */
    void reset() const;

    /**
     * \fn void LCDController::setBrightness(uint8_t value)
     *
     * \brief Sets the brightness of the LCD backlight.
     *
     * This function sets the brightness of the LCD backlight to the specified value. The value must be between 0 and
     * 100. If the specified value is greater than 100, it will be clamped to 100.
     *
     * \param value The brightness value to set (between 0 and 100).
     */
    void setBrightness(uint8_t value);

    void setRotation(Rotation newRotation);

    void setWindows(uint16_t firstX, uint16_t firstY, uint16_t lastX, uint16_t lastY) const;

    /**
     * \fn void LCDController::sendCommand(uint8_t command) const
     *
     * \brief Sends a command to the LCD display.
     *
     * This function sends a single command byte to the LCD display. It uses the SPI communication protocol to transmit
     * the command.
     *
     * \param command The command to be sent to the LCD display.
     */
    void sendCommand(uint8_t command) const;

    /**
     * \fn void LCDController::sendDataByte(uint8_t data) const
     *
     * \brief Sends a data byte to the LCD display.
     *
     * This function sends a single data byte to the LCD display using SPI communication.
     *
     * \param data The data byte to be sent.
     */
    void sendDataByte(uint8_t data) const;

    /**
     * \fn void LCDController::sendDataWord(uint16_t data) const
     *
     * \brief Sends a 16-bit data word to the LCD display.
     *
     * This function sends a 16-bit data word to the LCD display using SPI communication. The data is split into two
     * 8-bit bytes and sent sequentially.
     *
     * \param data The 16-bit data word to be sent.
     */
    void sendDataWord(uint16_t data) const;

    /**
     * \fn void LCDController::sendDataBytes(uint8_t *data, uint32_t len) const
     *
     * \brief Sends an array of data bytes to the LCD display.
     *
     * \param data The array of data bytes.
     * \param len The length of the data array.
     */
    void sendDataBytes(uint8_t *data, uint32_t len) const;

    /**
     * \fn void LCDController::sendDataRepeated(uint16_t data, uint32_t len, uint32_t blockSize) const
     *
     * \brief Sends repeated data to the LCD controller.
     *
     * This function sends the specified data to the LCD controller, repeating it multiple times depending on the length
     * and block size provided.
     *
     * \param data The data to be sent to the LCD controller.
     * \param len The total length of data to be sent.
     * \param blockSize The block size for repeating the data.
     */
    void sendDataRepeated(uint16_t data, uint32_t len, uint32_t blockSize = 64) const;

    [[nodiscard]] uint16_t getWidth() const;

    [[nodiscard]] uint16_t getHeight() const;
};

#endif // LCDLIBRARY_LCDCONTROLLER_H
