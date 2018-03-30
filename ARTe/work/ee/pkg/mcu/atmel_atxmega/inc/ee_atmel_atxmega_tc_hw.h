/** @file	ee_atmel_atxmega_tc_hw.h
 *
 *  @brief	Erika Enterprise - Timer Counter Hardware Abstraction -
 *  		Atmel ATXMEGA MCUs Header File.
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
 *  Timer Counter Hardware Abstraction of Atmel ATXMEGA MCUs.
 *
 *  @author	Giuseppe Serano
 *  @version	0.1
 *  @date	2013
 */

#ifndef	__INCLUDE_ATMEL_ATXMEGA_TC_HW_H__
#define	__INCLUDE_ATMEL_ATXMEGA_TC_HW_H__

#if	( \
	defined(__GPT_DRIVER__)	|| \
	defined(__PWM_DRIVER__)	|| \
	defined(__ICU_DRIVER__)	\
)

#define	EE_TC_HW_ALL_INT_MASK		0xF3U

#define	EE_TC_HW_ALL_TCX0_MASK		0x03U
#define	EE_TC_HW_ALL_TCX1_MASK		0x04U
#define	EE_TC_HW_ALL_UNITS_MASK		\
	( EE_TC_HW_ALL_TCX0_MASK | EE_TC_HW_ALL_TCX1_MASK )

#define	EE_TC_HW_BASE_MASK		0x01U
#define	EE_TC_HW_2_MASK(_tcx0)	( EE_TC_HW_BASE_MASK << (_tcx0) )

/*
 * Timer(s)/Counter(s) Hardware Units Base Addresses.
 */
#define	EE_TC_HW_TCC0_BASE_ADDR		0x0800U	/* TCC0 Base Address */
#define	EE_TC_HW_TCC1_BASE_ADDR		0x0840U	/* TCC1 Base Address */
#define	EE_TC_HW_TCD0_BASE_ADDR		0x0900U	/* TCD0 Base Address */
#define	EE_TC_HW_TCD1_BASE_ADDR		0x0940U	/* TCC1 Base Address */
#define	EE_TC_HW_TCE0_BASE_ADDR		0x0A00U	/* TCE0 Base Address */
#define	EE_TC_HW_TCE1_BASE_ADDR		0x0A40U	/* TCC1 Base Address */
#define	EE_TC_HW_TCF0_BASE_ADDR		0x0B00U	/* TCE0 Base Address */
#define	EE_TC_HW_TCF1_BASE_ADDR		0x0B40U	/* TCC1 Base Address */

#define	EE_TC_HW_TCX0_BASE_ADDR		0x0800U	/* TCx0 Base Address */

#define	EE_TC_HW_TCX0_OFS		0x0100U	/* TCx0 Offset. */
#define	EE_TC_HW_TCX0_OFS_SH_BITS	0x0008U	/* TCx0 Offset Shift Bits. */

#define	EE_TC_HW_TCX0_2_ADDR(_tcx0)	(				\
  EE_TC_HW_TCX0_BASE_ADDR + (						\
  	(_tcx0 & EE_TC_HW_ALL_TCX0_MASK) << EE_TC_HW_TCX0_OFS_SH_BITS	\
  )									\
)

#define	EE_TC_HW_TCX1_OFS		0x0040U	/* TCx1 Offset. */
#define	EE_TC_HW_TCX1_OFS_SH_BITS	0x0004U	/* TCx1 Offset Shift Bits. */

#define	EE_TC_HW_TCX01_2_ADDR(_tcx01)	(				\
  EE_TC_HW_TCX0_2_ADDR(_tcx01) + (					\
  	(_tcx01 & EE_TC_HW_ALL_TCX1_MASK) << EE_TC_HW_TCX1_OFS_SH_BITS	\
  )									\
)

#if	( \
  defined(__GPT_CHANNEL_C_1__) || defined(__PWM_CHANNEL_C_1__) || \
  defined(__GPT_CHANNEL_D_1__) || defined(__PWM_CHANNEL_D_1__) || \
  defined(__GPT_CHANNEL_E_1__) || defined(__PWM_CHANNEL_E_1__) || \
  defined(__GPT_CHANNEL_F_1__) || defined(__PWM_CHANNEL_F_1__) \
)

#define	EE_TC_HW_2_ADDR(_tcx01)	EE_TC_HW_TCX01_2_ADDR(_tcx01)

