# void merge(long src1[], long src2[], long dest[], long n)
# src1 in rdi, src2 in rsi, dest in rdx, n in rcx
merge:
.LFB27:
	.cfi_startproc
	endbr64
	movq	%rdi, %r11            # src1 in r11
	movq	%rsi, %r10            # src2 in r10
	movq	%rdx, %r8             # dest in r8
	movq	%rcx, %rsi            # n in rsi
	movl	$0, %eax              # id in rax
	movl	$0, %edx              # i2 in rdx
	movl	$0, %ecx              # i1 in rcx
	jmp	.L20
.L21:
	addq	$1, %rdx
	movq	%r9, %rdi
.L22:
	movq	%rdi, (%r8,%rax,8)
	leaq	1(%rax), %rax
.L20:
	cmpq	%rdx, %rcx
	movq	%rdx, %rdi
	cmovge	%rcx, %rdi                # rdi = i1 or i2
	cmpq	%rsi, %rdi                # rdi > n ?
	jge	.L24
	movq	(%r11,%rcx,8), %rdi       # rdi = src1[i1]
	movq	(%r10,%rdx,8), %r9        # r9 = src2[i2]
	cmpq	%r9, %rdi                 
	jge	.L21                          # src1[i1] > src2[i2] 
	addq	$1, %rcx
	jmp	.L22
.L25:
	movq	(%r11,%rcx,8), %rdi
	movq	%rdi, (%r8,%rax,8)
	leaq	1(%rax), %rax
	leaq	1(%rcx), %rcx
.L24:                                 # the cleanup loops are here
	cmpq	%rsi, %rcx
	jl	.L25
	jmp	.L26
.L27:
	movq	(%r10,%rdx,8), %rcx
	movq	%rcx, (%r8,%rax,8)
	leaq	1(%rax), %rax
	leaq	1(%rdx), %rdx
.L26:
	cmpq	%rsi, %rdx
	jl	.L27
	ret
	.cfi_endproc

