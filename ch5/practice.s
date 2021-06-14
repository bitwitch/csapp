	.file	"practice.c"
	.text
	.globl	poly
	.type	poly, @function
poly:
.LFB23:
	.cfi_startproc
	endbr64
	movapd	%xmm0, %xmm3
	movsd	(%rdi), %xmm1
	movl	$1, %eax
	jmp	.L2
.L3:
	movapd	%xmm0, %xmm2
	mulsd	(%rdi,%rax,8), %xmm2
	addsd	%xmm2, %xmm1
	mulsd	%xmm3, %xmm0
	addq	$1, %rax
.L2:
	cmpq	%rsi, %rax
	jle	.L3
	movapd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE23:
	.size	poly, .-poly
	.globl	polyh
	.type	polyh, @function
polyh:
.LFB24:
	.cfi_startproc
	endbr64
	movapd	%xmm0, %xmm1
	movsd	(%rdi,%rsi,8), %xmm0
	subq	$1, %rsi
	jmp	.L5
.L6:
	mulsd	%xmm1, %xmm0
	addsd	(%rdi,%rsi,8), %xmm0
	subq	$1, %rsi
.L5:
	testq	%rsi, %rsi
	jns	.L6
	ret
	.cfi_endproc
.LFE24:
	.size	polyh, .-polyh
	.globl	combine5
	.type	combine5, @function
combine5:
.LFB25:
	.cfi_startproc
	endbr64
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
	movq	%rdi, %r13
	movq	%rsi, %r12
	call	vec_length@PLT
	movq	%rax, %rbp
	leaq	-4(%rax), %rbx
	movq	%r13, %rdi
	call	get_vec_start@PLT
	movq	%rax, %rdx
	movss	.LC0(%rip), %xmm0
	movl	$0, %eax
	jmp	.L8
.L9:
	mulss	(%rdx,%rax,4), %xmm0
	mulss	4(%rdx,%rax,4), %xmm0
	mulss	8(%rdx,%rax,4), %xmm0
	mulss	12(%rdx,%rax,4), %xmm0
	mulss	16(%rdx,%rax,4), %xmm0
	addq	$5, %rax
.L8:
	cmpq	%rbx, %rax
	jl	.L9
.L10:
	cmpq	%rbp, %rax
	jge	.L13
	mulss	(%rdx,%rax,4), %xmm0
	addq	$1, %rax
	jmp	.L10
.L13:
	movss	%xmm0, (%r12)
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
.LFE25:
	.size	combine5, .-combine5
	.globl	aprod
	.type	aprod, @function
aprod:
.LFB26:
	.cfi_startproc
	endbr64
	movsd	.LC1(%rip), %xmm0
	movl	$0, %eax
	jmp	.L15
.L16:
	movsd	16(%rdi,%rax,8), %xmm1
	mulsd	(%rdi,%rax,8), %xmm0
	mulsd	8(%rdi,%rax,8), %xmm1
	mulsd	%xmm1, %xmm0
	addq	$3, %rax
.L15:
	leaq	-2(%rsi), %rdx
	cmpq	%rax, %rdx
	jg	.L16
	jmp	.L17
.L18:
	mulsd	(%rdi,%rax,8), %xmm0
	addq	$1, %rax
.L17:
	cmpq	%rsi, %rax
	jl	.L18
	ret
	.cfi_endproc
.LFE26:
	.size	aprod, .-aprod
	.globl	merge
	.type	merge, @function
merge:
.LFB27:
	.cfi_startproc
	endbr64
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, %r10
	movq	%rsi, %r9
	movq	%rdx, %rdi
	movq	%rcx, %rsi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movq	$0, 8(%rsp)
	movq	$0, 16(%rsp)
	jmp	.L20
.L28:
	leaq	16(%rsp), %r8
.L21:
	movq	(%r8), %rcx
	addq	$1, %rcx
	movq	%rcx, (%r8)
	movq	%rdx, (%rdi,%rax,8)
	leaq	1(%rax), %rax
.L20:
	movq	8(%rsp), %rcx
	movq	16(%rsp), %rdx
	cmpq	%rdx, %rcx
	movq	%rdx, %r8
	cmovge	%rcx, %r8
	cmpq	%rsi, %r8
	jge	.L23
	movq	(%r9,%rdx,8), %rdx
	movq	(%r10,%rcx,8), %rcx
	cmpq	%rcx, %rdx
	cmovg	%rcx, %rdx
	jle	.L28
	leaq	8(%rsp), %r8
	jmp	.L21
