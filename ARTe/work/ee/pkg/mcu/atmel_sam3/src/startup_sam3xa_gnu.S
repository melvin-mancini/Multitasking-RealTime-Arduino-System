/* File: startup_ARMCM3.S
 * Purpose: startup file for Cortex-M3 devices. Should use with 
 *   GCC for ARM Embedded Processors
 * Version: V1.4
 * Date: 09 July 2012
 * 
 * Copyright (c) 2011, 2012, ARM Limited
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "eecfg.h"	/* Configurable by RT-Druid */

	.syntax unified
	.arch armv7-m
	.cpu cortex-m3
	.thumb

	.globl  EE_cortex_mx_default_ISR
	.globl  __isr_vector

	.section .stack
	.align 3
#ifdef __STACK_SIZE
	.equ    Stack_Size, __STACK_SIZE
#else
	.equ    Stack_Size, 0xc00
#endif
	.globl    __StackTop
	.globl    __StackLimit
__StackLimit:
	.space    Stack_Size
	.size __StackLimit, . - __StackLimit
__StackTop:
	.size __StackTop, . - __StackTop

	.section .heap
	.align 3
#ifdef __HEAP_SIZE
	.equ    Heap_Size, __HEAP_SIZE
#else
	.equ    Heap_Size, 0
#endif
	.globl    __HeapBase
	.globl    __HeapLimit
__HeapBase:
	.if    Heap_Size
	.space    Heap_Size
	.endif
	.size __HeapBase, .-__HeapBase
__HeapLimit:
	.size __HeapLimit, .-__HeapLimit


/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/
	.section  .text.EE_cortex_mx_default_reset_ISR
	.weak    EE_cortex_mx_default_reset_ISR
	.type    EE_cortex_mx_default_reset_ISR, %function
EE_cortex_mx_default_reset_ISR:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in 
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

	ldr    r1, =__etext
	ldr    r2, =__data_start__
	ldr    r3, =__data_end__

#if 1
/* Here are two copies of loop implemenations. First one favors code size
 * and the second one favors performance. Default uses the first one. 
 * Change to "#if 0" to use the second one */
.LC0:
	cmp     r2, r3
	ittt    lt
	ldrlt   r0, [r1], #4
	strlt   r0, [r2], #4
	blt    .LC0
#else
	subs    r3, r2
	ble    .LC1    
.LC0:
	subs    r3, #4
	ldr    r0, [r1, r3]
	str    r0, [r2, r3]
	bgt    .LC0
.LC1:
#endif

#if 1 
/* #ifdef __STARTUP_CLEAR_BSS */
/*     This part of work usually is done in C library startup code. Otherwise,
 *     define this macro to enable it in this startup.
 *
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
	ldr r1, =__bss_start__
	ldr r2, =__bss_end__

	movs    r0, 0
.LC2:
	cmp     r1, r2
	itt    lt
	strlt   r0, [r1], #4
	blt    .LC2
#endif /* __STARTUP_CLEAR_BSS */

/* Call the clock system intitialization function.*/
#ifndef __NO_SYSTEM_INIT
	bl    SystemInit
#endif

/* Call the application's entry point.*/
	bl  main
	bx  lr    
	.pool
	.size EE_cortex_mx_default_reset_ISR, .-EE_cortex_mx_default_reset_ISR


/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
	.section  .text.EE_cortex_mx_default_ISR,"ax",%progbits
EE_cortex_mx_default_ISR:
#ifdef	DEBUG
	bkpt	#0x00
#endif	/* DEBUG */
Infinite_Loop:
	b  Infinite_Loop
	.size  EE_cortex_mx_default_ISR, .-EE_cortex_mx_default_ISR

/******************************************************************************
*																			
* This is the code that gets called when the processor receives a BUS FAULT 
* exception.
*
*******************************************************************************/
#ifdef	__AUTOSAR_R4_0__
	.section  .text.EE_cortex_mx_as_bus_fault_ISR,"ax",%progbits
EE_cortex_mx_as_bus_fault_ISR:
#ifdef	DEBUG
	bkpt	#0x00
#endif	/* DEBUG */
	b	EE_cortex_mx_as_bus_fault_ISR
	.size  EE_cortex_mx_as_bus_fault_ISR, .-EE_cortex_mx_as_bus_fault_ISR
#endif	/* __AUTOSAR_R4_0__ */

/******************************************************************************
*
* This is the code that gets called when the processor receives a HARD FAULT
* exception.
*
*******************************************************************************/
#ifdef	__AUTOSAR_R4_0__
	.section  .text.EE_cortex_mx_as_hard_fault_ISR,"ax",%progbits
