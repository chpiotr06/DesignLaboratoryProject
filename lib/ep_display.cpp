#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>


#define MISO 21
#define CS 19
#define SCLK 18 
#define MOSI 17

#define SPI_PORT spi0

#define CMD_DEEP_SLEEP_MODE 0x10


class EP_display{
    public:
        EP_display() {
            spi_init(SPI_PORT);
            gpio_set_function(MISO, GPIO_FUNC_SPI);
            gpio_set_function(SCLK, GPIO_FUNC_SPI);
            gpio_set_function(MOSI, GPIO_FUNC_SPI);

            gpio_init(CS);
            gpio_set_dir(CS, GPIO_OUT);
            gpio_put(CS, 1);
            
        }
        ~EP_display(){
            //  Disable SPI pins? 
        }

};


