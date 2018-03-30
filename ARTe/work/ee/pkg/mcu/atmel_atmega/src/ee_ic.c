/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2014  Evidence Srl
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
 * Author: 2005 Antonio Romano
 * 	   2014 Giuseppe Serano
 */

#include "mcu/atmel_atmega/inc/ee_ic.h"

#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
#define	EE_ATMEGA_INT_EXT_SOURCES	0x08U
#else	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */
#define	EE_ATMEGA_INT_EXT_SOURCES	0x02U
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */

EE_FUN vectEnExt[EE_ATMEGA_INT_EXT_SOURCES]=  {
	EE_atmega_initIntExt0,
	EE_atmega_initIntExt1,
#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
	EE_atmega_initIntExt2,
	EE_atmega_initIntExt3,
	EE_atmega_initIntExt4,
	EE_atmega_initIntExt5,
	EE_atmega_initIntExt6,
	EE_atmega_initIntExt7
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */
};

EE_FUN  vectDisExt[EE_ATMEGA_INT_EXT_SOURCES]=  {
	EE_atmega_disableExtInt0,
	EE_atmega_disableExtInt1,
#if	( defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) )
	EE_atmega_disableExtInt2,
	EE_atmega_disableExtInt3,
	EE_atmega_disableExtInt4,
	EE_atmega_disableExtInt5,
	EE_atmega_disableExtInt6,
	EE_atmega_disableExtInt7
#endif	/* __AVR_ATmega128__ || __AVR_ATmega1281__ */
};

void EE_IC_enable_external_IRQ(EE_TYPEIRQMASK i)
{ 
	int index;
	for (index=0; index<EE_ATMEGA_INT_EXT_SOURCES; ++index) {
		if (i & (1<<index)) {
			(*vectEnExt[index])();
		}
	};
		
	
}

void EE_IC_disable_external_IRQ(EE_TYPEIRQMASK i)
{
	int index;
	for (index=0; index<EE_ATMEGA_INT_EXT_SOURCES; ++index) {
		if (i & (1<<index)) {
			(*vectDisExt[index])();
		}
	};
}

void EE_IC_compute_masks(void)
{
  
}