#else	/*
	 * __GPT_CHANNEL_C_1__ || __PWM_CHANNEL_C_1__ ||
	 * __GPT_CHANNEL_D_1__ || __PWM_CHANNEL_D_1__ ||
	 * __GPT_CHANNEL_E_1__ || __PWM_CHANNEL_E_1__ ||
	 * __GPT_CHANNEL_F_1__ || __PWM_CHANNEL_F_1__
	 */

#define	EE_TC_HW_2_ADDR(_tcx0)	EE_TC_HW_TCX0_2_ADDR(_tcx0)

#endif	/*
	 * __GPT_CHANNEL_C_1__ || __PWM_CHANNEL_C_1__ ||
	 * __GPT_CHANNEL_D_1__ || __PWM_CHANNEL_D_1__ ||
	 * __GPT_CHANNEL_E_1__ || __PWM_CHANNEL_E_1__ ||
	 * __GPT_CHANNEL_F_1__ || __PWM_CHANNEL_F_1__
	 */

/*
 * Timer(s)/Counter(s) Hardware Units Registers Offsets.
 */
#define	EE_TC_HW_CTRLA_REG_OFS		0x00U
#define	EE_TC_HW_CTRLB_REG_OFS		0x01U
#define	EE_TC_HW_CTRLC_REG_OFS		0x02U
#define	EE_TC_HW_CTRLD_REG_OFS		0x03U
#define	EE_TC_HW_CTRLE_REG_OFS		0x04U
#define	EE_TC_HW_INTCTRLA_REG_OFS	0x06U
#define	EE_TC_HW_INTCTRLB_REG_OFS	0x07U
#define	EE_TC_HW_CTRLFCLR_REG_OFS	0x08U
#define	EE_TC_HW_CTRLFSET_REG_OFS	0x09U
#define	EE_TC_HW_CTRLGCLR_REG_OFS	0x0AU
#define	EE_TC_HW_CTRLGSET_REG_OFS	0x0BU
#define	EE_TC_HW_INTFLAGS_REG_OFS	0x0CU
#define	EE_TC_HW_TEMP_REG_OFS		0x0FU
#define	EE_TC_HW_CNT_REG_OFS		0x20U
#define	EE_TC_HW_CNT_L_REG_OFS		0x20U
#define	EE_TC_HW_CNT_H_REG_OFS		0x21U
#define	EE_TC_HW_PER_REG_OFS		0x26U
#define	EE_TC_HW_PER_L_REG_OFS		0x26U
#define	EE_TC_HW_PER_H_REG_OFS		0x27U
#define	EE_TC_HW_CCA_REG_OFS		0x28U
#define	EE_TC_HW_CCA_L_REG_OFS		0x28U
#define	EE_TC_HW_CCA_H_REG_OFS		0x29U
#define	EE_TC_HW_CCB_REG_OFS		0x2AU
#define	EE_TC_HW_CCB_L_REG_OFS		0x2AU
#define	EE_TC_HW_CCB_H_REG_OFS		0x2BU
#define	EE_TC_HW_CCC_REG_OFS		0x2CU
#define	EE_TC_HW_CCC_L_REG_OFS		0x2CU
#define	EE_TC_HW_CCC_H_REG_OFS		0x2DU
#define	EE_TC_HW_CCD_REG_OFS		0x2EU
#define	EE_TC_HW_CCD_L_REG_OFS		0x2EU
#define	EE_TC_HW_CCD_H_REG_OFS		0x2FU
#define	EE_TC_HW_PERBUF_REG_OFS		0x36U
#define	EE_TC_HW_PERBUF_L_REG_OFS	0x36U
#define	EE_TC_HW_PERBUF_H_REG_OFS	0x37U
#define	EE_TC_HW_CCABUF_REG_OFS		0x38U
#define	EE_TC_HW_CCABUF_L_REG_OFS	0x38U
#define	EE_TC_HW_CCABUF_H_REG_OFS	0x39U
#define	EE_TC_HW_CCBBUF_REG_OFS		0x3AU
#define	EE_TC_HW_CCBBUF_L_REG_OFS	0x3AU
#define	EE_TC_HW_CCBBUF_H_REG_OFS	0x3BU
#define	EE_TC_HW_CCCBUF_REG_OFS		0x3CU
#define	EE_TC_HW_CCCBUF_L_REG_OFS	0x3CU
#define	EE_TC_HW_CCCBUF_H_REG_OFS	0x3DU
#define	EE_TC_HW_CCDBUF_REG_OFS		0x3EU
#define	EE_TC_HW_CCDBUF_L_REG_OFS	0x3EU
#define	EE_TC_HW_CCDBUF_H_REG_OFS	0x3FU

