
#include "uart.h"

#define PIN_TX 24
#define PIN_RX 25
#define PIN_DISABLE 0xFFFFFFFF
#define BAUD_9600 0x00275000

#define UART_DISABLE 0
#define UART_ENABLE 4

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

void uart_init() {
    // for (int i=4;i<=15;i++) {
    //     GPIO->DIRSET=(1<<i);
    //     GPIO->OUTCLR=(1<<i);
    // }
    // GPIO->PIN_CNF[17]=0;
    // GPIO->PIN_CNF[26]=0;

    UART->ENABLE = UART_DISABLE;

    GPIO->PIN_CNF[PIN_TX] = 1;
    GPIO->PIN_CNF[PIN_RX] = 0;

    UART->PSELTXD = PIN_TX;         // TXD med pin 24
    UART->PSELRXD = PIN_RX;         // RXD med pin 25
    UART->BAUDRATE = BAUD_9600;     // Baudrate på 9600

    UART->PSELRTS = PIN_DISABLE;    // Ingen RTS-kobling
    UART->PSELCTS = PIN_DISABLE;    // Ingen CTS-kobling

    UART->ENABLE = UART_ENABLE;     // Enable UART
    UART->STARTRX = 1;              // Begynn å ta i mot meldinger
}

void uart_send(uint8_t byte) {
    UART->STARTTX = 1;
    UART->TXD = byte;

    while(!(UART->TXDRDY));
    UART->TXDRDY = 0;

    UART->STOPTX = 1;
}

// char uart_read() {
//     char letter;
//     UART->RXDRDY = 0;
//     if (UART->RXDRDY) {
//         letter=UART->RXD;
//     }
//     else {
//         letter='\0';
//     }
//     return letter;
// }

uint8_t uart_receive(uint8_t* p_byte) {
    if(!UART->RXDRDY) {
        return 0;
    }

    UART->RXDRDY = 0;
    *p_byte = UART->RXD;
    return 1;
}