#include "printf.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(unsigned int el)
{
	uart_init();
	init_printf(0, putc);
	printf("Exception level: %d \r\n", el);

        if (1 == el) {
            while (1) {
                uart_send(uart_recv());
            }
        }
}
