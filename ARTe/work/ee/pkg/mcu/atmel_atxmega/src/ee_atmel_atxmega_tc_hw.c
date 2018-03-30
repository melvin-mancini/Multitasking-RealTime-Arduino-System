/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

/*
 * Erika Enterprise - Timer Counter Hardware Abstraction -
 * Atmel ATXMEGA MCUs Implementation File.
 *
 * Author: 2013,  Giuseppe Serano
 */

/* ERIKA Enterprise Internals */
#include "ee_internal.h"

/*
 * Erika Enterprise - Timer Counter Hardware Abstraction -
 * Atmel ATXMEGA MCUs Header File.
 */
#include "mcu/atmel_atxmega/inc/ee_atmel_atxmega_tc_hw.h"

/*
 * Timer(s)/Counter(s) Type 0 Hardware Units Status Array.
 */
#ifdef	DEBUG
volatile EE_UINT8
#else
EE_UINT8
#endif
EE_Tc_0_Hw_Status[EE_TC_0_HW_UNITS_NUMBER] = {

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* TCC0 */
#ifdef	EE_AVR8_TCC0_OVF_ISR_PRI
EE_AVR8_TCC0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCD0 */
#ifdef	EE_AVR8_TCD0_OVF_ISR_PRI
EE_AVR8_TCD0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCE0 */
#ifdef	EE_AVR8_TCE0_OVF_ISR_PRI
EE_AVR8_TCE0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* TCC0 */
#ifdef	EE_AVR8_TCC0_OVF_ISR_PRI
EE_AVR8_TCC0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCD0 */
#ifdef	EE_AVR8_TCD0_OVF_ISR_PRI
EE_AVR8_TCD0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCE0 */
#ifdef	EE_AVR8_TCE0_OVF_ISR_PRI
EE_AVR8_TCE0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCF0 */
#ifdef	EE_AVR8_TCF0_OVF_ISR_PRI
EE_AVR8_TCF0_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Tc_0_Hw_Status[] */

/*
 * Timer(s)/Counter(s) Type 1 Hardware Units Status Array.
 */
#ifdef	DEBUG
volatile EE_UINT8
#else
EE_UINT8
#endif
EE_Tc_1_Hw_Status[EE_TC_1_HW_UNITS_NUMBER] = {

#if	( defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32D4__) )

/* TCC1 */
#ifdef	EE_AVR8_TCC1_OVF_ISR_PRI
EE_AVR8_TCC1_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

#else	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

/* TCC1 */
#ifdef	EE_AVR8_TCC1_OVF_ISR_PRI
EE_AVR8_TCC1_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCD1 */
#ifdef	EE_AVR8_TCD1_OVF_ISR_PRI
EE_AVR8_TCD1_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCE1 */
#ifdef	EE_AVR8_TCE1_OVF_ISR_PRI
EE_AVR8_TCE1_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

/* TCF1 */
#ifdef	EE_AVR8_TCF1_OVF_ISR_PRI
EE_AVR8_TCF1_OVF_ISR_PRI,
#else
EE_ISR_PRI_OFF,
#endif

#endif	/* __AVR_ATxmega16D4__ || __AVR_ATxmega32D4__ */

};	/* EE_Tc_1_Hw_Status[] */

/*
 * Timer/Counter Hardware Unit Status Retrieval.
 */
EE_UINT8
EE_Tc_Hw_GetStatus(
  EE_UREG	Id
) {

#if	DEBUG
  volatile EE_UINT8	Status;
#else	/* DEBUG */
  register EE_UINT8	Status;
#endif	/* DEBUG */

  if ( Id & EE_TC_HW_ALL_TCX1_MASK ) {

    Status = EE_Tc_1_Hw_Status[(Id & ~EE_TC_HW_ALL_TCX1_MASK)];

  }
  else {

    Status = EE_Tc_0_Hw_Status[Id];

  }

  return Status;

}

/*
 * Timer/Counter Hardware Unit Status Set-Up.
 */
void
EE_Tc_Hw_SetStatus(
  EE_UREG	Id,
  EE_UINT8	Status
) {

  if ( Id & EE_TC_HW_ALL_TCX1_MASK ) {

    EE_Tc_1_Hw_Status[(Id & ~EE_TC_HW_ALL_TCX1_MASK)] = Status;

  }
  else {

    EE_Tc_0_Hw_Status[Id] = Status;

  }

}

#if	( \
  defined( EE_AVR8_TCC0_OVF_ISR ) || \
  defined( EE_AVR8_TCC0_CCA_ISR ) || defined( EE_AVR8_TCC0_CCB_ISR ) || \
  defined( EE_AVR8_TCC0_CCC_ISR ) || defined( EE_AVR8_TCC0_CCD_ISR ) || \
  defined( EE_AVR8_TCD0_OVF_ISR ) || \
  defined( EE_AVR8_TCD0_CCA_ISR ) || defined( EE_AVR8_TCD0_CCB_ISR ) || \
  defined( EE_AVR8_TCD0_CCC_ISR ) || defined( EE_AVR8_TCD0_CCD_ISR ) || \
  defined( EE_AVR8_TCE0_OVF_ISR ) || \
  defined( EE_AVR8_TCE0_CCA_ISR ) || defined( EE_AVR8_TCE0_CCB_ISR ) || \
  defined( EE_AVR8_TCE0_CCC_ISR ) || defined( EE_AVR8_TCE0_CCD_ISR ) || \
  defined( EE_AVR8_TCF0_OVF_ISR ) || \
  defined( EE_AVR8_TCF0_CCA_ISR ) || defined( EE_AVR8_TCF0_CCB_ISR ) || \
  defined( EE_AVR8_TCF0_CCC_ISR ) || defined( EE_AVR8_TCF0_CCD_ISR ) \
)

