#include "SPIInterface.h"

void SPIInterface::init() const {
    gpio_init(dataCommandPin);
    gpio_init(chipSelectPin);

    gpio_set_dir(dataCommandPin, GPIO_OUT);
    gpio_set_dir(chipSelectPin, GPIO_OUT);

    gpio_put(chipSelectPin, true);
    gpio_put(dataCommandPin, false);
}


