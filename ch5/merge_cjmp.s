merge:
.LFB27:
	.cfi_startproc
	endbr64
	movq	%rdi, %r11
	movq	%rsi, %r10
	movq	%rdx, %rdi
	movq	%rcx, %rsi
	movl	$0, %eax
	movl	$0, %edx
	movl	$0, %ecx
	jmp	.L20
.L21:
	movq	%r8, (%rdi,%rax,8)
	addq	$1, %rdx
.L22:
	addq	$1, %rax
.L20:
	cmpq	%rdx, %rcx
	movq	%rdx, %r8
	cmovge	%rcx, %r8
	cmpq	%rsi, %r8
	jge	.L24
	movq	(%r11,%rcx,8), %r9
	movq	(%r10,%rdx,8), %r8
	cmpq	%r8, %r9
	jge	.L21
	movq	%r9, (%rdi,%rax,8)
	addq	$1, %rcx
	jmp	.L22
.L25:
	movq	(%r11,%rcx,8), %r8
	movq	%r8, (%rdi,%rax,8)
	leaq	1(%rax), %rax
	leaq	1(%rcx), %rcx
.L24:
	cmpq	%rsi, %rcx
	jl	.L25
	jmp	.L26
.L27:
	movq	(%r10,%rdx,8), %rcx
	movq	%rcx, (%rdi,%rax,8)
	leaq	1(%rax), %rax
	leaq	1(%rdx), %rdx
.L26:
	cmpq	%rsi, %rdx
	jl	.L27
	ret
	.cfi_endproc