EE_cortex_mx_as_hard_fault_ISR:
#ifdef	DEBUG
	bkpt	#0x00
#endif	/* DEBUG */
	b	EE_cortex_mx_as_hard_fault_ISR
	.size  EE_cortex_mx_as_hard_fault_ISR, .-EE_cortex_mx_as_hard_fault_ISR
#endif	/* __AUTOSAR_R4_0__ */


/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/

	.section .isr_vector
	.type  __isr_vector, %object
	.size  __isr_vector, .-__isr_vector

__isr_vector:
	.word    __StackTop                     /* Top of Stack */
	.word    EE_cortex_mx_default_reset_ISR /* Reset Handler */
	.word    EE_CORTEX_MX_NMI_ISR           /* NMI Handler */
	.word    EE_CORTEX_MX_HARD_FAULT_ISR    /* Hard Fault Handler */
	.word    EE_CORTEX_MX_MPU_FAULT_ISR     /* MPU Fault Handler */
	.word    EE_CORTEX_MX_BUS_FAULT_ISR     /* Bus Fault Handler */
	.word    EE_CORTEX_MX_USAGE_FAULT_ISR   /* Usage Fault Handler */
	.word    0                              /* Reserved */
	.word    0                              /* Reserved */
	.word    0                              /* Reserved */
	.word    0                              /* Reserved */
	.word    EE_cortex_mx_svc_ISR           /* SVCall Handler */
	.word    EE_CORTEX_MX_DEBUG_MONITOR_ISR /* Debug Monitor Handler */
	.word    0                              /* Reserved */
	.word    EE_cortex_mx_pendsv_ISR        /* PendSV Handler */
	.word    EE_CORTEX_MX_SYSTICK_ISR       /* SysTick Handler */

	/* External interrupts */
	/* MM: Check!!! */
	.word    EE_CORTEX_MX_SUPC_ISR          /*  0  Supply Controller */
	.word    EE_CORTEX_MX_RSTC_ISR          /*  1  Reset Controller */
	.word    EE_CORTEX_MX_RTC_ISR           /*  2  Real Time Clock */
	.word    EE_CORTEX_MX_RTT_ISR           /*  3  Real Time Timer */
	.word    EE_CORTEX_MX_WDT_ISR           /*  4  Watchdog Timer */
	.word    EE_CORTEX_MX_PMC_ISR           /*  5  PMC */
	.word    EE_CORTEX_MX_EFC0_ISR          /*  6  EFC 0 */
	.word    EE_CORTEX_MX_EFC1_ISR          /*  7  EFC 1 */
	.word    EE_CORTEX_MX_UART_ISR          /*  8  UART */
	.word    EE_CORTEX_MX_SMC_ISR           /*  9  SMC */
	.word    EE_CORTEX_MX_SDRAMC_ISR        /* 10  SDRAMC */
	.word    EE_CORTEX_MX_PIOA_ISR          /* 11 Parallel IO Controller A */
	.word    EE_CORTEX_MX_PIOB_ISR          /* 12 Parallel IO Controller B */
	.word    EE_CORTEX_MX_PIOC_ISR          /* 13 Parallel IO Controller C */
	.word    EE_CORTEX_MX_PIOD_ISR          /* 14 Parallel IO Controller D */
	.word    EE_CORTEX_MX_PIOE_ISR          /* 15 Parallel IO Controller E */
	.word    EE_CORTEX_MX_PIOF_ISR          /* 16 Parallel IO Controller F */
	.word    EE_CORTEX_MX_USART0_ISR        /* 17 USART 0 */
	.word    EE_CORTEX_MX_USART1_ISR        /* 18 USART 1 */
	.word    EE_CORTEX_MX_USART2_ISR        /* 19 USART 2 */
	.word    EE_CORTEX_MX_USART3_ISR        /* 20 USART 3 */
	.word    EE_CORTEX_MX_HSMCI_ISR         /* 21 MCI */
	.word    EE_CORTEX_MX_TWI0_ISR          /* 22 TWI 0 */
	.word    EE_CORTEX_MX_TWI1_ISR          /* 23 TWI 1 */
	.word    EE_CORTEX_MX_SPI0_ISR          /* 24 SPI 0 */
	.word    EE_CORTEX_MX_SPI1_ISR          /* 25 SPI 1 */
	.word    EE_CORTEX_MX_SSC_ISR           /* 26 SSC */
	.word    EE_CORTEX_MX_TC0_ISR           /* 27 Timer Counter 0 */
	.word    EE_CORTEX_MX_TC1_ISR           /* 28 Timer Counter 1 */
	.word    EE_CORTEX_MX_TC2_ISR           /* 29 Timer Counter 2 */
	.word    EE_CORTEX_MX_TC3_ISR           /* 30 Timer Counter 3 */
	.word    EE_CORTEX_MX_TC4_ISR           /* 31 Timer Counter 4 */
	.word    EE_CORTEX_MX_TC5_ISR           /* 32 Timer Counter 5 */
	.word    EE_CORTEX_MX_TC6_ISR           /* 33 Timer Counter 6 */
	.word    EE_CORTEX_MX_TC7_ISR           /* 34 Timer Counter 7 */
	.word    EE_CORTEX_MX_TC8_ISR           /* 35 Timer Counter 8 */
	.word    EE_CORTEX_MX_PWM_ISR           /* 36 PWM */
	.word    EE_CORTEX_MX_ADC_ISR           /* 37 ADC controller */
	.word    EE_CORTEX_MX_DACC_ISR          /* 38 DAC controller */
	.word    EE_CORTEX_MX_DMAC_ISR          /* 39 DMA Controller */
	.word    EE_CORTEX_MX_UOTGHS_ISR        /* 40 USB OTG High Speed */
	.word    EE_CORTEX_MX_TRNG_ISR          /* 41 True Random Number Generator */
	.word    EE_CORTEX_MX_EMAC_ISR          /* 42 Ethernet MAC */
	.word    EE_CORTEX_MX_CAN0_ISR          /* 43 CAN Controller 0 */
	.word    EE_CORTEX_MX_CAN1_ISR          /* 44 CAN Controller 1 */
    

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the EE_cortex_mx_default_ISR. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/


	.weak      EE_CORTEX_MX_NMI_ISR
	.thumb_set EE_CORTEX_MX_NMI_ISR,EE_cortex_mx_default_ISR
  
	.weak      EE_CORTEX_MX_HARD_FAULT_ISR
