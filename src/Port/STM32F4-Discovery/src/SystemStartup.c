/*
 * SystemStartup.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include <stdint.h>
#include <string.h>


typedef struct {
    // Cortex M4 Core Interrupts
	uint32_t EndOfStack;
	void (*ResetHandler)(void);
    void (*NmiHandler)(void);
    void (*HardFaultHandler)(void);
    void (*MemoryFaultHandler)(void);
    void (*BusFaultHandler)(void);
    void (*UsageFaultHandler)(void);
    void (*Reserved7)(void);
    void (*Reserved8)(void);
    void (*Reserved9)(void);
    void (*Reserved10)(void);
    void (*SVCallHandler)(void);
    void (*ReservedForDebug)(void);
    void (*Reserved13)(void);
    void (*PendSVHandler)(void);
    void (*SystickHandler)(void);

    // Peripheral Interrupts
    void (*unused1[16])(void);
    void (*Dma1Stream5)(void);
    void (*Dma1Stream6)(void);
} InterruptVector_t;

extern int main (void);
extern void uart_receiveInterrupt (void);
extern void uart_transmitInterrupt (void);
void ResetHandler (void);


InterruptVector_t InterruptVector = {
		0x20030000,
		ResetHandler,
		.Dma1Stream5 = uart_receiveInterrupt,
		.Dma1Stream6 = uart_transmitInterrupt
};


__attribute__((noreturn))
void ResetHandler (void) {
	// set up clock tree and external memory

	// Initialize RAM
	extern uint8_t _sdata;
	extern uint8_t _edata;
	extern uint8_t _sidata;
	memcpy(&_sdata, &_sidata, &_edata - &_sdata);
    extern uint8_t _sbss;
    extern uint8_t _ebss;
	memset(&_sbss, 0, &_ebss - &_sbss);

	// execute preinit array
    extern void (*__preinit_array_start[]) (void);
    extern void (*__preinit_array_end[]) (void);
    for (uint32_t i = 0; i< (__preinit_array_end - __preinit_array_start); i++) {
        __preinit_array_start[i]();
    }
    // execute init array
    extern void (*__init_array_start[]) (void);
    extern void (*__init_array_end[]) (void);
    for (uint32_t i = 0; i< (__init_array_end - __init_array_start); i++) {
        __init_array_start[i]();
    }

	// call to main
	main();

	// execute fini array
    extern void (*__fini_array_start[]) (void);
    extern void (*__fini_array_end[]) (void);
    for (uint32_t i = 0; i< (__fini_array_end - __fini_array_start); i++) {
        __fini_array_start[i]();
    }

	// main has returned, stop execution
	while (1);
}
