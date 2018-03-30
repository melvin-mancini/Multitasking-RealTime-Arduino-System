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
; 	@file ee_keil_multi_context.s
; 	@brief Context switch function for multistack on Cortex MX
; 	Implementation of EE_cortex_mx_change_context as described in
; 	pkg/cpu/common/inc/ee_context.h
; 	@author Gianluca Franchino
; 	@author Giuseppe Serano
; 	@date 2011
; */ 

;*******************************************************************************
;                         PUBLIC FUNCTIONS
;*******************************************************************************
	EXPORT	EE_cortex_mx_change_context

	IMPORT	EE_std_run_task_code
	IMPORT	EE_std_thread_tos
	IMPORT	EE_cortex_mx_active_tos
	IMPORT	EE_cortex_mx_system_tos

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

;Pseudo code for EE_cortex_mx_change_context():
;  begin:
;    tos_index = EE_std_thread_tos[tid+1];
;    if is_not_the_current_stack(tos_index) {
;      save_caller_saved_registers();
;      switch_stacks(tos_index);
;      restore_caller_saved_registers();
;    }
;    if (is_not_marked_stacked(tid)) {
;      tid = EE_std_run_task_code(tid);
;      goto begin;
;    }
;
;void EE_cortex_mx_change_context(EE_TID tid);
EE_cortex_mx_change_context
	; R0 == tid
	; tos_index = EE_std_thread_tos[tid+1];
	ADDS	R1, R0, #1		; R1 = tid+1
	LSLS	R1, R1, #2		; R1 = (tid+1)*4= correct offset in  EE_std_thread_tos
					; The last shift, also gets rid of the `stacked' mark
	LDR	R2, =EE_std_thread_tos
	ADD	R1, R2, R1
	LDR	R1, [R1]		;R1 == tos_index

	;*
	;* if is_not_the_current_stack(tos_index) { 
	;*
	LDR	R2, =EE_cortex_mx_active_tos	;R2 = & EE_cortex_mx_active_tos;
	LDR	R3, [R2]		; R3 = EE_cortex_mx_active_tos;
	CMP	R1, R3
	BEQ	end_change_stacks

	;save_caller_saved_registers();

	;Save all callee-saved registers
	;R0-R3 and R12 are scratch registers, R13 ->(MSP), R14 ->(LR), R15 -> (PC)
	PUSH	{R4-R7}			; Store R4, R5, R6, R7 onto stack 
	MOV	R4, R8
	MOV	R5, R9
	MOV	R6, R10
	MOV	R7, R11
	PUSH	{R4-R7}			; Store R8, R9, R10, R11 onto stack
	PUSH	{LR}			; Store link register (return address)

	;!!!!!!!!!!!!!!!
	; At this point the non scratch registers (R4...R11) are pushed into stack,
	; hence I can use them in the following.
	;!!!!!!!!!!!!!!!!

	; switch_stacks(tos_index);
	LDR	R4, =EE_cortex_mx_system_tos	; R4 = & cortex_mx_system_tos[0];
	; Note: although R4 is not a scratch register, it has been saved onto stack,
	; therefore we can used it in the follow without problem
	; EE_cortex_mx_system_tos[R3] = MSP;
	LSLS	R3, R3, #2
	ADD	R3, R4, R3
	MRS	R5, MSP
	; Note STR can only use the register range -> R0 to R4.
	STR	R5, [R3]		; save stack pointer

	; EE_cortex_mx_active_tos = tos_index;
	STR	R1, [R2]
	; MSP= EE_cortex_mx_system_tos[R1];
	LSLS	R1, R1, #2
	ADD	R1, R4, R1
	LDR	R1, [R1]
	MSR	MSP, R1
	; restore_caller_saved_registers();
	POP	{R1}			; Get link register from stack
	MOV	LR, R1			; Restore the link register
	; Restore R8, R9, R10, R11 from stack
	POP	{R4-R7}
	MOV	R8, R4
	MOV	R9, R5
	MOV	R10, R6
	MOV	R11, R7
	POP	{R4-R7}			; Restore R4, R5, R6, R7 from stack
	; *
	; * } 
	; *

end_change_stacks
	; R0 == tid

	; *
	; *if (is_not_marked_stacked(tid)) {
	; *

	CMP	R0, #0
	BLT	end_run_thread

	; tid = EE_std_run_task_code(tid);
	PUSH	{LR}
	BL EE_std_run_task_code
	; R0 == tid
	POP {R1}
	MOV LR, R1

	B EE_cortex_mx_change_context
	; goto begin

	; *
	; * } 
	; *

end_run_thread

	BX	LR			; Return

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

