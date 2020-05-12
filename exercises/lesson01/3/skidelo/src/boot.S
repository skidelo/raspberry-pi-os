#include "mm.h"

.data
meminit: .byte 0
         .align 2

.section ".text.boot"

.globl _start
_start:
	mrs	x3, mpidr_el1
	and	x3, x3,#0xFF		// Check processor id
	cbnz	x3, wait_bss_init	// Delay for all non-primary CPU

bss_init:
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero
        ldr     x1, =meminit
        mov     w0, #1
        strb    w0, [x1]

wait_bss_init:
        ldr     x1, =meminit
        ldrb    w0, [x1]
        cbz     w0, wait_bss_init

stack_init:
        add     x3, x3, #1      // 1-indexing for processor id
        mov     x2, #LOW_MEMORY
        mul     x1, x3, x2      // Stack pointer offsets by 4MB (4MB, 8MB, 12MB, 16MB)
        mov     sp, x1

        bl      kernel_main
        b       proc_hang       // Should never get here

proc_hang:
        b       proc_hang
