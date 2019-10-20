	.arch armv5t
	.eabi_attribute 28, 1
	.fpu vfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"evm3.cpp"
	.text
	.align	2
	.global	main
	.syntax unified
	.arm
	.type	main, %function
main:
	.fnstart
.LFB2:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	ldr	r3, .L5+16
	str	r3, [fp, #-8]
	mov	r3, #0
	str	r3, [fp, #-16]
	mov	r3, #0
	str	r3, [fp, #-12]
.L3:
	ldr	r2, [fp, #-12]
	ldr	r3, [fp, #-8]
	cmp	r2, r3
	bcs	.L2
	ldr	r3, [fp, #-12]
	vmov	s15, r3	@ int
	vcvt.f64.s32	d7, s15
	vmov.f64	d1, d7
	vldr.64	d0, .L5
	bl	pow
	vmov.f64	d6, d0
	vldr.64	d7, .L5+8
	vmul.f64	d5, d6, d7
	ldr	r3, [fp, #-12]
	lsl	r3, r3, #1
	add	r3, r3, #1
	vmov	s15, r3	@ int
	vcvt.f64.s32	d6, s15
	vdiv.f64	d7, d5, d6
	ldr	r3, [fp, #-16]
	vmov	s13, r3	@ int
	vcvt.f64.u32	d6, s13
	vadd.f64	d7, d6, d7
	vcvt.u32.f64	s15, d7
	vmov	r3, s15	@ int
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-12]
	add	r3, r3, #1
	str	r3, [fp, #-12]
	b	.L3
.L2:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
.L6:
	.align	3
.L5:
	.word	0
	.word	-1074790400
	.word	0
	.word	1074790400
	.word	568041472
	.cantunwind
	.fnend
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits
