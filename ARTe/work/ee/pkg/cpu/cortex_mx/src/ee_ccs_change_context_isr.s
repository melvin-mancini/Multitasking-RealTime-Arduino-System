; ###*B*###
; ERIKA Enterprise - a tiny RTOS for small microcontrollers
;
; Copyright (C) 2002-2012  Evidence Srl
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

;	@file	ee_ccs_change_context_isr.s
;	@brief	Functions to active and manage the context switch for Cortex_MX 
;	@author	Gianluca Franchino
;	@author	Giuseppe Serano
;	@date	2012

;*******************************************************************************
;                         PUBLIC FUNCTIONS
;*******************************************************************************

; void EE_switch_context(void);
	.global	EE_switch_context

; void EE_cortex_mx_pendsv_handler(void);
	.global	EE_cortex_mx_pendsv_handler

; void EE_cortex_mx_svcall_handler(void);
	.global	EE_cortex_mx_svcall_handler

; void EE_set_switch_context_pri(void)
	.global	EE_set_switch_context_pri

; void EE_IRQ_end_instance(void);
	.global	EE_IRQ_end_instance

; void EE_cortex_mx_change_context(EE_TID tid);
	.global	EE_cortex_mx_change_context

; EE_TID EE_std_endcycle_next_tid;
	.global	EE_std_endcycle_next_tid

	.if $$isdefed("__MULTI__")
; int EE_std_need_context_change(EE_TID tid);
	.global	EE_std_need_context_change
	.endif

;*******************************************************************************
;                              CODE SECTION
;*******************************************************************************
	.text

NVIC_INT_CTRL	.word	0E000ED04h	; Interrupt control status register
NVIC_SHPR2	.word	0E000ED1Ch	; System priority register (SVCall 11)
NVIC_SHPR3	.word	0E000ED20h	; System priority register (PendSV 14)
NVIC_PENDSV_PRI	.word	000FF0000h	; PendSV priority OR-value (Lowest)
NVIC_SVCALL_PRI	.word	000FFFFFFh	; SVCall priority AND-value (Highest)
NVIC_PENDSVSET	.word	010000000h	; Value to trigger PendSV exception
NVIC_PENDSVCLR	.word	008000000h	; Value to un-trigger PendSV exception

EPSR_T_BIT_VAL	.word	001000000h	; Value to set the T-bit in EPSR
					; (always Thumb mode)

EXC_RETURN	.word	0xFFFFFFF9	; No-FPU, Thread-Mode, MSP.

			.if $$isdefed("__MONO__")
TID_IS_STACKED_MARK	.word	0x80000000
			.endif

_EE_cortex_mx_change_context_addr	.word EE_cortex_mx_change_context
_EE_std_endcycle_next_tid_addr		.word EE_std_endcycle_next_tid
_EE_cortex_mx_change_context_return_point_addr	.word EE_cortex_mx_change_context_return_point


; void EE_set_switch_context_pri(void)
EE_set_switch_context_pri:	.asmfunc

;Set PendSV priority to the minumum one
				LDR	R0, NVIC_SHPR3
				LDR	R1, NVIC_PENDSV_PRI
				LDR	R2, [R0];
				ORRS	R2, R2, R1;
				STR	R2, [R0];

;Set SVCall priority to the maximum one
				LDR	R0, NVIC_SHPR2
				LDR	R1, NVIC_SVCALL_PRI
				LDR	R2, [R0];
				ANDS	R2, R2, R1;
				STR	R2, [R0];

				BX	LR

				.endasmfunc


; void EE_switch_context(void)
EE_switch_context:	.asmfunc

; Trigger the PendSV exception (causes context switch)
			LDR	R0, NVIC_INT_CTRL
			LDR	R1, NVIC_PENDSVSET
			STR	R1, [R0]
			BX	LR

			.endasmfunc


; void EE_cortex_mx_pendsv_handler(void)
EE_cortex_mx_pendsv_handler:	.asmfunc

				CPSID	I	; Disable all interrupts.

; Clear the PendSV exception (preventing 2nd triggering)
				LDR	R0, NVIC_INT_CTRL
				LDR	R1, NVIC_PENDSVCLR
				STR	R1, [R0]

				BL	EE_IRQ_end_instance	; IRQ Scheduler.

; R0 = EE_std_endcycle_next_tid.
				LDR	R0, _EE_std_endcycle_next_tid_addr
				LDR	R0, [R0]

				.if $$isdefed("__MONO__")
; #define EE_std_need_context_change(tid) ((tid) >= 0)
				LDR	R1, TID_IS_STACKED_MARK
				ANDS	R0, R0, R1
				CBNZ	R0, EE_cortex_mx_pendsv_handler_end
				.endif

				.if $$isdefed("__MULTI__")
				BL	EE_std_need_context_change
				CBZ	R0, EE_cortex_mx_pendsv_handler_end
				.endif

; Build a stack frame to jump into the EE_std_change_context(EE_TID) at the end
; of PendSV_Handler.

; R0 = EE_std_endcycle_next_tid (R12)
				LDR	R0, _EE_std_endcycle_next_tid_addr
				LDR	R0, [R0]

; R3 = 0x01000000 (xPSR)
				LDR	R3, EPSR_T_BIT_VAL

; R2 = EE_cortex_mx_change_context (PC)
				LDR	R2, _EE_cortex_mx_change_context_addr

; R1 = exit_EE_cortex_mx_change_context (LR)
				LDR	R1, _EE_cortex_mx_change_context_return_point_addr

;|xPSR|-> xPSR AND 0xFFFFFE0
;| PC |-> EE_cortex_mx_change_context
;| LR |-> EE_cortex_mx_change_context_return_point
;| R12|
				PUSH	{R0-R3}

;| R3 |
;| R2 |
;| R1 |
;| R0 |
				PUSH	{R0-R3}

;Fake IRQ handler frame on top of PendSV frame:
;|xPSR|-> xPSR AND 0xFFFFFE0
;| PC |-> EE_cortex_mx_change_context
;| LR |-> EE_cortex_mx_change_context_return_point
;| R12|
;| R3 |
;| R2 |
;| R1 |
;| R0 | <- MSP
; R0 = EXC_RETURN -> Return to Thread mode.
;		  -> Exception return gets state from MSP.
;		  -> Execution uses MSP after return.
				LDR	R0, EXC_RETURN
				BX	R0		; EXC_RETURN.

				NOP			; Alignment. 

EE_cortex_mx_pendsv_handler_end:
; R0 = EXC_RETURN -> Return to Thread mode.
;		  -> Exception return gets state from MSP.
;		  -> Execution uses MSP after return.
				LDR	R0, EXC_RETURN
				CPSIE	I		; Enable all interrupts.
				BX	R0		; EXC_RETURN.

				.endasmfunc


EE_cortex_mx_change_context_return_point:	.asmfunc
; Enable interrupts (clear PRIMASK)
; NOTE:	If SVC is executed when PRIMASK is set to 1, HardFault Exception will
; 	occur. To solve this, instead of using PRIMASK to mask interrupts, use
; 	BASEPRI to mask particular interrupts.
						CPSIE	I
; SVCall exception to remove Original PendSV stack-frame.
						SVC	#0

						.endasmfunc

; void EE_cortex_mx_svc_ISR(void)
EE_cortex_mx_svcall_handler:	.asmfunc
; Remove SVCall Stack-Frame.
				ADD	SP, SP, #(8*4)

				BX	LR	; EXC_RETURN.

				.endasmfunc

	.end