/*
 * Timers(s)/Counters(s) Type 0 Hardware Units Callback Array.
 */
#ifdef	DEBUG
volatile EE_ISR_callback
#else
EE_ISR_callback
#endif
#if	0
EE_Tc_0_Hw_CallBack[EE_TC_0_HW_UNITS_NUMBER] = { EE_NULL_PTR };
#else	/* 0 */
EE_Tc_0_Hw_CallBack[EE_TC_0_HW_UNITS_NUMBER];
#endif	/* 0 */

#endif	/*
	 * EE_AVR8_TCC0_OVF_ISR ||
	 * EE_AVR8_TCC0_CCA_ISR || EE_AVR8_TCC0_CCB_ISR ||
	 * EE_AVR8_TCC0_CCC_ISR || EE_AVR8_TCC0_CCD_ISR ||
	 * EE_AVR8_TCD0_OVF_ISR ||
	 * EE_AVR8_TCD0_CCA_ISR || EE_AVR8_TCD0_CCB_ISR ||
	 * EE_AVR8_TCD0_CCC_ISR || EE_AVR8_TCD0_CCD_ISR ||
	 * EE_AVR8_TCE0_OVF_ISR ||
	 * EE_AVR8_TCE0_CCA_ISR || EE_AVR8_TCE0_CCB_ISR ||
	 * EE_AVR8_TCE0_CCC_ISR || EE_AVR8_TCE0_CCD_ISR ||
	 * EE_AVR8_TCF0_OVF_ISR ||
	 * EE_AVR8_TCF0_CCA_ISR || EE_AVR8_TCF0_CCB_ISR ||
	 * EE_AVR8_TCF0_CCC_ISR || EE_AVR8_TCF0_CCD_ISR
	 */

#if	( \
  defined( EE_AVR8_TCC1_OVF_ISR ) || \
  defined( EE_AVR8_TCC1_CCA_ISR ) || defined( EE_AVR8_TCC1_CCB_ISR ) || \
  defined( EE_AVR8_TCD1_OVF_ISR ) || \
  defined( EE_AVR8_TCD1_CCA_ISR ) || defined( EE_AVR8_TCD1_CCB_ISR ) || \
  defined( EE_AVR8_TCE1_OVF_ISR ) || \
  defined( EE_AVR8_TCE1_CCA_ISR ) || defined( EE_AVR8_TCE1_CCB_ISR ) || \
  defined( EE_AVR8_TCF1_OVF_ISR ) || \
  defined( EE_AVR8_TCF1_CCA_ISR ) || defined( EE_AVR8_TCF1_CCB_ISR ) \
)

/*
 * Timers(s)/Counters(s) Type 1 Hardware Units Callback Array.
 */
#ifdef	DEBUG
volatile EE_ISR_callback
#else
EE_ISR_callback
#endif
#if	0
EE_Tc_1_Hw_CallBack[EE_TC_1_HW_UNITS_NUMBER] = { EE_NULL_PTR };
#else	/* 0 */
EE_Tc_1_Hw_CallBack[EE_TC_0_HW_UNITS_NUMBER];
#endif	/* 0 */

#endif	/*
	 * EE_AVR8_TCC1_OVF_ISR ||
	 * EE_AVR8_TCC1_CCA_ISR || EE_AVR8_TCC1_CCB_ISR ||
	 * EE_AVR8_TCD1_OVF_ISR ||
	 * EE_AVR8_TCD1_CCA_ISR || EE_AVR8_TCD1_CCB_ISR ||
	 * EE_AVR8_TCE1_OVF_ISR ||
	 * EE_AVR8_TCE1_CCA_ISR || EE_AVR8_TCE1_CCB_ISR ||
	 * EE_AVR8_TCF1_OVF_ISR ||
	 * EE_AVR8_TCF1_CCA_ISR || EE_AVR8_TCF1_CCB_ISR
	 */

