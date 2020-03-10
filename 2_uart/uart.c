#include <stdint.h>
#include "uart.h"
#include "gpio.h"

#define UART ((NRF_UART_REG*)(0x40002000))

typedef struct {
    volatile uint32_t STARTRX;
    volatile uint32_t STOPRX;
    volatile uint32_t STARTTX;
    volatile uint32_t STOPTX;
    volatile uint32_t UNUSED0[3];
    volatile uint32_t SUSPEND;
    volatile uint32_t UNUSED1[56];
    volatile uint32_t CTS;
    volatile uint32_t NCTS;
    volatile uint32_t RXDRDY;
    volatile uint32_t UNUSED2[4];
    volatile uint32_t TXDRDY;
    volatile uint32_t UNUSED3;
    volatile uint32_t ERROR;
    volatile uint32_t UNUSED4[7];
    volatile uint32_t RXTO;
    volatile uint32_t UNUSED5[110];
    volatile uint32_t INTEN;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t UNUSED6[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t UNUSED7[31];
    volatile uint32_t ENABLE;
    volatile uint32_t UNUSED8;
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t UNUSED9;
    volatile uint32_t BAUDRATE;
    volatile uint32_t UNUSED10[17];
    volatile uint32_t CONFIG;
} NRF_UART_REG;

void uart_init(){
    for (int i=4;i<=15;i++){
        GPIO->DIRSET=(1<<i);
        GPIO->OUTCLR=(1<<i);
    }
    GPIO->PIN_CNF[17]=0;
    GPIO->PIN_CNF[26]=0;

    UART->PSELTXD = 24;         // TXD med pin 24
    UART->PSELRXD = 25;         // RXD med pin 25
    UART->BAUDRATE = 0x00275000;      // Baudrate på 9600

    UART->PSELRTS = 0xFFFFFFFF; // Ingen RTS-kobling
    UART->PSELCTS = 0xFFFFFFFF; // Ingen CTS-kobling

    UART->ENABLE = 4;           // Enable UART
    UART->STARTRX = 1;          // Begynn å ta i mot meldinger
}

void uart_send(char letter) {
    UART->STARTTX = 1;
    UART->TXD = letter;
    while(!(UART->TXDRDY));

    UART->STOPTX = 1;
}

char uart_read(){
    char letter;
    UART->RXDRDY = 0;
    if (UART->RXDRDY){
        letter=UART->RXD;
    }else{
        letter='\0';
    }
    return letter;
}