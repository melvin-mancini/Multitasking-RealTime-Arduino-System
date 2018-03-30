/* Linker script */

/* Memory regions (MCU specific) */
MEMORY
{
	FLASH (rx)  : ORIGIN = 0x00080000, LENGTH = 0x00080000 /* Flash, 512K */
	SRAM0 (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00010000 /* sram0, 64K */
	SRAM1 (rwx) : ORIGIN = 0x20080000, LENGTH = 0x00008000 /* sram1, 32K */
	RAM (rwx)   : ORIGIN = 0x20070000, LENGTH = 0x00018000 /* sram, 96K */
}

/* Output format and libbrary path */
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)
 
/* Default entry point for execution */
ENTRY(EE_cortex_mx_default_reset_ISR)

/* List of defined symbols in sections:
 *
 * __preinit_array_start
 * __preinit_array_end
 * __init_array_start
 * __init_array_end
 * __fini_array_start
 * __fini_array_end
 * 
 * __exidx_start
 * __exidx_end
 *
 * __etext
 * _start_data
 * _end_data
 *
 */

/* define stack size and heap size here */
stack_size = DEFINED(stack_size) ? stack_size : 0x2000 ;
heap_size  = DEFINED(heap_size)  ? heap_size  : 0x0100 ;
 
/* define beginning and ending of stack */
_stack_start = ORIGIN(RAM)+LENGTH(RAM);
_stack_end = _stack_start - stack_size;

SECTIONS
{
	. = ORIGIN(FLASH);

	/* Interrupt Vector Table */
	vectors :
	{
		. = ALIGN(4);
		_sfixed = .;
		KEEP(*(.vectors .vectors.*))
	} >FLASH

	/* Text section, rodata section, ... */
	.text : 
	{
		/* text input sections */
		. = ALIGN(4);
		*(.text .stub .text.* .gnu.linkonce.t.*)

		/*init contains constructor functions called before entering main. used by crt (?).*/
		. = ALIGN(4);
		KEEP(*(.init))
 
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
 
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
 
		/*fini contains destructor functions called after leaving main. used by crt (?).*/
		. = ALIGN(4);
		KEEP(*(.fini))
 
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;
 
		/* constructors */
		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
 
		/* destructors */
		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))

		/* read only data */
		*(.rodata.* .gnu.linkonce.r.*)
		*(.rodata)

		KEEP(*(.eh_frame*))

		/* TODO: do we really need that section? */
		/**(.comment)*/
		/* .gnu.warning sections are handled specially by elf32.em. */
		*(.gnu.warning)
		*(.glue_7t) *(.glue_7)
		/* exception table */
		*(.gcc_except_table)
	} > FLASH

	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
	__exidx_end = .;

	. = ALIGN(4);
	__etext = .;
	_etext = .;
	/* used by the startup to initialize data */
 	_sidata = .; 

/* 	.data: AT (_sidata) */
	.data : AT (_sidata)
	{
		. = ALIGN(4);
		__data_start = .;
		_sdata = .;
		_srelocate = .;

		*(vtable)
		*(.ramfunc .ramfunc.*);
		*(.data .data.*);

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

 		. = ALIGN(4); 
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

 		. = ALIGN(4); 
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		KEEP(*(.jcr*))

		. = ALIGN(4);
		__data_end = .;
		_edata = .;
		_erelocate = .;
	} >RAM

	/* .bss section which is used for uninitialized data */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = . ;
		_szero = . ;
		__bss_start = .;
		__bss_start__ = .;
		*(.dynbss)
		*(.bss .bss.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = . ;
		_ezero = . ;
		__bss_end = .;
		__bss_end__ = .;
	} > RAM
	__bss_size = __bss_end - __bss_start;

	/* */
	. = ALIGN(4);
	_end = .;
	__end = .;
	__end__ = .;
	.heap :
	{
		_heap_start = .;
		. = . + heap_size;
	} > RAM
 
	. = ALIGN(4);
	. = _stack_end;
	_sstack = .;
	.stack :
	{
		. = . + stack_size;
	} > RAM
	_stack_start = .;
	_estack = .;

	/* Remove information from the standard libraries */
	/*/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}*/
}