	.file	"evm3.cpp"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movabsq	$400000000000, %rax
	movq	%rax, -8(%rbp)
	movq	$0, -16(%rbp)
	movl	$0, -20(%rbp)
.L7:
	movl	-20(%rbp), %eax
	cltq
	cmpq	-8(%rbp), %rax
	jnb	.L2
	pxor	%xmm0, %xmm0
	cvtsi2sd	-20(%rbp), %xmm0
	movapd	%xmm0, %xmm1
	movsd	.LC0(%rip), %xmm0
	call	pow
	movapd	%xmm0, %xmm1
	movsd	.LC1(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	movl	-20(%rbp), %eax
	addl	%eax, %eax
	addl	$1, %eax
	pxor	%xmm1, %xmm1
	cvtsi2sd	%eax, %xmm1
	divsd	%xmm1, %xmm0
	movapd	%xmm0, %xmm1
	movq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L3
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	jmp	.L4
.L3:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L4:
	addsd	%xmm1, %xmm0
	ucomisd	.LC2(%rip), %xmm0
	jnb	.L5
	cvttsd2siq	%xmm0, %rax
	movq	%rax, -16(%rbp)
	jmp	.L6
.L5:
	movsd	.LC2(%rip), %xmm1
	subsd	%xmm1, %xmm0
	cvttsd2siq	%xmm0, %rax
	movq	%rax, -16(%rbp)
	movabsq	$-9223372036854775808, %rax
	xorq	%rax, -16(%rbp)
.L6:
	movq	-16(%rbp), %rax
	movq	%rax, -16(%rbp)
	addl	$1, -20(%rbp)
	jmp	.L7
.L2:
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	-1074790400
	.align 8
.LC1:
	.long	0
	.long	1074790400
	.align 8
.LC2:
	.long	0
	.long	1138753536
	.ident	"GCC: (Ubuntu 5.5.0-12ubuntu1~16.04) 5.5.0 20171010"
	.section	.note.GNU-stack,"",@progbits
