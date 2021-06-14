	.file	"practice.c"
	.text
	.globl	poly
	.type	poly, @function
poly:
.LFB11:
	.cfi_startproc
	movsd	(%rdi), %xmm2
	movapd	%xmm0, %xmm1
	movl	$1, %eax
.L2:
	cmpq	%rsi, %rax
	jg	.L4
	movapd	%xmm1, %xmm3
	mulsd	(%rdi,%rax,8), %xmm3
	addsd	%xmm3, %xmm2
	mulsd	%xmm0, %xmm1
	addq	$1, %rax
	jmp	.L2
.L4:
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
.L6:
	testq	%rsi, %rsi
	js	.L8
	mulsd	%xmm0, %xmm1
	addsd	(%rdi,%rsi,8), %xmm1
	subq	$1, %rsi
	jmp	.L6
.L8:
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
	movss	.LC0(%rip), %xmm0
	movl	$0, %edx
.L10:
	cmpq	%rbx, %rdx
	jge	.L12
	mulss	(%rax,%rdx,4), %xmm0
	mulss	4(%rax,%rdx,4), %xmm0
	mulss	8(%rax,%rdx,4), %xmm0
	mulss	12(%rax,%rdx,4), %xmm0
	mulss	16(%rax,%rdx,4), %xmm0
	addq	$5, %rdx
	jmp	.L10
.L13:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
.L12:
	cmpq	%rbp, %rdx
	jl	.L13
	movss	%xmm0, 0(%r13)
	addq	$8, %rsp
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
	.cfi_endproc
.LFE13:
	.size	combine5, .-combine5
	.globl	aprod
	.type	aprod, @function
aprod:
.LFB14:
	.cfi_startproc
	movsd	.LC1(%rip), %xmm0
	movl	$0, %eax
.L16:
	leaq	-2(%rsi), %rdx
	cmpq	%rax, %rdx
	jle	.L18
	movsd	16(%rdi,%rax,8), %xmm1
	mulsd	(%rdi,%rax,8), %xmm0
	mulsd	8(%rdi,%rax,8), %xmm1
	mulsd	%xmm1, %xmm0
	addq	$3, %rax
	jmp	.L16
.L19:
	mulsd	(%rdi,%rax,8), %xmm0
	addq	$1, %rax
.L18:
	cmpq	%rsi, %rax
	jl	.L19
	ret
	.cfi_endproc
.LFE14:
	.size	aprod, .-aprod
	.globl	merge
	.type	merge, @function
merge:
.LFB15:
	.cfi_startproc
	movl	$0, %r9d
	movl	$0, %eax
	movl	$0, %r8d
	cmpq	%rax, %r8
	movq	%rax, %r10
	cmovge	%r8, %r10
	cmpq	%rcx, %r10
	jge	.L31
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	jmp	.L23
.L22:
	movq	%r10, (%rdx,%r9,8)
	addq	%rbx, %r8
	movl	$1, %r10d
	subq	%rbx, %r10
	addq	%r10, %rax
	leaq	1(%r9), %r9
	cmpq	%rax, %r8
	movq	%rax, %r10
	cmovge	%r8, %r10
	cmpq	%rcx, %r10
	jge	.L24
.L23:
	movq	(%rdi,%r8,8), %r11
	movq	(%rsi,%rax,8), %r10
	cmpq	%r10, %r11
	setl	%bl
	movzbl	%bl, %ebx
	jge	.L22
	movq	%r11, %r10
	jmp	.L22
.L24:
	cmpq	%rcx, %r8
	jge	.L26
	movq	(%rdi,%r8,8), %r10
	movq	%r10, (%rdx,%r9,8)
	leaq	1(%r9), %r9
	leaq	1(%r8), %r8
	jmp	.L24
.L26:
	cmpq	%rcx, %rax
	jge	.L37
	movq	(%rsi,%rax,8), %rdi
	movq	%rdi, (%rdx,%r9,8)
	leaq	1(%r9), %r9
	leaq	1(%rax), %rax
	jmp	.L26
.L37:
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L32:
	.cfi_restore 3
	movq	(%rdi,%r8,8), %r10
	movq	%r10, (%rdx,%r9,8)
	leaq	1(%r9), %r9
	leaq	1(%r8), %r8