#ifdef	__AUTOSAR_R4_0__  
	.thumb_set EE_CORTEX_MX_HARD_FAULT_ISR,EE_cortex_mx_as_hard_fault_ISR
#else
	.thumb_set EE_CORTEX_MX_HARD_FAULT_ISR,EE_cortex_mx_default_ISR
#endif /*__AUTOSAR_R4_0__*/
  
	.weak      EE_CORTEX_MX_MPU_FAULT_ISR
	.thumb_set EE_CORTEX_MX_MPU_FAULT_ISR,EE_cortex_mx_default_ISR
  
	.weak      EE_CORTEX_MX_BUS_FAULT_ISR
#ifdef	__AUTOSAR_R4_0__  
	.thumb_set EE_CORTEX_MX_BUS_FAULT_ISR,EE_cortex_mx_as_bus_fault_ISR
#else
	.thumb_set EE_CORTEX_MX_BUS_FAULT_ISR,EE_cortex_mx_default_ISR
#endif /*__AUTOSAR_R4_0__*/
   
	.weak      EE_CORTEX_MX_USAGE_FAULT_ISR
	.thumb_set EE_CORTEX_MX_USAGE_FAULT_ISR,EE_cortex_mx_default_ISR

	.weak      EE_cortex_mx_svc_ISR
	.thumb_set EE_cortex_mx_svc_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_DEBUG_MONITOR_ISR
	.thumb_set EE_CORTEX_MX_DEBUG_MONITOR_ISR,EE_cortex_mx_default_ISR

	.weak      EE_cortex_mx_pendsv_ISR
	.thumb_set EE_cortex_mx_pendsv_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_SYSTICK_ISR
	.thumb_set EE_CORTEX_MX_SYSTICK_ISR,EE_cortex_mx_default_ISR              

	/* External interrupts */

	.weak      EE_CORTEX_MX_SUPC_ISR          /*  0  Supply Controller */
	.thumb_set EE_CORTEX_MX_SUPC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_RSTC_ISR          /*  1  Reset Controller */
	.thumb_set EE_CORTEX_MX_RSTC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_RTC_ISR           /*  2  Real Time Clock */
	.thumb_set EE_CORTEX_MX_RTC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_RTT_ISR           /*  3  Real Time Timer */
	.thumb_set EE_CORTEX_MX_RTT_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_WDT_ISR           /*  4  Watchdog Timer */
	.thumb_set EE_CORTEX_MX_WDT_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PMC_ISR           /*  5  PMC */
	.thumb_set EE_CORTEX_MX_PMC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_EFC0_ISR          /*  6  EFC 0 */
	.thumb_set EE_CORTEX_MX_EFC0_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_EFC1_ISR          /*  7  EFC 1 */
	.thumb_set EE_CORTEX_MX_EFC1_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_UART_ISR          /*  8  UART */
	.thumb_set EE_CORTEX_MX_UART_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_SMC_ISR           /*  9  SMC */
	.thumb_set EE_CORTEX_MX_SMC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_SDRAMC_ISR        /* 10  SDRAMC */
	.thumb_set EE_CORTEX_MX_SDRAMC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PIOA_ISR          /* 11 Parallel IO Controller A */
	.thumb_set EE_CORTEX_MX_PIOA_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PIOB_ISR          /* 12 Parallel IO Controller B */
	.thumb_set EE_CORTEX_MX_PIOB_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PIOC_ISR          /* 13 Parallel IO Controller C */
	.thumb_set EE_CORTEX_MX_PIOC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PIOD_ISR          /* 14 Parallel IO Controller D */
	.thumb_set EE_CORTEX_MX_PIOD_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PIOE_ISR          /* 15 Parallel IO Controller E */
	.thumb_set EE_CORTEX_MX_PIOE_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PIOF_ISR          /* 16 Parallel IO Controller F */
	.thumb_set EE_CORTEX_MX_PIOF_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_USART0_ISR        /* 17 USART 0 */
	.thumb_set EE_CORTEX_MX_USART0_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_USART1_ISR        /* 18 USART 1 */
	.thumb_set EE_CORTEX_MX_USART1_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_USART2_ISR        /* 19 USART 2 */
	.thumb_set EE_CORTEX_MX_USART2_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_USART3_ISR        /* 20 USART 3 */
	.thumb_set EE_CORTEX_MX_USART3_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_HSMCI_ISR         /* 21 MCI */
	.thumb_set EE_CORTEX_MX_HSMCI_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TWI0_ISR          /* 22 TWI 0 */
	.thumb_set EE_CORTEX_MX_TWI0_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TWI1_ISR          /* 23 TWI 1 */
	.thumb_set EE_CORTEX_MX_TWI1_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_SPI0_ISR          /* 24 SPI 0 */
	.thumb_set EE_CORTEX_MX_SPI0_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_SPI1_ISR          /* 25 SPI 1 */
	.thumb_set EE_CORTEX_MX_SPI1_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_SSC_ISR           /* 26 SSC */
	.thumb_set EE_CORTEX_MX_SSC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC0_ISR           /* 27 Timer Counter 0 */
	.thumb_set EE_CORTEX_MX_TC0_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC1_ISR           /* 28 Timer Counter 1 */
	.thumb_set EE_CORTEX_MX_TC1_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC2_ISR           /* 29 Timer Counter 2 */
	.thumb_set EE_CORTEX_MX_TC2_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC3_ISR           /* 30 Timer Counter 3 */
	.thumb_set EE_CORTEX_MX_TC3_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC4_ISR           /* 31 Timer Counter 4 */
	.thumb_set EE_CORTEX_MX_TC4_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC5_ISR           /* 32 Timer Counter 5 */
	.thumb_set EE_CORTEX_MX_TC5_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC6_ISR           /* 33 Timer Counter 6 */
	.thumb_set EE_CORTEX_MX_TC6_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC7_ISR           /* 34 Timer Counter 7 */
	.thumb_set EE_CORTEX_MX_TC7_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TC8_ISR           /* 35 Timer Counter 8 */
	.thumb_set EE_CORTEX_MX_TC8_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_PWM_ISR           /* 36 PWM */
	.thumb_set EE_CORTEX_MX_PWM_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_ADC_ISR           /* 37 ADC controller */
	.thumb_set EE_CORTEX_MX_ADC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_DACC_ISR          /* 38 DAC controller */
	.thumb_set EE_CORTEX_MX_DACC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_DMAC_ISR          /* 39 DMA Controller */
	.thumb_set EE_CORTEX_MX_DMAC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_UOTGHS_ISR        /* 40 USB OTG High Speed */
	.thumb_set EE_CORTEX_MX_UOTGHS_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_TRNG_ISR          /* 41 True Random Number Generator */
	.thumb_set EE_CORTEX_MX_TRNG_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_EMAC_ISR          /* 42 Ethernet MAC */
	.thumb_set EE_CORTEX_MX_EMAC_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_CAN0_ISR          /* 43 CAN Controller 0 */
	.thumb_set EE_CORTEX_MX_CAN0_ISR,EE_cortex_mx_default_ISR

	.weak      EE_CORTEX_MX_CAN1_ISR          /* 44 CAN Controller 1 */
	.thumb_set EE_CORTEX_MX_CAN1_ISR,EE_cortex_mx_default_ISR
