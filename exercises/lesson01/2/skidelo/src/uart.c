#include "utils.h"
#include "peripherals/uart.h"
#include "peripherals/gpio.h"
#include <stdio.h>

void uart_send ( char c )
{
	while( get32(UART_FR) & 0x20 ) {}

        unsigned int dr = get32(UART_DR);
        dr &= ~0xFF;
        dr |= c;
	put32(UART_DR,dr);
}

char uart_recv ( void )
{
        while( get32(UART_FR) & 0x10 ) {}

        return(get32(UART_DR)&0xFF);
}

void uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_send((char)str[i]);
	}
}

void uart_init ( void )
{
        unsigned int selector;

        selector = get32(GPFSEL1);
        selector &= ~(7<<12);                   // clean gpio14
        selector |= 4<<12;                      // set alt0 for gpio14
        selector &= ~(7<<15);                   // clean gpio15
        selector |= 4<<15;                      // set alt0 for gpio15
        put32(GPFSEL1,selector);

        put32(GPPUD,0);
        delay(150);
        put32(GPPUDCLK0,(1<<14)|(1<<15));
        delay(150);
        put32(GPPUDCLK0,0);

        // Assume fresh boot
        put32(UART_CR, 0);                      // Disable uart

        // See example 3-1 here:
        // http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183f/DDI0183.pdf
        // bauddiv = FUARTCLK / (16 * baudrate)
        // Assume FUARTCLK = 48 MHz b/c I cheated and looked at other excercises
        //
        // Can't do floating point math b/c of -mgeneral-regs-only compile option
        // bauddiv = 48000000.f / (16 * baudrate);
        // put32(UART_IBRD, (unsigned int)bauddiv);
        // put32(UART_FBRD, (unsigned int)((bauddiv - (unsigned int)bauddiv) * 64) + 0.5);
        //
        put32(UART_IBRD, 26);
        put32(UART_FBRD, 3);
        put32(UART_LCRH,7<<4);                  // Enable 8 bit words and enable FIFOs
        put32(UART_CR,3<<8 | 1);                // Enable uart (UARTEN, RXE, TXE)
}

