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
	.file	"ee_irqsc.c"
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
	.align	2
	.global	EE_IRQ_end_instance
	.thumb
	.thumb_func
	.type	EE_IRQ_end_instance, %function
EE_IRQ_end_instance:
.LFB39:
	.file 2 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg\\kernel\\fp\\src\\ee_irqsc.c"
	.loc 2 57 0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, r7, lr}
	.cfi_def_cfa_offset 12
	.cfi_offset 4, -12
	.cfi_offset 7, -8
	.cfi_offset 14, -4
	sub	sp, sp, #12
	.cfi_def_cfa_offset 24
	add	r7, sp, #0
	.cfi_def_cfa_register 7
.LBB10:
.LBB11:
	.file 3 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/kernel/fp/inc/ee_internal.h"
	.loc 3 97 0
	movw	r3, #:lower16:EE_rqfirst
	movt	r3, #:upper16:EE_rqfirst
	ldr	r3, [r3]
.LBE11:
.LBE10:
	.loc 2 60 0
	mov	r4, r3
	.loc 2 61 0
	cmp	r4, #-1
	beq	.L5
	.loc 2 61 0 is_stmt 0 discriminator 1
	movw	r3, #:lower16:EE_th_ready_prio
	movt	r3, #:upper16:EE_th_ready_prio
	ldr	r2, [r3, r4, lsl #2]
	movw	r3, #:lower16:EE_sys_ceiling
	movt	r3, #:upper16:EE_sys_ceiling
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L5
	.loc 2 68 0 is_stmt 1
	movw	r3, #:lower16:EE_th_dispatch_prio
	movt	r3, #:upper16:EE_th_dispatch_prio
	ldr	r2, [r3, r4, lsl #2]
	movw	r3, #:lower16:EE_sys_ceiling
	movt	r3, #:upper16:EE_sys_ceiling
	ldr	r3, [r3]
	orrs	r2, r2, r3
	movw	r3, #:lower16:EE_sys_ceiling
	movt	r3, #:upper16:EE_sys_ceiling
	str	r2, [r3]
	.loc 2 80 0
	bl	EE_rq2stk_exchange
	mov	r3, r0
	str	r3, [r7, #4]
.LBB12:
.LBB13:
	.file 4 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/cortex_mx/inc/ee_context.h"
	.loc 4 167 0
	movw	r3, #:lower16:EE_std_endcycle_next_tid
	movt	r3, #:upper16:EE_std_endcycle_next_tid
	ldr	r2, [r7, #4]
	str	r2, [r3]
.LBE13:
.LBE12:
	.loc 2 80 0
	b	.L3
.L5:
.LBB14:
.LBB15:
	.loc 3 105 0
	movw	r3, #:lower16:EE_stkfirst
	movt	r3, #:upper16:EE_stkfirst
	ldr	r3, [r3]
	str	r3, [r7]
.LBE15:
.LBE14:
.LBB16:
.LBB17:
	.loc 4 173 0
	movw	r3, #:lower16:EE_std_endcycle_next_tid
	movt	r3, #:upper16:EE_std_endcycle_next_tid
	mov	r2, #-1
	str	r2, [r3]
.L3:
.LBE17:
.LBE16:
	.loc 2 85 0
	adds	r7, r7, #12
	mov	sp, r7
	@ sp needed
	pop	{r4, r7, pc}
	.cfi_endproc
.LFE39:
	.size	EE_IRQ_end_instance, .-EE_IRQ_end_instance
.Letext0:
	.file 5 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/common/inc/ee_types.h"
	.file 6 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/cpu/common/inc/ee_primitives.h"
	.file 7 "C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg/kernel/fp/inc/ee_common.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x21a
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF25
	.byte	0x1
	.4byte	.LASF26
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x5
	.byte	0x38
	.4byte	0x21
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
	.byte	0x5
	.byte	0x4b
	.4byte	0x53
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF6
	.byte	0x5
	.byte	0x4c
	.4byte	0x65
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x3
	.4byte	.LASF8
	.byte	0x1
	.byte	0x54
	.4byte	0x5a
	.uleb128 0x3
	.4byte	.LASF9
	.byte	0x1
	.byte	0x70
	.4byte	0x48
	.uleb128 0x5
	.4byte	.LASF10
	.byte	0x3
	.byte	0x5f
	.4byte	0x77
	.byte	0x1
	.uleb128 0x6
	.4byte	.LASF12
	.byte	0x4
	.byte	0xa5
	.byte	0x1
	.4byte	0xa6
	.uleb128 0x7
	.ascii	"tid\000"
	.byte	0x4
	.byte	0xa5
	.4byte	0x77
	.byte	0
	.uleb128 0x5
	.4byte	.LASF11
	.byte	0x3
	.byte	0x67
	.4byte	0x77
	.byte	0x1
	.uleb128 0x6
	.4byte	.LASF13
	.byte	0x4
	.byte	0xab
	.byte	0x1
	.4byte	0xca
	.uleb128 0x7
	.ascii	"tid\000"
	.byte	0x4
	.byte	0xab
	.4byte	0x77
	.byte	0
	.uleb128 0x8
	.4byte	.LASF15
	.byte	0x1
	.2byte	0x1bb
	.4byte	.LFB21
	.4byte	.LFE21-.LFB21
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xfe
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x1bb
	.4byte	0x28
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0xa
	.4byte	.LASF14
	.byte	0x1
	.2byte	0x1bb
	.4byte	0x28
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.byte	0
	.uleb128 0x8
	.4byte	.LASF16
	.byte	0x1
	.2byte	0x1c0
	.4byte	.LFB22
	.4byte	.LFE22-.LFB22
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x132
	.uleb128 0x9
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x1c0
	.4byte	0x28
	.uleb128 0x2
	.byte	0x91
	.sleb128 -9
	.uleb128 0xa
	.4byte	.LASF14
	.byte	0x1
	.2byte	0x1c0
	.4byte	0x5a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0xb
	.4byte	.LASF27
	.byte	0x2
	.byte	0x38
	.4byte	.LFB39
	.4byte	.LFE39-.LFB39
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1a5
	.uleb128 0xc
	.ascii	"t\000"
	.byte	0x2
	.byte	0x3a
	.4byte	0x77
	.uleb128 0x1
	.byte	0x54
	.uleb128 0xd
	.4byte	0x82
	.4byte	.LBB10
	.4byte	.LBE10-.LBB10
	.byte	0x2
	.byte	0x3c
	.uleb128 0xe
	.4byte	0x8e
	.4byte	.LBB12
	.4byte	.LBE12-.LBB12
	.byte	0x2
	.byte	0x50
	.4byte	0x17d
	.uleb128 0xf
	.4byte	0x9a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0xd
	.4byte	0xa6
	.4byte	.LBB14
	.4byte	.LBE14-.LBB14
	.byte	0x2
	.byte	0x53
	.uleb128 0x10
	.4byte	0xb2
	.4byte	.LBB16
	.4byte	.LBE16-.LBB16
	.byte	0x2
	.byte	0x53
	.uleb128 0xf
	.4byte	0xbe
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.uleb128 0x11
	.4byte	.LASF17
	.byte	0x6
	.byte	0x38
	.4byte	0x6c
	.uleb128 0x11
	.4byte	.LASF18
	.byte	0x4
	.byte	0x42
	.4byte	0x77
	.uleb128 0x12
	.4byte	0x6c
	.4byte	0x1c6
	.uleb128 0x13
	.byte	0
	.uleb128 0x11
	.4byte	.LASF19
	.byte	0x7
	.byte	0x65
	.4byte	0x1d1
	.uleb128 0x14
	.4byte	0x1bb
	.uleb128 0x11
	.4byte	.LASF20
	.byte	0x7
	.byte	0x66
	.4byte	0x1e1
	.uleb128 0x14
	.4byte	0x1bb
	.uleb128 0x12
	.4byte	0x77
	.4byte	0x1f1
	.uleb128 0x13
	.byte	0
	.uleb128 0x11
	.4byte	.LASF21
	.byte	0x7
	.byte	0x71
	.4byte	0x1e6
	.uleb128 0x11
	.4byte	.LASF22
	.byte	0x7
	.byte	0x72
	.4byte	0x6c
	.uleb128 0x11
	.4byte	.LASF23
	.byte	0x7
	.byte	0x77
	.4byte	0x77
	.uleb128 0x11
	.4byte	.LASF24
	.byte	0x7
	.byte	0x7a
	.4byte	0x77
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
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
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
	.uleb128 0x9
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
	.uleb128 0xa
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
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x1d
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
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
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
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
.LASF11:
	.ascii	"EE_stk_queryfirst\000"
.LASF15:
	.ascii	"EE_cortex_mx_send_otm8\000"
.LASF16:
	.ascii	"EE_cortex_mx_send_otm32\000"
.LASF13:
	.ascii	"EE_hal_endcycle_stacked\000"
.LASF9:
	.ascii	"EE_TID\000"
.LASF14:
	.ascii	"data\000"
.LASF0:
	.ascii	"unsigned char\000"
.LASF12:
	.ascii	"EE_hal_endcycle_ready\000"
.LASF26:
	.ascii	"C:\\eeCygwin\\opt\\erika\\svn\\ee\\pkg\\kernel\\fp\\"
	.ascii	"src\\ee_irqsc.c\000"
.LASF3:
	.ascii	"short unsigned int\000"
.LASF5:
	.ascii	"EE_INT32\000"
.LASF25:
	.ascii	"GNU C 4.8.1 -march=armv7-m -mcpu=cortex-m3 -mthumb "
	.ascii	"-mthumb-interwork -ggdb -O0\000"
.LASF17:
	.ascii	"EE_IRQ_nesting_level\000"
.LASF22:
	.ascii	"EE_sys_ceiling\000"
.LASF18:
	.ascii	"EE_std_endcycle_next_tid\000"
.LASF7:
	.ascii	"unsigned int\000"
.LASF6:
	.ascii	"EE_UINT32\000"
.LASF10:
	.ascii	"EE_rq_queryfirst\000"
.LASF21:
	.ascii	"EE_th_next\000"
.LASF4:
	.ascii	"EE_UINT8\000"
.LASF20:
	.ascii	"EE_th_dispatch_prio\000"
.LASF19:
	.ascii	"EE_th_ready_prio\000"
.LASF27:
	.ascii	"EE_IRQ_end_instance\000"
.LASF23:
	.ascii	"EE_rqfirst\000"
.LASF8:
	.ascii	"EE_UREG\000"
.LASF24:
	.ascii	"EE_stkfirst\000"
.LASF2:
	.ascii	"short int\000"
.LASF1:
	.ascii	"signed char\000"
	.ident	"GCC: (Sourcery CodeBench Lite 2013.11-24) 4.8.1"