/*
 * Timer(s)/Counter(s) Hardware Units Number.
 */
#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )
#define	EE_TC_0_HW_UNITS_NUMBER	0x03U
#define	EE_TC_1_HW_UNITS_NUMBER	0x01U
#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */
#define	EE_TC_0_HW_UNITS_NUMBER	0x04U
#define	EE_TC_1_HW_UNITS_NUMBER	0x04U
#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

#define	EE_TC_HW_CCA_ENABLE	0x10U
#define	EE_TC_HW_CCB_ENABLE	0x20U
#define	EE_TC_HW_CCC_ENABLE	0x40U
#define	EE_TC_HW_CCD_ENABLE	0x80U
#define	EE_TC_HW_CCX_EN_MASK	0xF0U

#define	EE_TC_HW_IRQ_OVF_PRI_MASK	0x03U
#define	EE_TC_HW_IRQ_OVF_PRI_SH_BITS	0x00U
#define	EE_TC_HW_IRQ_OVF_PRI(_pri)	(_pri)

#define	EE_TC_HW_IRQ_ERR_PRI_MASK	0x0CU
#define	EE_TC_HW_IRQ_ERR_PRI_SH_BITS	0x02U
#define	EE_TC_HW_IRQ_ERR_PRI(_pri)	\
	( (_pri) << EE_TC_HW_IRQ_ERR_PRI_SH_BITS )

#define	EE_TC_HW_IRQ_CCA_PRI_MASK	0x03U
#define	EE_TC_HW_IRQ_CCA_PRI_SH_BITS	0x00U
#define	EE_TC_HW_IRQ_CCA_PRI(_pri)	\
	( (_pri) << EE_TC_HW_IRQ_CCA_PRI_SH_BITS )

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

/** @brief	Timer/Counter Status Retrieval.
 *  @param[in]	Id	Numeric identifier of the Timer/Counter Hardware Unit.
 *  @return	Status of the Timer/Counter Hardware Unit.
 *
 *  The service gets the status of the specified Timer/Counter Hardware Unit.
 *
 *  The function <tt>EE_Tc_Hw_GetStatus()</tt> shall return the current
 *  operational status of the Timer/Counter Hardware Unit.
 */
EE_UINT8
EE_Tc_Hw_GetStatus(
  EE_UREG	Id
);

/** @brief	Timer/Counter Status Set-Up.
 *  @param[in]	Id	Numeric identifier of the Timer/Counter Hardware Unit.
 *  @param[in]	Status	Status of the Timer/Counter Hardware Unit.
 *  @return	Nothing.
 *
 *  The service set the status of the specified Timer/Counter Hardware Unit.
 *
 *  The function <tt>EE_Tc_Hw_SetStatus()</tt> shall set the operational status
 *  of the Timer/Counter Hardware Unit.
 */
void
EE_Tc_Hw_SetStatus(
  EE_UREG	Id,
  EE_UINT8	Status
);

/** @brief	Timer/Counter Callback Retrieval.
 *  @param[in]	Id	Numeric identifier of the Timer/Counter Hardware Unit.
 *  @return	CbPtr	Callback Pointer of the Timer/Counter Hardware Unit.
 *
 *  The service gets the Callback of the specified Timer/Counter Hardware Unit.
 *
 *  The function <tt>EE_Tc_Hw_GetCallback()</tt> shall return the current
 *  operational Callback of the Timer/Counter Hardware Unit.
 */
EE_ISR_callback
EE_Tc_Hw_GetCallback(
  EE_UREG	Id
);

/** @brief	Timer/Counter Callback Set-Up.
 *  @param[in]	Id	Numeric identifier of the Timer/Counter Hardware Unit.
 *  @param[in]	CbPtr	Callback Pointer of the Timer/Counter Hardware Unit.
 *  @return	Nothing.
 *
 *  The service set the Callback of the specified Timer/Counter Hardware Unit.
 *
 *  The function <tt>EE_Tc_Hw_SetCallback()</tt> shall set the operational
 *  Callback of the Timer/Counter Hardware Unit.
 */
void
EE_Tc_Hw_SetCallback(
  EE_UREG		Id,
  EE_ISR_callback	CbPtr
);

#endif	/*
	 * (
	 * 	defined(__GPT_DRIVER__)	||
	 * 	defined(__PWM_DRIVER__)	||
	 * 	defined(__ICU_DRIVER__)
	 * )
	 */

#endif	/* __INCLUDE_ATMEL_ATXMEGA_TC_HW_H__ */
