#include "mini_uart.h"

void kernel_main(void)
{
        const unsigned int BAUDRATE = 115200;
	uart_init(BAUDRATE);
	uart_send_string("Hello, world!\r\n");

	while (1) {
		uart_send(uart_recv());
	}
}
