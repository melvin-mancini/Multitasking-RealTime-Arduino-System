; ###*B*###
; ERIKA Enterprise - a tiny RTOS for small microcontrollers
;
; Copyright (C) 2002-2011  Evidence Srl
;
; This file is part of ERIKA Enterprise.
;
; ERIKA Enterprise is free software; you can redistribute it
; and/or modify it under the terms of the GNU General Public License
; version 2 as published by the Free Software Foundation, 
; (with a special exception described below).
;
; Linking this code statically or dynamically with other modules is
; making a combined work based on this code.  Thus, the terms and
; conditions of the GNU General Public License cover the whole
; combination.
;
; As a special exception, the copyright holders of this library give you
; permission to link this code with independent modules to produce an
; executable, regardless of the license terms of these independent
; modules, and to copy and distribute the resulting executable under
; terms of your choice, provided that you also meet, for each linked
; independent module, the terms and conditions of the license of that
; module.  An independent module is a module which is not derived from
; or based on this library.  If you modify this code, you may extend
; this exception to your version of the code, but you are not
; obligated to do so.  If you do not wish to do so, delete this
; exception statement from your version.
;
; ERIKA Enterprise is distributed in the hope that it will be
; useful, but WITHOUT ANY WARRANTY; without even the implied warranty
; of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License version 2 for more details.
;
; You should have received a copy of the GNU General Public License
; version 2 along with ERIKA Enterprise; if not, write to the
; Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
; Boston, MA 02110-1301 USA.
; ###*E*###

; /**
; 	@file ee_keil_irq_stack.s
; 	@brief Context switch function for multistack on Cortex_MX 
; 	@brief Stack switch for ISRs on Cortex_MX. 
; 	Implementation of EE_cortex_mx_call_ISR_new_stack() as described in
; 	pkg/cpu/cortex_mx/inc/ee_irq_internal.h
; 	@author Gianluca Franchino
; 	@author Giuseppe Serano
; 	@date 2011
; */ 

;*******************************************************************************
;                         PUBLIC FUNCTIONS
;*******************************************************************************
	EXPORT	EE_cortex_mx_change_IRQ_stack
	EXPORT	EE_cortex_mx_change_IRQ_stack_back

	IMPORT	EE_cortex_mx_IRQ_tos

;******************************************************************************
;
; Indicate that the code in this file preserves 8-byte alignment of the stack.
;
;******************************************************************************
	PRESERVE8

;*******************************************************************************
;                              DATA SECTION
;*******************************************************************************	
	AREA	|.data|,	READWRITE,	ALIGN=3
EE_cortex_mx_tmp_tos		DCD	0	; EE_UREG EE_cortex_mx_tmp_tos;

;*******************************************************************************
;                              CODE SECTION
;*******************************************************************************	
	AREA	|.text|,	CODE,	READONLY
	THUMB

;void EE_cortex_mx_change_IRQ_stack(void);
EE_cortex_mx_change_IRQ_stack
	MRS	R0, MSP			; R0 = MSP (Main stack Pointer)
	LDR	R1, =EE_cortex_mx_tmp_tos	; R1 = address of EE_cortex_m0_tmp_tos
	STR	R0, [R1]		; Save MSP in EE_cortex_mx_tmp_tos
	LDR	R0, =EE_cortex_mx_IRQ_tos; R0 = address of EE_cortex_mx_IRQ_tos
	LDR	R0, [R0]		; R0 = IRQ new stack pointer
	MSR	MSP, R0			; change IRQ stack

	BX LR 				; return

;void EE_cortex_mx_change_IRQ_stack_back(void);
EE_cortex_mx_change_IRQ_stack_back
	LDR	R0, =EE_cortex_mx_tmp_tos	; R0 = address of EE_cortex_mx_tmp_tos
	LDR	R0, [R0]		; R0 = old MSP
	MSR	MSP, R0			; Restore the stack pointer

	BX LR				; return 

;******************************************************************************
;
; Make sure the end of this section is aligned.
;
;******************************************************************************
	ALIGN

;******************************************************************************
;
; Tell the assembler that we're done.
;
;******************************************************************************
	END