.L31:
	cmpq	%rcx, %r8
	jl	.L32
	jmp	.L34
.L35:
	movq	(%rsi,%rax,8), %rdi
	movq	%rdi, (%rdx,%r9,8)
	leaq	1(%r9), %r9
	leaq	1(%rax), %rax
.L34:
	cmpq	%rcx, %rax
	jl	.L35
	ret
	.cfi_endproc
.LFE15:
	.size	merge, .-merge
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC7:
	.string	"poly result: %f\npolyh result: %f\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC8:
	.string	"combine5 result: %f\n"
.LC9:
	.string	"%3ld "
	.text
	.globl	main
	.type	main, @function
main:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$88, %rsp
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 3, -40
	movq	.LC2(%rip), %rax
	movq	%rax, -80(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -72(%rbp)
	movq	.LC3(%rip), %rax
	movq	%rax, -64(%rbp)
	movsd	%xmm0, -56(%rbp)
	movq	.LC4(%rip), %rax
	movq	%rax, -48(%rbp)
	movq	.LC5(%rip), %rax
	movq	%rax, -40(%rbp)
	movl	$5, %esi
	movsd	.LC6(%rip), %xmm0
	leaq	-80(%rbp), %rdi
	call	poly
	movsd	%xmm0, -104(%rbp)
	movl	$5, %esi
	movsd	.LC6(%rip), %xmm0
	leaq	-80(%rbp), %rdi
	call	polyh
	movapd	%xmm0, %xmm1
	movsd	-104(%rbp), %xmm0
	movl	$.LC7, %edi
	movl	$2, %eax
	call	printf
	movl	$69, %edi
	call	new_vec
	movq	%rax, %r12
	movl	$0, %ebx
	jmp	.L39
.L40:
	addl	$1, %ebx
	pxor	%xmm0, %xmm0
	cvtsi2ss	%ebx, %xmm0
	movq	%r12, %rdi
	call	set_vec_element
.L39:
	movslq	%ebx, %rsi
	cmpl	$68, %ebx
	jle	.L40
	leaq	-84(%rbp), %rsi
	movq	%r12, %rdi
	call	combine5
	pxor	%xmm0, %xmm0
	cvtss2sd	-84(%rbp), %xmm0
	movl	$.LC8, %edi
	movl	$1, %eax
	call	printf
	movl	$560, %eax
	subq	%rax, %rsp
	movq	%rsp, %rdi
	movq	%rdi, %r9
	subq	%rax, %rsp
	movq	%rsp, %rsi
	movq	%rsi, %r8
	subq	$1104, %rsp
	movq	%rsp, %rdx
	movq	%rdx, %r12
	movl	$0, %eax
	jmp	.L41
.L42:
	leaq	(%rax,%rax), %rcx
	movq	%rcx, (%r9,%rax,8)
	addq	$1, %rcx
	movq	%rcx, (%r8,%rax,8)
	addq	$1, %rax
.L41:
	cmpq	$68, %rax
	jle	.L42
	movl	$69, %ecx
	call	merge
	movl	$0, %ebx
	movl	$0, %r13d
	jmp	.L43
.L48:
	movl	$10, %edi
	call	putchar
	movl	$0, %r13d
.L44:
	movq	(%r12,%rbx,8), %rsi
	movl	$.LC9, %edi
	movl	$0, %eax
	call	printf
	addq	$1, %rbx
	addl	$1, %r13d
.L43:
	cmpq	$137, %rbx
	jg	.L47
	cmpl	$9, %r13d
	jle	.L44
	jmp	.L48
.L47:
	movl	$10, %edi
	call	putchar
	movl	$0, %eax
	leaq	-24(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	main, .-main
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1065353216
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1072693248
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.align 8
.LC3:
	.long	0
	.long	1074790400
	.align 8
.LC4:
	.long	0
	.long	1075052544
	.align 8
.LC5:
	.long	0
	.long	1075970048
	.align 8
.LC6:
	.long	1374389535
	.long	1074339512
	.ident	"GCC: (GNU) 8.3.0"
