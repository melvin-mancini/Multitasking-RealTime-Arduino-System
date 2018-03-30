/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2008  Evidence Srl
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
 * Author: 2006 Antonio Romano
 * CVS: $Id: ee_external_int.S,v 1.2 2006/08/02 12:07:32 romano Exp $
 */



#include "mcu/atmel_atmega328/inc/ee_avr5def.S"

#define TEMP r18
.global initIntExt0
.global initIntExt1

.global disableExtInt0
.global disableExtInt1

.global clearPendingIRQ


.text
initIntExt0:	
		
		IN TEMP, DDRD
		ANDI TEMP, 0xFC
		OUT DDRD, TEMP
		IN TEMP, PORTD
		ORI TEMP, 0x01
		OUT PORTD, TEMP     ;  pullup attivati
		LDS TEMP, EICRA
		ORI TEMP, 0x02
		STS EICRA, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x01
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x01  ; setto nella maschera delle interruzioni i bit relativi a int0
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int0  
		out MCUCR,TEMP
		sei
		ret

initIntExt1:		
			
		IN TEMP, DDRD
		ANDI TEMP, 0xFD
		OUT DDRD, TEMP
		IN TEMP, PORTD
		ORI TEMP, 0x02
		OUT PORTD, TEMP     ;  pullup attivati
		LDS TEMP, EICRA
		ORI TEMP, 0x0C
		STS EICRA, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x02
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x02  ; setto nella maschera delle interruzioni i bit relativi a int1
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int1
		out MCUCR,TEMP
		sei
		ret

		
disableExtInt0:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xFE 
		OUT EIMSK, TEMP
		SEI
		RET
disableExtInt1:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xFD 
		OUT EIMSK, TEMP
		SEI
		RET


clearPendingIRQ:
		CLI
		SER TEMP
		OUT EIFR, TEMP
		SEI
		RET
		

