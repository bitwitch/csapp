	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_rfact                  ## -- Begin function rfact
	.p2align	4, 0x90
_rfact:                                 ## @rfact
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$1, -8(%rbp)
	jg	LBB0_2
## %bb.1:
	movq	$1, -16(%rbp)
	jmp	LBB0_3
LBB0_2:
	movq	-8(%rbp), %rax
	movq	-8(%rbp), %rcx
	subq	$1, %rcx
	movq	%rcx, %rdi
	movq	%rax, -24(%rbp)         ## 8-byte Spill
	callq	_rfact
	movq	-24(%rbp), %rcx         ## 8-byte Reload
	imulq	%rax, %rcx
	movq	%rcx, -16(%rbp)
LBB0_3:
	movq	-16(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$5, %eax
	movl	%eax, %edi
	movl	$0, -4(%rbp)
	callq	_rfact
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rsi
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	movl	%ecx, %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%ld"


.subsections_via_symbols
