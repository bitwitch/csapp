	.file	"practice.c"
	.text
	.globl	poly
	.type	poly, @function
poly:
.LFB11:
	.cfi_startproc
	movsd	(%rdi), %xmm2
	testq	%rsi, %rsi
	jle	.L1
	leaq	8(%rdi), %rax
	leaq	8(%rdi,%rsi,8), %rdx
	movapd	%xmm0, %xmm1
.L3:
	movapd	%xmm1, %xmm3
	mulsd	(%rax), %xmm3
	addsd	%xmm3, %xmm2
	mulsd	%xmm0, %xmm1
	addq	$8, %rax
	cmpq	%rdx, %rax
	jne	.L3
.L1:
	movapd	%xmm2, %xmm0
	ret
	.cfi_endproc
.LFE11:
	.size	poly, .-poly
	.globl	polyh
	.type	polyh, @function
polyh:
.LFB12:
	.cfi_startproc
	movsd	(%rdi,%rsi,8), %xmm1
	subq	$1, %rsi
	js	.L5
.L7:
	mulsd	%xmm0, %xmm1
	addsd	(%rdi,%rsi,8), %xmm1
	subq	$1, %rsi
	cmpq	$-1, %rsi
	jne	.L7
.L5:
	movapd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE12:
	.size	polyh, .-polyh
	.globl	combine5
	.type	combine5, @function
combine5:
.LFB13:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, %r12
	movq	%rsi, %r13
	call	vec_length
	movq	%rax, %rbp
	leaq	-4(%rax), %rbx
	movq	%r12, %rdi
	call	get_vec_start
	testq	%rbx, %rbx
	jle	.L14
	movsd	.LC0(%rip), %xmm0
	movl	$0, %edx
.L11:
	mulsd	(%rax,%rdx,8), %xmm0
	mulsd	8(%rax,%rdx,8), %xmm0
	mulsd	16(%rax,%rdx,8), %xmm0
	mulsd	24(%rax,%rdx,8), %xmm0
	mulsd	32(%rax,%rdx,8), %xmm0
	addq	$5, %rdx
	cmpq	%rdx, %rbx
	jg	.L11
.L10:
	cmpq	%rdx, %rbp
	jle	.L12
.L13:
	mulsd	(%rax,%rdx,8), %xmm0
	addq	$1, %rdx
	cmpq	%rdx, %rbp
	jne	.L13
.L12:
	movsd	%xmm0, 0(%r13)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L14:
	.cfi_restore_state
	movsd	.LC0(%rip), %xmm0
	movl	$0, %edx
	jmp	.L10
	.cfi_endproc
.LFE13:
	.size	combine5, .-combine5
	.globl	aprod
	.type	aprod, @function
aprod:
.LFB14:
	.cfi_startproc
	leaq	-2(%rsi), %rdx
	testq	%rdx, %rdx
	jle	.L23
	movsd	.LC0(%rip), %xmm0
	movl	$0, %eax
.L20:
	mulsd	(%rdi,%rax,8), %xmm0
	movsd	8(%rdi,%rax,8), %xmm1
	mulsd	16(%rdi,%rax,8), %xmm1
	mulsd	%xmm1, %xmm0
	addq	$3, %rax
	cmpq	%rdx, %rax
	jl	.L20
.L19:
	cmpq	%rax, %rsi
	jle	.L18
	leaq	(%rdi,%rax,8), %rax
	leaq	(%rdi,%rsi,8), %rdx
.L22:
	mulsd	(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rdx, %rax
	jne	.L22
.L18:
	ret
.L23:
	movsd	.LC0(%rip), %xmm0
	movl	$0, %eax
	jmp	.L19
	.cfi_endproc
.LFE14:
	.size	aprod, .-aprod
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC6:
	.string	"poly result: %f\npolyh result: %f\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC7:
	.string	"combine5 result: %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$88, %rsp
	.cfi_def_cfa_offset 112
	movq	.LC1(%rip), %rax
	movq	%rax, 32(%rsp)
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, 40(%rsp)
	movq	.LC2(%rip), %rax
	movq	%rax, 48(%rsp)
	movsd	%xmm0, 56(%rsp)
	movq	.LC3(%rip), %rax
	movq	%rax, 64(%rsp)
	movq	.LC4(%rip), %rax
	movq	%rax, 72(%rsp)
	movl	$5, %esi
	movsd	.LC5(%rip), %xmm0
	leaq	32(%rsp), %rdi
	call	poly
	movsd	%xmm0, 8(%rsp)
	movl	$5, %esi
	movsd	.LC5(%rip), %xmm0
	leaq	32(%rsp), %rdi
	call	polyh
	movapd	%xmm0, %xmm1
	movsd	8(%rsp), %xmm0
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
	movl	$69, %edi
	call	new_vec
	movq	%rax, %rbp
	movl	$0, %ebx
.L27:
	leal	1(%rbx), %eax
	pxor	%xmm0, %xmm0
	cvtsi2sd	%eax, %xmm0
	movq	%rbx, %rsi
	movq	%rbp, %rdi
	call	set_vec_element
	addq	$1, %rbx
	cmpq	$69, %rbx
	jne	.L27
	leaq	24(%rsp), %rsi
	movq	%rbp, %rdi
	call	combine5
	movsd	24(%rsp), %xmm0
	movl	$.LC7, %edi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	addq	$88, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE15:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1074266112
	.align 8
.LC2:
	.long	0
	.long	1074790400
	.align 8
.LC3:
	.long	0
	.long	1075052544
	.align 8
.LC4:
	.long	0
	.long	1075970048
	.align 8
.LC5:
	.long	1374389535
	.long	1074339512
	.ident	"GCC: (GNU) 8.3.0"
