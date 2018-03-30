/** @file	ee_atmel_atxmega_tcx0.h
 *
 *  @brief	Erika Enterprise - TCX0 - Atmel ATXMEGA MCUs Header File.
 *
 *  <!--
 *    ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 *    Copyright (C) 2002-2013  Evidence Srl
 *
 *    This file is part of ERIKA Enterprise.
 *
 *    ERIKA Enterprise is free software; you can redistribute it
 *    and/or modify it under the terms of the GNU General Public License
 *    version 2 as published by the Free Software Foundation, 
 *    (with a special exception described below).
 *
 *    Linking this code statically or dynamically with other modules is
 *    making a combined work based on this code.  Thus, the terms and
 *    conditions of the GNU General Public License cover the whole
 *    combination.
 *
 *    As a special exception, the copyright holders of this library give you
 *    permission to link this code with independent modules to produce an
 *    executable, regardless of the license terms of these independent
 *    modules, and to copy and distribute the resulting executable under
 *    terms of your choice, provided that you also meet, for each linked
 *    independent module, the terms and conditions of the license of that
 *    module.  An independent module is a module which is not derived from
 *    or based on this library.  If you modify this code, you may extend
 *    this exception to your version of the code, but you are not
 *    obligated to do so.  If you do not wish to do so, delete this
 *    exception statement from your version.
 *
 *    ERIKA Enterprise is distributed in the hope that it will be
 *    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License version 2 for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    version 2 along with ERIKA Enterprise; if not, write to the
 *    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301 USA.
 *  -->
 *
 *  This file contains Internal data types and functions declaration for
 *  TCX0 of Atmel ATXMEGA MCUs.
 *
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_TCX0_H__
#define	__INCLUDE_ATMEL_ATXMEGA_TCX0_H__

#if	( \
	defined(__GPT_DRIVER__)	|| \
	defined(__PWM_DRIVER__)	|| \
	defined(__ICU_DRIVER__)	\
)

#define	EE_TC_HW_ALL_INT_MASK		0xF3U

#define	EE_TC_HW_ALL_TCX0_MASK		0x03U

#define	EE_TC_HW_TCX0_BASE_MASK		0x01U
#define	EE_TC_HW_TCX0_2_MASK(_tcx0)	( EE_TC_HW_TCX0_BASE_MASK << (_tcx0) )

/*
 * Timer(s)/Counter(s) Hardware Units Base Addresses.
 */
#define	EE_TC_HW_TCC0_BASE_ADDR		0x0800U	/* TCC0 Base Address */
#define	EE_TC_HW_TCD0_BASE_ADDR		0x0900U	/* TCD0 Base Address */
#define	EE_TC_HW_TCE0_BASE_ADDR		0x0A00U	/* TCE0 Base Address */

#define	EE_TC_HW_TCX0_BASE_ADDR		0x0800U	/* TCx0 Base Address */

#define	EE_TC_HW_TCX0_OFS		0x0100U	/* TCx0 Offset. */
#define	EE_TC_HW_TCX0_OFS_SH_BITS	0x0008U	/* TCx0 Offset Shift Bits. */

#define	EE_TC_HW_TCX0_2_ADDR(_tcx0)	(				\
  EE_TC_HW_TCX0_BASE_ADDR + ( (_tcx0) << EE_TC_HW_TCX0_OFS_SH_BITS )	\
)

#define	EE_TC_HW_TCX0_CCX_BASE_OFS	0x28U

#define	EE_TC_HW_TCX0_CCX_OFS		0x02U
#define	EE_TC_HW_TCX0_CCX_OFS_SH_BITS	0x01U

