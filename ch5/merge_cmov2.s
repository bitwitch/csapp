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
