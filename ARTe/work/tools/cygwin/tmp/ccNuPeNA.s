	.syntax unified
	.cpu cortex-m3
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.thumb
	.file	"eecfg.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.align	2
	.thumb
	.thumb_func
	.type	EE_cortex_mx_send_otm8, %function
EE_cortex_mx_send_otm8:
.LFB21:
	.file 1 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/cortex_mx/inc/ee_cpu.h"
	.loc 1 444 0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	.cfi_def_cfa_offset 4
	.cfi_offset 7, -4
	sub	sp, sp, #12
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	mov	r2, r0
	mov	r3, r1
	strb	r2, [r7, #7]
	strb	r3, [r7, #6]
	.loc 1 446 0
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.cfi_endproc
.LFE21:
	.size	EE_cortex_mx_send_otm8, .-EE_cortex_mx_send_otm8
	.align	2
	.thumb
	.thumb_func
	.type	EE_cortex_mx_send_otm32, %function
EE_cortex_mx_send_otm32:
.LFB22:
	.loc 1 449 0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	.cfi_def_cfa_offset 4
	.cfi_offset 7, -4
	sub	sp, sp, #12
	.cfi_def_cfa_offset 16
	add	r7, sp, #0
	.cfi_def_cfa_register 7
	mov	r3, r0
	str	r1, [r7]
	strb	r3, [r7, #7]
	.loc 1 451 0
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.cfi_endproc
.LFE22:
	.size	EE_cortex_mx_send_otm32, .-EE_cortex_mx_send_otm32
	.global	EE_hal_thread_body
	.section	.rodata
	.align	2
	.type	EE_hal_thread_body, %object
	.size	EE_hal_thread_body, 8
EE_hal_thread_body:
	.word	FuncTask1
	.word	FuncTask2
	.global	EE_th_ready_prio
	.align	2
	.type	EE_th_ready_prio, %object
	.size	EE_th_ready_prio, 8
EE_th_ready_prio:
	.word	1
	.word	2
	.global	EE_th_dispatch_prio
	.align	2
	.type	EE_th_dispatch_prio, %object
	.size	EE_th_dispatch_prio, 8
EE_th_dispatch_prio:
	.word	1
	.word	2
	.global	EE_th_next
	.data
	.align	2
	.type	EE_th_next, %object
	.size	EE_th_next, 8
EE_th_next:
	.word	-1
	.word	-1
	.comm	EE_th_nact,8,4
	.global	EE_stkfirst
	.align	2
	.type	EE_stkfirst, %object
	.size	EE_stkfirst, 4
EE_stkfirst:
	.word	-1
	.global	EE_rqfirst
	.align	2
	.type	EE_rqfirst, %object
	.size	EE_rqfirst, 4
EE_rqfirst:
	.word	-1
	.global	EE_sys_ceiling
	.bss
	.align	2
	.type	EE_sys_ceiling, %object
	.size	EE_sys_ceiling, 4
EE_sys_ceiling:
	.space	4
	.global	EE_counter_RAM
	.data
	.align	2
	.type	EE_counter_RAM, %object
	.size	EE_counter_RAM, 8
EE_counter_RAM:
	.word	0
	.word	-1
	.global	EE_alarm_ROM
	.section	.rodata
	.align	2
	.type	EE_alarm_ROM, %object
	.size	EE_alarm_ROM, 32
EE_alarm_ROM:
	.word	0
	.byte	0
	.space	3
	.word	0
	.word	0
	.word	0
	.byte	0
	.space	3
	.word	1
	.word	0
	.comm	EE_alarm_RAM,24,4
	.text
.Letext0:
	.file 2 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/common/inc/ee_types.h"
	.file 3 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/kernel/alarms/inc/ee_alarms.h"
	.file 4 "eecfg.c"
	.file 5 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/cortex_mx/inc/ee_context.h"
	.file 6 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/common/inc/ee_primitives.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x303
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF40
	.byte	0x1
	.4byte	.LASF41
	.4byte	.LASF42
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x38
	.4byte	0x25
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x3
	.4byte	.LASF5
	.byte	0x2
	.byte	0x4b
	.4byte	0x57
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF6
	.byte	0x2
	.byte	0x4c
	.4byte	0x69
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x5
	.byte	0x4
	.4byte	0x76
	.uleb128 0x6
	.uleb128 0x3
	.4byte	.LASF8
	.byte	0x2
	.byte	0x66
	.4byte	0x70
	.uleb128 0x3
	.4byte	.LASF9
	.byte	0x2
	.byte	0x69
	.4byte	0x70
	.uleb128 0x3
	.4byte	.LASF10
	.byte	0x1
	.byte	0x54
	.4byte	0x5e
	.uleb128 0x3
	.4byte	.LASF11
	.byte	0x1
	.byte	0x55
	.4byte	0x4c
	.uleb128 0x3
	.4byte	.LASF12
	.byte	0x1
	.byte	0x70
	.4byte	0x4c
	.uleb128 0x7
	.byte	0x8
	.byte	0x3
	.byte	0x52
	.4byte	0xcf
	.uleb128 0x8
	.4byte	.LASF13
	.byte	0x3
	.byte	0x53
	.4byte	0x98
	.byte	0
	.uleb128 0x8
	.4byte	.LASF14
	.byte	0x3
	.byte	0x54
	.4byte	0x98
	.byte	0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF15
	.byte	0x3
	.byte	0x55
	.4byte	0xae
	.uleb128 0x7
	.byte	0x10
	.byte	0x3
	.byte	0x5d
	.4byte	0x10f
	.uleb128 0x9
	.ascii	"c\000"
	.byte	0x3
	.byte	0x5e
	.4byte	0x98
	.byte	0
	.uleb128 0x8
	.4byte	.LASF16
	.byte	0x3
	.byte	0x60
	.4byte	0x2c
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF17
	.byte	0x3
	.byte	0x62
	.4byte	0xa3
	.byte	0x8
	.uleb128 0x9
	.ascii	"f\000"
	.byte	0x3
	.byte	0x64
	.4byte	0x77
	.byte	0xc
	.byte	0
	.uleb128 0x3
	.4byte	.LASF18
	.byte	0x3
	.byte	0x65
	.4byte	0xda
	.uleb128 0x7
	.byte	0xc
	.byte	0x3
	.byte	0x68
	.4byte	0x147
	.uleb128 0x8
	.4byte	.LASF19
	.byte	0x3
	.byte	0x69
	.4byte	0x98
	.byte	0
	.uleb128 0x8
	.4byte	.LASF20
	.byte	0x3
	.byte	0x6a
	.4byte	0x98
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF21
	.byte	0x3
	.byte	0x6b
	.4byte	0x98
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.4byte	.LASF22
	.byte	0x3
	.byte	0x6c
	.4byte	0x11a
	.uleb128 0xa
	.4byte	.LASF24
	.byte	0x1
	.2byte	0x1bb
	.4byte	.LFB21
	.4byte	.LFE21-.LFB21
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x186
	.uleb128 0xb
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x1bb
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0xc
	.4byte	.LASF23
	.byte	0x1
	.2byte	0x1bb
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.byte	0
	.uleb128 0xa
	.4byte	.LASF25
	.byte	0x1
	.2byte	0x1c0
	.4byte	.LFB22
	.4byte	.LFE22-.LFB22
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1ba
	.uleb128 0xb
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x1c0
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0xc
	.4byte	.LASF23
	.byte	0x1
	.2byte	0x1c0
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0xd
	.4byte	0x82
	.4byte	0x1ca
	.uleb128 0xe
	.4byte	0x1ca
	.byte	0x1
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF26
	.uleb128 0xf
	.4byte	.LASF27
	.byte	0x4
	.byte	0x10
	.4byte	0x1e2
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_hal_thread_body
	.uleb128 0x10
	.4byte	0x1ba
	.uleb128 0xd
	.4byte	0x8d
	.4byte	0x1f7
	.uleb128 0xe
	.4byte	0x1ca
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.4byte	.LASF28
	.byte	0x4
	.byte	0x16
	.4byte	0x208
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_th_ready_prio
	.uleb128 0x10
	.4byte	0x1e7
	.uleb128 0xf
	.4byte	.LASF29
	.byte	0x4
	.byte	0x1c
	.4byte	0x21e
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_th_dispatch_prio
	.uleb128 0x10
	.4byte	0x1e7
	.uleb128 0xf
	.4byte	.LASF30
	.byte	0x4
	.byte	0x2f
	.4byte	0x1e7
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_th_nact
	.uleb128 0xd
	.4byte	0xa3
	.4byte	0x244
	.uleb128 0xe
	.4byte	0x1ca
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.4byte	.LASF31
	.byte	0x4
	.byte	0x2a
	.4byte	0x234
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_th_next
	.uleb128 0xf
	.4byte	.LASF32
	.byte	0x4
	.byte	0x37
	.4byte	0x8d
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_sys_ceiling
	.uleb128 0xf
	.4byte	.LASF33
	.byte	0x4
	.byte	0x34
	.4byte	0xa3
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_rqfirst
	.uleb128 0xf
	.4byte	.LASF34
	.byte	0x4
	.byte	0x31
	.4byte	0xa3
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_stkfirst
	.uleb128 0xd
	.4byte	0xcf
	.4byte	0x298
	.uleb128 0xe
	.4byte	0x1ca
	.byte	0
	.byte	0
	.uleb128 0xf
	.4byte	.LASF35
	.byte	0x4
	.byte	0x3e
	.4byte	0x288
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_counter_RAM
	.uleb128 0xd
	.4byte	0x10f
	.4byte	0x2b9
	.uleb128 0xe
	.4byte	0x1ca
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.4byte	.LASF36
	.byte	0x4
	.byte	0x48
	.4byte	0x2ca
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_alarm_ROM
	.uleb128 0x10
	.4byte	0x2a9
	.uleb128 0xd
	.4byte	0x147
	.4byte	0x2df
	.uleb128 0xe
	.4byte	0x1ca
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.4byte	.LASF37
	.byte	0x4
	.byte	0x4d
	.4byte	0x2cf
	.uleb128 0x5
	.byte	0x3
	.4byte	EE_alarm_RAM
	.uleb128 0x11
	.4byte	.LASF38
	.byte	0x5
	.byte	0x42
	.4byte	0xa3
	.uleb128 0x11
	.4byte	.LASF39
	.byte	0x6
	.byte	0x38
	.4byte	0x8d
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x15
	.byte	0
	.uleb128 0x27
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF38:
	.ascii	"EE_std_endcycle_next_tid\000"
.LASF21:
	.ascii	"next\000"
.LASF23:
	.ascii	"data\000"
.LASF13:
	.ascii	"value\000"
.LASF20:
	.ascii	"delta\000"
.LASF30:
	.ascii	"EE_th_nact\000"
.LASF24:
	.ascii	"EE_cortex_mx_send_otm8\000"
.LASF14:
	.ascii	"first\000"
.LASF7:
	.ascii	"unsigned int\000"
.LASF12:
	.ascii	"EE_TID\000"
.LASF17:
	.ascii	"TaskID\000"
.LASF27:
	.ascii	"EE_hal_thread_body\000"
.LASF0:
	.ascii	"unsigned char\000"
.LASF11:
	.ascii	"EE_SREG\000"
.LASF29:
	.ascii	"EE_th_dispatch_prio\000"
.LASF3:
	.ascii	"short unsigned int\000"
.LASF5:
	.ascii	"EE_INT32\000"
.LASF16:
	.ascii	"action\000"
.LASF40:
	.ascii	"GNU C 4.8.1 -march=armv7-m -mcpu=cortex-m3 -mthumb "
	.ascii	"-mthumb-interwork -ggdb -O0\000"
.LASF39:
	.ascii	"EE_IRQ_nesting_level\000"
.LASF32:
	.ascii	"EE_sys_ceiling\000"
.LASF36:
	.ascii	"EE_alarm_ROM\000"
.LASF9:
	.ascii	"EE_THREAD_PTR\000"
.LASF22:
	.ascii	"EE_alarm_RAM_type\000"
.LASF41:
	.ascii	"eecfg.c\000"
.LASF6:
	.ascii	"EE_UINT32\000"
.LASF31:
	.ascii	"EE_th_next\000"
.LASF4:
	.ascii	"EE_UINT8\000"
.LASF28:
	.ascii	"EE_th_ready_prio\000"
.LASF26:
	.ascii	"sizetype\000"
.LASF8:
	.ascii	"EE_VOID_CALLBACK\000"
.LASF33:
	.ascii	"EE_rqfirst\000"
.LASF37:
	.ascii	"EE_alarm_RAM\000"
.LASF18:
	.ascii	"EE_alarm_ROM_type\000"
.LASF10:
	.ascii	"EE_UREG\000"
.LASF34:
	.ascii	"EE_stkfirst\000"
.LASF2:
	.ascii	"short int\000"
.LASF35:
	.ascii	"EE_counter_RAM\000"
.LASF19:
	.ascii	"cycle\000"
.LASF42:
	.ascii	"C:\\eeCygwin\\home\\Pietro\\erika_sam\\Debug\000"
.LASF25:
	.ascii	"EE_cortex_mx_send_otm32\000"
.LASF1:
	.ascii	"signed char\000"
.LASF15:
	.ascii	"EE_counter_RAM_type\000"
	.ident	"GCC: (Sourcery CodeBench Lite 2013.11-24) 4.8.1"
