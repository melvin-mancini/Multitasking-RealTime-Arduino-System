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
; 	@file ee_keil_oo.S
; 	@brief Functions to save and restore registers for OSEK TerminateTask().
; 	@author Gianluca Franchino
; 	@author Giuseppe Serano
; 	@date 2011
; */

;*******************************************************************************
;                         PUBLIC FUNCTIONS
;*******************************************************************************
	; Functions declared in this file 
	EXPORT	EE_hal_terminate_savestk	; void EE_hal_terminate_savestk(EE_TID tid)
	EXPORT	EE_hal_terminate_task		; NORETURN void EE_hal_terminate_task(EE_TID tid);

	IMPORT	EE_terminate_real_th_body
	IMPORT	EE_terminate_data
	IMPORT	EE_thread_not_terminated

;*******************************************************************************
;                              EQUATES
;*******************************************************************************
EPSR_T_BIT_VAL		EQU	0x01000000	; Value to set the T-bit in EPSR (always Thumb mode)

;******************************************************************************
;
; Indicate that the code in this file preserves 8-byte alignment of the stack.
;
;******************************************************************************
	PRESERVE8

;*******************************************************************************
;                              CODE SECTION
;*******************************************************************************	
	AREA	|.text|,	CODE,	READONLY
	THUMB

;void EE_hal_terminate_savestk(EE_TID tid);
EE_hal_terminate_savestk	FUNCTION
	; Save all callee-saved registers
	; R0-R3 and R12 are scratch registers, R13 ->(MSP), R14 ->(LR), R15 -> (PC)
	PUSH	{R4-R7}			; Store R4, R5, R6, R7 onto stack 
	MOV	R4, R8
	MOV	R5, R9
	MOV	R6, R10
	MOV	R7, R11
	PUSH	{R4-R7}			; Store R8, R9, R10, R11 onto stack
	PUSH	{LR}			; Store link register (return address)
	MRS	R3, PSR			; Store xPSR to 8-bytes stack aligment
	PUSH	{R3}
	;R0 == tid
	LSLS	R0, R0, #2		; R0 = tid << 2
	LDR	R1, =EE_terminate_real_th_body	; R1 == EE_terminate_real_th_body[tid]
	ADD	R1, R1, R0
	LDR	R1, [R1]
	; Save the stack pointer (including space for registers)
	; R2 == & EE_terminate_data[tid]
	LDR	R2, =EE_terminate_data
	ADD	R2, R2, R0
	MRS	R3, MSP			; Get the stack pointer
	STR	R3, [R2]		; Save stack pointer

	;Start the thread body
	BLX	R1

	; The task terminated with a return: do the usual cleanup
	LDR	R0, =EE_thread_not_terminated
	BLX	R0

	; NOTE: code never executed because EE_thread_not_terminated()
	POP	{R2}			; Get xPSR from stack
	LDR	R0, =EPSR_T_BIT_VAL	; R0 = 0x01000000
	ORRS	R2, R2, R0		; R2 = (xPSR OR 0x01000000). This guarantees that Thumbs bit is set
					; to avoid an hard_fault exception
	MSR	PSR, R2			; Restore xPSR register
	POP	{R0}			; Get link register from stack
	MOV	LR, R0			; Restore the link register
	; Restore R8, R9, R10, R11 from stack
	POP	{R4-R7}
	MOV	R8, R4
	MOV	R9, R5
	MOV	R10, R6
	MOV	R11, R7
	POP	{R4-R7}			; Restore R4, R5, R6, R7 from stack

	BX	LR			; Return
	ENDFUNC

;void EE_hal_terminate_task(EE_TID tid) /* NORETURN; */
EE_hal_terminate_task	FUNCTION
	; R0 == tid

	; Restore the stack pointer
	; R1 == & EE_terminate_data[tid]
	LSLS	R0, R0, #2		; R0 = tid << 2	
	LDR	R1, =EE_terminate_data	; R1 == & EE_terminate_data[tid]
	ADD	R1, R1, R0
	LDR	R2, [R1]
	MSR	MSP, R2

	POP	{R2}			; Get xPSR from stack
	LDR	R0, =EPSR_T_BIT_VAL	; R0 = 0x01000000
	ORRS	R2, R2, R0		; R2 = (xPSR OR 0x01000000). This guarantees that Thumbs bit is set
					; to avoid an hard_fault exception
	MSR	PSR, R2			; Restore xPSR register
	POP	{R3}			; Get link register from stack
	MOV	LR, R3			; Restore the link register
	; Restore R8, R9, R10, R11 from stack
	POP	{R4-R7}
	MOV	R8, R4
	MOV	R9, R5
	MOV	R10, R6
	MOV	R11, R7
	POP	{R4-R7}			; Restore R4, R5, R6, R7 from stack

	BX	LR			; Return
	ENDFUNC

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
