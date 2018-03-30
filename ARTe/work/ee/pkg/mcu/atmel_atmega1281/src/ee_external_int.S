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
 * CVS: $Id: ee_external_int.S,v 1.1 2007/10/04 14:39:08 romano Exp $
 */



#include "mcu/atmel_atmega1281/inc/ee_avr5def.S"

#define TEMP r18
.global initIntExt0
.global initIntExt1
.global initIntExt2
.global initIntExt3
.global initIntExt4
.global initIntExt5
.global initIntExt6
.global initIntExt7
.global disableExtInt0
.global disableExtInt1
.global disableExtInt2
.global disableExtInt3
.global disableExtInt4
.global disableExtInt5
.global disableExtInt6
.global disableExtInt7
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

initIntExt2:
					
		IN TEMP, DDRD
		ANDI TEMP, 0xFB
		OUT DDRD, TEMP
		IN TEMP, PORTD
		ORI TEMP, 0x04
		OUT PORTD, TEMP     ;  pullup attivati
		LDS TEMP, EICRA
		ORI TEMP, 0x30
		STS EICRA, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x04
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x04  ; setto nella maschera delle interruzioni i bit relativi a int2
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int2
		out MCUCR,TEMP
		sei
		ret
		
initIntExt3:			
		
		IN TEMP, DDRD
		ANDI TEMP, 0xF7
		OUT DDRD, TEMP
		IN TEMP, PORTD
		ORI TEMP, 0x08
		OUT PORTD, TEMP     ;  pullup attivati
		LDS TEMP, EICRA
		ORI TEMP, 0xC0
		STS EICRA, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x08
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x08  ; setto nella maschera delle interruzioni i bit relativi a int3
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int3
		out MCUCR,TEMP
		sei
		ret
		
initIntExt4:	
		
		IN TEMP, DDRE
		ANDI TEMP, 0xEF
		OUT DDRE, TEMP
		IN TEMP, PORTE
		ORI TEMP, 0x10
		OUT PORTE, TEMP     ;  pullup attivati
		in TEMP, EICRB
		ORI TEMP, 0x03
		out EICRB, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x10
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x10  ; setto nella maschera delle interruzioni i bit relativi a int4
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int4  
		out MCUCR,TEMP
		sei
		ret
		
initIntExt5:	
		
		IN TEMP, DDRE
		ANDI TEMP, 0xDF
		OUT DDRE, TEMP
		IN TEMP, PORTE
		ORI TEMP, 0x20
		OUT PORTE, TEMP     ;  pullup attivati
		in TEMP, EICRB
		ORI TEMP, 0x0C
		out EICRB, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x20
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x20  ; setto nella maschera delle interruzioni i bit relativi a int5
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int5  
		out MCUCR,TEMP
		sei
		ret
initIntExt6:	
		
		IN TEMP, DDRE
		ANDI TEMP, 0xBF
		OUT DDRE, TEMP
		IN TEMP, PORTE
		ORI TEMP, 0x40
		OUT PORTE, TEMP     ;  pullup attivati
		in TEMP, EICRB
		ORI TEMP, 0x30
		out EICRB, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x40
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x40  ; setto nella maschera delle interruzioni i bit relativi a int6
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int6  
		out MCUCR,TEMP
		sei
		ret

initIntExt7:	
		
		IN TEMP, DDRE
		ANDI TEMP, 0x8F
		OUT DDRE, TEMP
		IN TEMP, PORTE
		ORI TEMP, 0x80
		OUT PORTE, TEMP     ;  pullup attivati
		in TEMP, EICRB
		ORI TEMP, 0xC0
		out EICRB, TEMP
		IN TEMP, EIFR
		ORI TEMP, 0x80
		sts EIFR, TEMP
		IN TEMP, EIMSK
		ORI TEMP, 0x80  ; setto nella maschera delle interruzioni i bit relativi a int7
		out EIMSK,TEMP
		ldi TEMP,0x0F 		; interrupt int7  
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
disableExtInt2:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xFB 
		OUT EIMSK, TEMP
		SEI
		RET
disableExtInt3:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xF7 
		OUT EIMSK, TEMP
		SEI
		RET
		
disableExtInt4:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xEF 
		OUT EIMSK, TEMP
		SEI
		RET
disableExtInt5:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xDF 
		OUT EIMSK, TEMP
		SEI
		RET
		
disableExtInt6:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xBF 
		OUT EIMSK, TEMP
		SEI
		RET
		
disableExtInt7:
		CLI
		IN TEMP, EIMSK
		ANDI TEMP, 0xF7 
		OUT EIMSK, TEMP
		SEI
		RET

clearPendingIRQ:
		CLI
		SER TEMP
		OUT EIFR, TEMP
		SEI
		RET
		