/*
 * Timer/Counter Hardware Unit Callback Retrieval.
 */
EE_ISR_callback
EE_Tc_Hw_GetCallback(
  EE_UREG	Id
) {

#if	DEBUG
  volatile EE_ISR_callback	CbPtr;
#else	/* DEBUG */
  register EE_ISR_callback	CbPtr;
#endif	/* DEBUG */

  if ( Id & EE_TC_HW_ALL_TCX1_MASK ) {

#if	( \
  defined( EE_AVR8_TCC1_OVF_ISR ) || \
  defined( EE_AVR8_TCC1_CCA_ISR ) || defined( EE_AVR8_TCC1_CCB_ISR ) || \
  defined( EE_AVR8_TCD1_OVF_ISR ) || \
  defined( EE_AVR8_TCD1_CCA_ISR ) || defined( EE_AVR8_TCD1_CCB_ISR ) || \
  defined( EE_AVR8_TCE1_OVF_ISR ) || \
  defined( EE_AVR8_TCE1_CCA_ISR ) || defined( EE_AVR8_TCE1_CCB_ISR ) || \
  defined( EE_AVR8_TCF1_OVF_ISR ) || \
  defined( EE_AVR8_TCF1_CCA_ISR ) || defined( EE_AVR8_TCF1_CCB_ISR ) \
)

    CbPtr = EE_Tc_1_Hw_CallBack[(Id & ~EE_TC_HW_ALL_TCX1_MASK)];

#else

    CbPtr = EE_NULL_PTR;

#endif	/*
	 * EE_AVR8_TCC1_OVF_ISR ||
	 * EE_AVR8_TCC1_CCA_ISR || EE_AVR8_TCC1_CCB_ISR ||
	 * EE_AVR8_TCD1_OVF_ISR ||
	 * EE_AVR8_TCD1_CCA_ISR || EE_AVR8_TCD1_CCB_ISR ||
	 * EE_AVR8_TCE1_OVF_ISR ||
	 * EE_AVR8_TCE1_CCA_ISR || EE_AVR8_TCE1_CCB_ISR ||
	 * EE_AVR8_TCF1_OVF_ISR ||
	 * EE_AVR8_TCF1_CCA_ISR || EE_AVR8_TCF1_CCB_ISR
	 */

  }
  else {

    CbPtr = EE_Tc_0_Hw_CallBack[Id];

  }

  return CbPtr;

}

/*
 * Timer/Counter Hardware Unit Callback Setup.
 */
void
EE_Tc_Hw_SetCallback(
  EE_UREG		Id,
  EE_ISR_callback	CbPtr
) {

  if ( Id & EE_TC_HW_ALL_TCX1_MASK ) {

#if	( \
  defined( EE_AVR8_TCC1_OVF_ISR ) || \
  defined( EE_AVR8_TCC1_CCA_ISR ) || defined( EE_AVR8_TCC1_CCB_ISR ) || \
  defined( EE_AVR8_TCD1_OVF_ISR ) || \
  defined( EE_AVR8_TCD1_CCA_ISR ) || defined( EE_AVR8_TCD1_CCB_ISR ) || \
  defined( EE_AVR8_TCE1_OVF_ISR ) || \
  defined( EE_AVR8_TCE1_CCA_ISR ) || defined( EE_AVR8_TCE1_CCB_ISR ) || \
  defined( EE_AVR8_TCF1_OVF_ISR ) || \
  defined( EE_AVR8_TCF1_CCA_ISR ) || defined( EE_AVR8_TCF1_CCB_ISR ) \
)

    EE_Tc_1_Hw_CallBack[(Id & ~EE_TC_HW_ALL_TCX1_MASK)] = CbPtr;

#else

    (void)EE_OFF;

#endif	/*
	 * EE_AVR8_TCC1_OVF_ISR ||
	 * EE_AVR8_TCC1_CCA_ISR || EE_AVR8_TCC1_CCB_ISR ||
	 * EE_AVR8_TCD1_OVF_ISR ||
	 * EE_AVR8_TCD1_CCA_ISR || EE_AVR8_TCD1_CCB_ISR ||
	 * EE_AVR8_TCE1_OVF_ISR ||
	 * EE_AVR8_TCE1_CCA_ISR || EE_AVR8_TCE1_CCB_ISR ||
	 * EE_AVR8_TCF1_OVF_ISR ||
	 * EE_AVR8_TCF1_CCA_ISR || EE_AVR8_TCF1_CCB_ISR
	 */

  }
  else {

    EE_Tc_0_Hw_CallBack[Id] = CbPtr;

  }

}
