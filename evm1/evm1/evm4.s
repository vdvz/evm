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
	.fnstart //Marks the start of a function with an unwind table entry. 
.LFB2:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}	fp-frame pointer, lr-link register
	(this allows the program return to the “parent” function that initiated the “child” function call after the “child” function is finished.)
	add	fp, sp, #4		Add without Carry. sp-stack pointer
	sub	sp, sp, #16		Subtract without carry. sp-destination. sp-first opernad. #16-value
	ldr	r3, .L5+16		
	str	r3, [fp, #-8]
	mov	r3, #0
	str	r3, [fp, #-16]  //store with offset. r3-register to store. 
	mov	r3, #0			//move. r3-destination operand. #0-value
	str	r3, [fp, #-12]
	//load - move data into processor from memory. loads a data value to a specified register. 
	//store - moves data out of the processor to memory locations from a specified register.
	
.L3:
	ldr	r2, [fp, #-12]  //load register with offset (#offset), memory address based in fp-register, offset -12, push into r2(size=doubleword).ldr-чтение.str-запись
	ldr	r3, [fp, #-8]	//load register with offset (#offset), memory address based in fp-register, offset -8, push into r3 (size=doubleword)
	cmp	r2, r3			//compare. They update the condition flags on the result, but do not place the result in any register.
	bcs	.L2				//Branch with condition. Переход при переполнении.
	ldr	r3, [fp, #-12]	//
	vmov	s15, r3	@ int 	  //
	vcvt.f64.s32	d7, s15   //convert from signed-integer to floating-point. result push in d7 (d-double-precision, s-single-precision), operand holding in s15.
	vmov.f64	d1, d7		  //
	vldr.64	d0, .L5			  //
	bl	pow					  //Branch with Link.Unconditional jump to label=pow and and copies the address of the next instruction into LR(R14)
	vmov.f64	d6, d0		  //
	vldr.64	d7, .L5+8		  //Vector Floating Point - VFP. Расширение сопроцессора производящее низкозатратные вычисления чисел с плавающей точкой.
	vmul.f64	d5, d6, d7	  //
	ldr	r3, [fp, #-12]
	lsl	r3, r3, #1 			  //Logical Shift Left. #1-constant shift. r3 holding operand. r3(first) - destination register
	add	r3, r3, #1
	vmov	s15, r3	@ int
	vcvt.f64.s32	d6, s15   //convert from signed integer to floating-point.
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
	b	.L3			//Branch. Безусловный переход
.L2:
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, pc}
.L6:
	.align	3
.L5:
	.word	0 			//0
	.word	-1074790400 //-1
	.word	0 			//0
	.word	1074790400  //1
	.word	568041472   //4
	.cantunwind
	.fnend
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.9) 5.4.0 20160609"
	.section	.note.GNU-stack,"",%progbits