#if	0
#define	TC_HW_TCX0_CCX_2_OFS(_ccx)	(				\
  EE_TC_HW_TCX0_CCX_BASE_OFS + ( (_ccx) >> 
#endif

/* TCx0 Buffered Register Offset */
#define	EE_TC_HW_TCX0_BUF_REG_OFS	0x10U

/*
 * Timer(s)/Counter(s) Hardware Units Registers Offsets.
 */
#define	EE_TC_HW_TCX0_CTRLA_REG_OFS		0x00U
#define	EE_TC_HW_TCX0_CTRLB_REG_OFS		0x01U
#define	EE_TC_HW_TCX0_CTRLC_REG_OFS		0x02U
#define	EE_TC_HW_TCX0_CTRLD_REG_OFS		0x03U
#define	EE_TC_HW_TCX0_CTRLE_REG_OFS		0x04U
#define	EE_TC_HW_TCX0_INTCTRLA_REG_OFS		0x06U
#define	EE_TC_HW_TCX0_INTCTRLB_REG_OFS		0x07U
#define	EE_TC_HW_TCX0_CTRLFCLR_REG_OFS		0x08U
#define	EE_TC_HW_TCX0_CTRLFSET_REG_OFS		0x09U
#define	EE_TC_HW_TCX0_CTRLGCLR_REG_OFS		0x0AU
#define	EE_TC_HW_TCX0_CTRLGSET_REG_OFS		0x0BU
#define	EE_TC_HW_TCX0_INTFLAGS_REG_OFS		0x0CU
#define	EE_TC_HW_TCX0_TEMP_REG_OFS		0x0FU
#define	EE_TC_HW_TCX0_CNT_REG_OFS		0x20U
#define	EE_TC_HW_TCX0_CNT_L_REG_OFS		0x20U
#define	EE_TC_HW_TCX0_CNT_H_REG_OFS		0x21U
#define	EE_TC_HW_TCX0_PER_REG_OFS		0x26U
#define	EE_TC_HW_TCX0_PER_L_REG_OFS		0x26U
#define	EE_TC_HW_TCX0_PER_H_REG_OFS		0x27U
#define	EE_TC_HW_TCX0_CCA_REG_OFS		0x28U
#define	EE_TC_HW_TCX0_CCA_L_REG_OFS		0x28U
#define	EE_TC_HW_TCX0_CCA_H_REG_OFS		0x29U
#define	EE_TC_HW_TCX0_CCB_REG_OFS		0x2AU
#define	EE_TC_HW_TCX0_CCB_L_REG_OFS		0x2AU
#define	EE_TC_HW_TCX0_CCB_H_REG_OFS		0x2BU
#define	EE_TC_HW_TCX0_CCC_REG_OFS		0x2CU
#define	EE_TC_HW_TCX0_CCC_L_REG_OFS		0x2CU
#define	EE_TC_HW_TCX0_CCC_H_REG_OFS		0x2DU
#define	EE_TC_HW_TCX0_CCD_REG_OFS		0x2EU
#define	EE_TC_HW_TCX0_CCD_L_REG_OFS		0x2EU
#define	EE_TC_HW_TCX0_CCD_H_REG_OFS		0x2FU
#define	EE_TC_HW_TCX0_PERBUF_REG_OFS		0x36U
#define	EE_TC_HW_TCX0_PERBUF_L_REG_OFS		0x36U
#define	EE_TC_HW_TCX0_PERBUF_H_REG_OFS		0x37U
#define	EE_TC_HW_TCX0_CCABUF_REG_OFS		0x38U
#define	EE_TC_HW_TCX0_CCABUF_L_REG_OFS		0x38U
#define	EE_TC_HW_TCX0_CCABUF_H_REG_OFS		0x39U
#define	EE_TC_HW_TCX0_CCBBUF_REG_OFS		0x3AU
#define	EE_TC_HW_TCX0_CCBBUF_L_REG_OFS		0x3AU
#define	EE_TC_HW_TCX0_CCBBUF_H_REG_OFS		0x3BU
#define	EE_TC_HW_TCX0_CCCBUF_REG_OFS		0x3CU
#define	EE_TC_HW_TCX0_CCCBUF_L_REG_OFS		0x3CU
#define	EE_TC_HW_TCX0_CCCBUF_H_REG_OFS		0x3DU
#define	EE_TC_HW_TCX0_CCDBUF_REG_OFS		0x3EU
#define	EE_TC_HW_TCX0_CCDBUF_L_REG_OFS		0x3EU
#define	EE_TC_HW_TCX0_CCDBUF_H_REG_OFS		0x3FU

/*
 * Timer(s)/Counter(s) Hardware Units Number.
 */
#define	EE_TC_HW_UNITS_NUMBER	0x03U

/*
#if	0
*/
#define	EE_TC_HW_CCA_ENABLE	0x10U
#define	EE_TC_HW_CCB_ENABLE	0x20U
#define	EE_TC_HW_CCC_ENABLE	0x40U
#define	EE_TC_HW_CCD_ENABLE	0x80U
#define	EE_TC_HW_CCX_EN_MASK	0xF0U
/*#else*/	/* 0 */
/*#define	EE_TC_HW_CCA_ENABLE	0x00U
#define	EE_TC_HW_CCB_ENABLE	0x10U
#define	EE_TC_HW_CCC_ENABLE	0x20U
#define	EE_TC_HW_CCD_ENABLE	0x30U
#define	EE_TC_HW_CCX_EN_MASK	0x30U
#endif*/	/* 0 */

#if	0
#define	
#endif

#define	EE_TC_HW_IRQ_OVF_PRI_MASK	0x03U
#define	EE_TC_HW_IRQ_OVF_PRI_SH_BITS	0x00U
#define	EE_TC_HW_IRQ_OVF_PRI(_pri)	(_pri)

#define	EE_TC_HW_IRQ_ERR_PRI_MASK	0x0CU
#define	EE_TC_HW_IRQ_ERR_PRI_SH_BITS	0x02U
#define	EE_TC_HW_IRQ_ERR_PRI(_pri)	\
	( (_pri) << EE_TC_HW_IRQ_ERR_PRI_SH_BITS )

#define	EE_TC_HW_IRQ_CCA_PRI_MASK	0x03U
#define	EE_TC_HW_IRQ_CCA_PRI_SH_BITS	0x00U
#define	EE_TC_HW_IRQ_CCA_PRI(_pri)	(_pri)

#define	EE_TC_HW_IRQ_CCB_PRI_MASK	0x0CU
#define	EE_TC_HW_IRQ_CCB_PRI_SH_BITS	0x02U
#define	EE_TC_HW_IRQ_CCB_PRI(_pri)	\
	( (_pri) << EE_TC_HW_IRQ_CCB_PRI_SH_BITS )

#define	EE_TC_HW_IRQ_CCC_PRI_MASK	0x30U
#define	EE_TC_HW_IRQ_CCC_PRI_SH_BITS	0x04U
#define	EE_TC_HW_IRQ_CCC_PRI(_pri)	\
	( (_pri) << EE_TC_HW_IRQ_CCC_PRI_SH_BITS )

#define	EE_TC_HW_IRQ_CCD_PRI_MASK	0xC0U
#define	EE_TC_HW_IRQ_CCD_PRI_SH_BITS	0x06U
#define	EE_TC_HW_IRQ_CCD_PRI(_pri)	\
	( (_pri) << EE_TC_HW_IRQ_CCD_PRI_SH_BITS )

/*
 * Initialization Mask.
 */
#define	EE_TC_HW_INIT_MASK		0x80U

/*
 * Timer(s)/Counter(s) Hardware Units Status Array.
 */
#ifdef	DEBUG
extern volatile EE_UINT8
#else
extern EE_UINT8
#endif
EE_Tc_Hw_Status[EE_TC_HW_UNITS_NUMBER];

#if	( \
  defined( EE_AVR8_TCC0_OVF_ISR ) || \
  defined( EE_AVR8_TCC0_CCA_ISR ) || defined( EE_AVR8_TCC0_CCB_ISR ) || \
  defined( EE_AVR8_TCC0_CCC_ISR ) || defined( EE_AVR8_TCC0_CCD_ISR ) || \
  defined( EE_AVR8_TCD0_OVF_ISR ) || \
  defined( EE_AVR8_TCD0_CCA_ISR ) || defined( EE_AVR8_TCD0_CCB_ISR ) || \
  defined( EE_AVR8_TCD0_CCC_ISR ) || defined( EE_AVR8_TCD0_CCD_ISR ) || \
  defined( EE_AVR8_TCE0_OVF_ISR ) || \
  defined( EE_AVR8_TCE0_CCA_ISR ) || defined( EE_AVR8_TCE0_CCB_ISR ) || \
  defined( EE_AVR8_TCE0_CCC_ISR ) || defined( EE_AVR8_TCE0_CCD_ISR ) \
)

