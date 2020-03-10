#include "uart.h"
#include "gpio.h"

int main() {
    uart_init();
    while (1){

        if (~(GPIO->IN) & (1<<17)) {
            uart_send('A');
        }

        if (~(GPIO->IN) & (1<<26)) {
            uart_send('B');
        }

    }
    return 0;
}