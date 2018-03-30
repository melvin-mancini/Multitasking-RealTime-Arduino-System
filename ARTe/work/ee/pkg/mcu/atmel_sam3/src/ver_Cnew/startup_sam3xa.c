/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following condition is met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

#include "ee.h"	/* Configurable by RT-Druid */
#include "sam3.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

//void __libc_init_array(void);
//void SystemInit(void);

/* Default empty handler */
void EE_cortex_mx_default_ISR(void);

/* Cortex-M3 core handlers */
void EE_CORTEX_MX_NMI_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
#ifdef	__AUTOSAR_R4_0__
void EE_CORTEX_MX_HARD_FAULT_ISR	( void ) __attribute__ ((weak, alias("EE_cortex_mx_as_halt_fault_ISR")));
#else	/* __AUTOSAR_R4_0__ */
void EE_CORTEX_MX_HARD_FAULT_ISR	( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
#endif	/* __AUTOSAR_R4_0__ */
void EE_CORTEX_MX_MPU_FAULT_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
#ifdef	__AUTOSAR_R4_0__
void EE_CORTEX_MX_BUS_FAULT_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_as_bus_fault_ISR")));
#else	/* __AUTOSAR_R4_0__ */
void EE_CORTEX_MX_BUS_FAULT_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
#endif	/* __AUTOSAR_R4_0__ */
void EE_CORTEX_MX_USAGE_FAULT_ISR	( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_cortex_mx_svc_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_DEBUG_MONITOR_ISR	( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_cortex_mx_pendsv_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_SYSTICK_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));

/* Peripherals handlers */
void EE_CORTEX_MX_SUPC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_RSTC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_RTC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_RTT_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_WDT_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PMC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_EFC0_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_EFC1_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_UART_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_SMC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_SDRAMC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PIOA_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PIOB_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PIOC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PIOD_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PIOE_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PIOF_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_USART0_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_USART1_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_USART2_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_USART3_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_HSMCI_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TWI0_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TWI1_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_SPI0_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_SPI1_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_SSC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC0_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC1_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC2_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC3_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC4_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC5_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC6_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC7_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TC8_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_PWM_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_ADC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_DACC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_DMAC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_UOTGHS_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_TRNG_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_EMAC_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_CAN0_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));
void EE_CORTEX_MX_CAN1_ISR		( void ) __attribute__ ((weak, alias("EE_cortex_mx_default_ISR")));

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(void*) (&_estack),
	(void*) EE_cortex_mx_default_ISR,

	(void*) EE_CORTEX_MX_NMI_ISR,		/* NMI Handler */
	(void*) EE_CORTEX_MX_HARD_FAULT_ISR,	/* Hard Fault Handler */
	(void*) EE_CORTEX_MX_MPU_FAULT_ISR,	/* MPU Fault Handler */
	(void*) EE_CORTEX_MX_BUS_FAULT_ISR,	/* Bus Fault Handler */
	(void*) EE_CORTEX_MX_USAGE_FAULT_ISR,	/* Usage Fault Handler */
	(void*) (0UL),				/* Reserved */
	(void*) (0UL),				/* Reserved */
	(void*) (0UL),				/* Reserved */
	(void*) (0UL),				/* Reserved */
	(void*) EE_cortex_mx_svc_ISR,		/* SVCall Handler */
	(void*) EE_CORTEX_MX_DEBUG_MONITOR_ISR,	/*Debug Monitor Handler */
	(void*) (0UL),				/* Reserved */
	(void*) EE_cortex_mx_pendsv_ISR,	/* PendSV Handler */
	(void*) EE_CORTEX_MX_SYSTICK_ISR,	/* SysTick Handler */

	/* Configurable interrupts */
	(void*) EE_CORTEX_MX_SUPC_ISR,    /*  0  Supply Controller */
	(void*) EE_CORTEX_MX_RSTC_ISR,    /*  1  Reset Controller */
	(void*) EE_CORTEX_MX_RTC_ISR,     /*  2  Real Time Clock */
	(void*) EE_CORTEX_MX_RTT_ISR,     /*  3  Real Time Timer */
	(void*) EE_CORTEX_MX_WDT_ISR,     /*  4  Watchdog Timer */
	(void*) EE_CORTEX_MX_PMC_ISR,     /*  5  PMC */
	(void*) EE_CORTEX_MX_EFC0_ISR,    /*  6  EFC 0 */
	(void*) EE_CORTEX_MX_EFC1_ISR,    /*  7  EFC 1 */
	(void*) EE_CORTEX_MX_UART_ISR,    /*  8  UART */
	(void*) EE_CORTEX_MX_SMC_ISR,     /*  9  SMC */
	(void*) EE_CORTEX_MX_SDRAMC_ISR,  /* 10  SDRAMC */
	(void*) EE_CORTEX_MX_PIOA_ISR,    /* 11 Parallel IO Controller A */
	(void*) EE_CORTEX_MX_PIOB_ISR,    /* 12 Parallel IO Controller B */
	(void*) EE_CORTEX_MX_PIOC_ISR,    /* 13 Parallel IO Controller C */
	(void*) EE_CORTEX_MX_PIOD_ISR,    /* 14 Parallel IO Controller D */
	(void*) EE_CORTEX_MX_PIOE_ISR,    /* 15 Parallel IO Controller E */
	(void*) EE_CORTEX_MX_PIOF_ISR,    /* 16 Parallel IO Controller F */
	(void*) EE_CORTEX_MX_USART0_ISR,  /* 17 USART 0 */
	(void*) EE_CORTEX_MX_USART1_ISR,  /* 18 USART 1 */
	(void*) EE_CORTEX_MX_USART2_ISR,  /* 19 USART 2 */
	(void*) EE_CORTEX_MX_USART3_ISR,  /* 20 USART 3 */
	(void*) EE_CORTEX_MX_HSMCI_ISR,   /* 21 MCI */
	(void*) EE_CORTEX_MX_TWI0_ISR,    /* 22 TWI 0 */
	(void*) EE_CORTEX_MX_TWI1_ISR,    /* 23 TWI 1 */
	(void*) EE_CORTEX_MX_SPI0_ISR,    /* 24 SPI 0 */
	(void*) EE_CORTEX_MX_SPI1_ISR,    /* 25 SPI 1 */
	(void*) EE_CORTEX_MX_SSC_ISR,     /* 26 SSC */
	(void*) EE_CORTEX_MX_TC0_ISR,     /* 27 Timer Counter 0 */
	(void*) EE_CORTEX_MX_TC1_ISR,     /* 28 Timer Counter 1 */
	(void*) EE_CORTEX_MX_TC2_ISR,     /* 29 Timer Counter 2 */
	(void*) EE_CORTEX_MX_TC3_ISR,     /* 30 Timer Counter 3 */
	(void*) EE_CORTEX_MX_TC4_ISR,     /* 31 Timer Counter 4 */
	(void*) EE_CORTEX_MX_TC5_ISR,     /* 32 Timer Counter 5 */
	(void*) EE_CORTEX_MX_TC6_ISR,     /* 33 Timer Counter 6 */
	(void*) EE_CORTEX_MX_TC7_ISR,     /* 34 Timer Counter 7 */
	(void*) EE_CORTEX_MX_TC8_ISR,     /* 35 Timer Counter 8 */
	(void*) EE_CORTEX_MX_PWM_ISR,     /* 36 PWM */
	(void*) EE_CORTEX_MX_ADC_ISR,     /* 37 ADC controller */
	(void*) EE_CORTEX_MX_DACC_ISR,    /* 38 DAC controller */
	(void*) EE_CORTEX_MX_DMAC_ISR,    /* 39 DMA Controller */
	(void*) EE_CORTEX_MX_UOTGHS_ISR,  /* 40 USB OTG High Speed */
	(void*) EE_CORTEX_MX_TRNG_ISR,    /* 41 True Random Number Generator */
	(void*) EE_CORTEX_MX_EMAC_ISR,    /* 42 Ethernet MAC */
	(void*) EE_CORTEX_MX_CAN0_ISR,    /* 43 CAN Controller 0 */
	(void*) EE_CORTEX_MX_CAN1_ISR     /* 44 CAN Controller 1 */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void EE_cortex_mx_default_reset_ISR(void)
{
	uint32_t *pSrc, *pDest;

	/* Initialize the relocate segment */
	pSrc = &_etext;
	pDest = &_srelocate;

	if (pSrc != pDest) {
		for (; pDest < &_erelocate;) {
			*pDest++ = *pSrc++;
		}
	}

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) {
		*pDest++ = 0;
	}

	/* Set the vector table base address */
	pSrc = (uint32_t *) & _sfixed;
	SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

	if (((uint32_t) pSrc >= IRAM0_ADDR) && ((uint32_t) pSrc < NFC_RAM_ADDR)) {
		SCB->VTOR |= (1UL) << SCB_VTOR_TBLBASE_Pos;
	}

	/* Initialize the C library */
	//__libc_init_array();

	/* Initialize the clock */
	//SystemInit();

	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}

/**
 * \brief This is the code that gets called when the processor receives an unexpected interrupt.
 */
void EE_cortex_mx_default_ISR(void)
{
	while (1) {
#ifdef	DEBUG
		__breakpoint(0);
#endif	/* DEBUG */
	}
}

#ifdef	__AUTOSAR_R4_0__
/**
 * \brief This is the code that gets called when the processor receives a BUS FAULT exception.
 */
void EE_cortex_mx_as_bus_fault_ISR(void)
{
	while (1) {
#ifdef	DEBUG
		__breakpoint(0);
#endif	/* DEBUG */
	}
}
#endif	/* __AUTOSAR_R4_0__ */

#ifdef	__AUTOSAR_R4_0__
/**
 * \brief This is the code that gets called when the processor receives a HARD FAULT exception.
 */
void EE_cortex_mx_as_halt_fault_ISR(void)
{
	while (1) {
#ifdef	DEBUG
		__breakpoint(0);
#endif	/* DEBUG */
	}
}
#endif	/* __AUTOSAR_R4_0__ */