/*
 * Timers(s)/Counters(s) Hardware Units Callback Array.
 */
#ifdef	DEBUG
extern volatile EE_ISR_callback
#else
extern EE_ISR_callback
#endif
EE_Tc_Hw_CallBack[EE_TC_HW_UNITS_NUMBER];

#endif	/*
	 * EE_AVR8_TCC0_OVF_ISR ||
	 * EE_AVR8_TCC0_CCA_ISR || EE_AVR8_TCC0_CCB_ISR ||
	 * EE_AVR8_TCC0_CCC_ISR || EE_AVR8_TCC0_CCD_ISR ||
	 * EE_AVR8_TCD0_OVF_ISR ||
	 * EE_AVR8_TCD0_CCA_ISR || EE_AVR8_TCD0_CCB_ISR ||
	 * EE_AVR8_TCD0_CCC_ISR || EE_AVR8_TCD0_CCD_ISR ||
	 * EE_AVR8_TCE0_OVF_ISR ||
	 * EE_AVR8_TCC0_CCA_ISR || EE_AVR8_TCC0_CCB_ISR ||
	 * EE_AVR8_TCC0_CCC_ISR || EE_AVR8_TCC0_CCD_ISR
	 */

#endif	/*
	 * (
	 * 	defined(__GPT_DRIVER__)	||
	 * 	defined(__PWM_DRIVER__)	||
	 * 	defined(__ICU_DRIVER__)
	 * )
	 */

#endif	/* __INCLUDE_ATMEL_ATXMEGA_TCX0_H__ */
