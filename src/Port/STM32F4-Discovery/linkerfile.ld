

ENTRY(ResetHandler)

MEMORY
{
	IBUS (xr)	:	ORIGIN = 0x00000000, LENGTH = 2M
	FLASH (rx)	:	ORIGIN = 0x08000000, LENGTH = 2M
	CCMRAM (xrw):	ORIGIN = 0x10000000, LENGTH = 64K
	RAM (xrw)	:	ORIGIN = 0x20000000, LENGTH = 128K
}


SECTIONS
{
	.vectors : {
		KEEP(*(*.InterruptVector))
	} > IBUS AT > RAM
	
	.text : {
		/* user code */
		*(.text)
		*(.text*)
		
		/* ARM code */
		*(.glue_7)
		*(.glue_7t)
		*(.vfp11_veneer)
		*(.v4_bx)
		
		/* const data */
		*(.rodata)
		*(.rodata.*)
		
		/* arrays */
		__preinit_array_start = .;
		KEEP(*(.preinit_array));
		KEEP(*(.preinit_array.*));
		__preinit_array_end = .;
		
		__init_array_start = .;
		KEEP(*(.init_array));
		KEEP(*(.init_array.*));
		__init_array_end = .;
		
		__fini_array_start = .;
		KEEP(*(.fini_array));
		KEEP(*(.fini_array.*));
		__fini_array_end = .;
		
	} > IBUS AT > RAM
	
	.data : {
		_sdata = .;
		*(.data)
		*(.data*)
		_edata = .;
	} > RAM AT > RAM
	_sidata = LOADADDR(.data);
	
	.bss (NOLOAD) : {
		_sbss = .;
		*(.bss)
		*(.bss*)
		*(.COMMON)
		_ebss = .;
	} > RAM
	
}
