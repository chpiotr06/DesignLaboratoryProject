#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"



// ----------------------BLUETOOTH --------------------
#define BLUETOOTH_UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY  UART_PARITY_NONE

#define BUF_LEN         0x100
// ----------------------------------------------------------------

//----------------------GPS------------------------
#define GPS_UART_ID uart1
char uartBuffer[512];

#define GPS_RX_PIN 3 
#define GPS_TX_PIN 4


//----------------------------------------------------------------

#define led_pin 25







void bluetoothInit() {
    uart_init(BLUETOOTH_UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);


    // Send out a string, with CR/LF conversions
    int __unused actual = uart_set_baudrate(BLUETOOTH_UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    // uart_set_hw_flow(BLUETOOTH_UART_ID, false, false);

    // Set our data format
    uart_set_format(BLUETOOTH_UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(BLUETOOTH_UART_ID, false);
    uart_puts(BLUETOOTH_UART_ID, "\nHello you fool\r\n");
}

void bluetooth_send(const char *data ) {
    uart_puts(BLUETOOTH_UART_ID, data );
}


void gps_init()  {

    uart_init(GPS_UART_ID, 9600);
 
    gpio_set_function(GPS_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GPS_TX_PIN, GPIO_FUNC_UART);



}

void read_GPS() {

    bluetooth_send("Entering GPS read mode\r\n");
    if(uart_is_enabled(GPS_UART_ID)){   
        if(uart_is_readable(GPS_UART_ID)){
            // uart_read_blocking(GPS_UART_ID, uartBuffer, sizeof uartBuffer);
        } else {
            bluetooth_send( "uart unreadable \r\n" );
        }

    } else {
        bluetooth_send("urat disabled \r\n");
    }

    bluetooth_send("Fucked up here");
    for(int i = 0; i < 512 ; i++) {
        char * ptr;
        ptr = &uartBuffer[i];
        bluetooth_send(ptr);
    }
    bluetooth_send("\n");

}

void setup_succesful(){
    
    sleep_ms(500);
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, true);
    sleep_ms(1000);
}

int main() {
    
    sleep_ms(3000);

    stdio_init_all();
    bluetoothInit();
    gps_init();
    setup_succesful();
    

    while (true) {

        read_GPS();        

        gpio_put(led_pin, true);
        bluetooth_send("Led ON!\r\n");
        sleep_ms(1000);

        gpio_put(led_pin, false);
        bluetooth_send("LED OFF!\r\n");
        sleep_ms(1000);
    }
}
