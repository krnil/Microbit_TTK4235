#include "uart.h"
#include "gpio.h"

int main() {
    uart_init();
    GPIO->PIN_CNF[17]=0;
    GPIO->PIN_CNF[26]=0;


    while (1){

        if (~(GPIO->IN) & (1<<17)) {
            uart_send('A');
            while(~(GPIO->IN) & (1<<17));
        }

        if (~(GPIO->IN) & (1<<26)) {
            uart_send('B');
            while(~(GPIO->IN) & (1<<26));
        }

        uint8_t received_byte;
        if(uart_receive(&received_byte)){

            if(received_byte >= 'a' && received_byte <= 'z'){
                received_byte += 'A' - 'a';
            }

            uart_send(received_byte);
            uart_send('\n');
            uart_send('\r');
        }

    }

    return 0;
}
