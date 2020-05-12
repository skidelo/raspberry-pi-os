#include "mini_uart.h"
#include "utils.h"

volatile unsigned int i = 0;

void kernel_main(void)
{
    unsigned int coreNum = getCoreNum();
    unsigned int coreExecutionOrder[] = {2, 1, 3, 0};

    // Busy wait until it's the core's turn
    while (coreNum != coreExecutionOrder[i]) {}

    if (coreExecutionOrder[0] == coreNum)
        uart_init();

    char coreNumStr[2] = {coreNum+'0', '\0'};
    uart_send_string("Hello, from core ");
    uart_send_string(coreNumStr);
    uart_send_string("!\r\n");

    ++i;

    // Wait for everyone to say hi
    while(i != 4) { }

    while (1) {
        if (coreExecutionOrder[0] == coreNum)
            uart_send(uart_recv());
    }
}