.L24:
	leaq	1(%rdx), %rcx
	movq	%rcx, 8(%rsp)
	movq	(%r10,%rdx,8), %rdx
	movq	%rdx, (%rdi,%rax,8)
	leaq	1(%rax), %rax
.L23:
	movq	8(%rsp), %rdx
	cmpq	%rsi, %rdx
	jl	.L24
	jmp	.L25
.L26:
	leaq	1(%rdx), %rcx
	movq	%rcx, 16(%rsp)
	movq	(%r9,%rdx,8), %rdx
	movq	%rdx, (%rdi,%rax,8)
	leaq	1(%rax), %rax
.L25:
	movq	16(%rsp), %rdx
	cmpq	%rsi, %rdx
	jl	.L26
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L30
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L30:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE27:
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
.LFB28:
	.cfi_startproc
	endbr64
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
	movq	%fs:40, %rax
	movq	%rax, -40(%rbp)
	xorl	%eax, %eax
	movq	.LC2(%rip), %rax
	movq	%rax, -96(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -88(%rbp)
	movq	.LC3(%rip), %rax
	movq	%rax, -80(%rbp)
	movsd	%xmm0, -72(%rbp)
	movq	.LC4(%rip), %rax
	movq	%rax, -64(%rbp)
	movq	.LC5(%rip), %rax
	movq	%rax, -56(%rbp)
	leaq	-96(%rbp), %r12
	movl	$5, %esi
	movq	.LC6(%rip), %rax
	movq	%rax, %xmm0
	movq	%r12, %rdi
	call	poly
	movq	%xmm0, %rbx
	movl	$5, %esi
	movq	.LC6(%rip), %rax
	movq	%rax, %xmm0
	movq	%r12, %rdi
	call	polyh
	movapd	%xmm0, %xmm1
	movq	%rbx, %xmm0
	leaq	.LC7(%rip), %rsi
	movl	$1, %edi
	movl	$2, %eax
	call	__printf_chk@PLT
	movl	$69, %edi
	call	new_vec@PLT
	movq	%rax, %r12
	movl	$0, %ebx
	jmp	.L32
.L33:
	addl	$1, %ebx
	pxor	%xmm0, %xmm0
	cvtsi2ssl	%ebx, %xmm0
	movq	%r12, %rdi
	call	set_vec_element@PLT
.L32:
	movslq	%ebx, %rsi
	cmpl	$68, %ebx
	jle	.L33
	leaq	-100(%rbp), %rsi
	movq	%r12, %rdi
	call	combine5
	pxor	%xmm0, %xmm0
	cvtss2sd	-100(%rbp), %xmm0
	leaq	.LC8(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk@PLT
	movq	%rsp, %rax
.L34:
	cmpq	%rax, %rsp
	je	.L35
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L34
.L35:
	subq	$560, %rsp
	orq	$0, 552(%rsp)
	movq	%rsp, %rdi
	movq	%rsp, %rax
.L37:
	cmpq	%rax, %rsp
	je	.L38
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L37
.L38:
	subq	$560, %rsp
	orq	$0, 552(%rsp)
	movq	%rsp, %rsi
	movq	%rsp, %rax
.L40:
	cmpq	%rax, %rsp
	je	.L41
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L40
.L41:
	subq	$1104, %rsp
	orq	$0, 1096(%rsp)
	movq	%rsp, %r12
	movl	$0, %eax
	jmp	.L43
.L44:
	leaq	(%rax,%rax), %rdx
	movq	%rdx, (%rdi,%rax,8)
	addq	$1, %rdx
	movq	%rdx, (%rsi,%rax,8)
	addq	$1, %rax
.L43:
	cmpq	$68, %rax
	jle	.L44
	movl	$69, %ecx
	movq	%r12, %rdx
	call	merge
	movl	$0, %ebx
	movl	$0, %r13d
	jmp	.L45
.L46:
	movq	(%r12,%rbx,8), %rdx
	leaq	.LC9(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	addq	$1, %rbx
	addl	$1, %r13d
.L45:
	cmpq	$137, %rbx
	jg	.L50
	cmpl	$9, %r13d
	jle	.L46
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, %r13d
	jmp	.L46
.L50:
	movl	$10, %edi
	call	putchar@PLT
	movq	-40(%rbp), %rax
	subq	%fs:40, %rax
	jne	.L51
	movl	$0, %eax
	leaq	-24(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L51:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE28:
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
	.ident	"GCC: (Ubuntu 10.2.0-5ubuntu1~20.04) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
