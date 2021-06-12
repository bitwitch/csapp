	.file	"vec.c"
	.text
	.globl	new_vec
	.type	new_vec, @function
new_vec:
.LFB11:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbp
	movl	$24, %edi
	call	malloc
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L1
	movq	%rbp, (%rax)
	movq	%rbp, 16(%rax)
	movl	$0, %r12d
	testq	%rbp, %rbp
	jg	.L6
.L3:
	movq	%r12, 8(%rbx)
.L1:
	movq	%rbx, %rax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, %r12
	testq	%rax, %rax
	jne	.L3
	movq	%rbx, %rdi
	call	free
	movq	%r12, %rbx
	jmp	.L1
	.cfi_endproc
.LFE11:
	.size	new_vec, .-new_vec
	.globl	free_vec
	.type	free_vec, @function
free_vec:
.LFB12:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L8
	call	free
.L8:
	movq	%rbx, %rdi
	call	free
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	free_vec, .-free_vec
	.globl	get_vec_element
	.type	get_vec_element, @function
get_vec_element:
.LFB13:
	.cfi_startproc
	movl	$0, %eax
	testq	%rsi, %rsi
	js	.L10
	cmpq	%rsi, (%rdi)
	jle	.L10
	movq	8(%rdi), %rax
	movsd	(%rax,%rsi,8), %xmm0
	movsd	%xmm0, (%rdx)
	movl	$1, %eax
.L10:
	ret
	.cfi_endproc
.LFE13:
	.size	get_vec_element, .-get_vec_element
	.globl	vec_length
	.type	vec_length, @function
vec_length:
.LFB14:
	.cfi_startproc
	movq	(%rdi), %rax
	ret
	.cfi_endproc
.LFE14:
	.size	vec_length, .-vec_length
	.globl	get_vec_start
	.type	get_vec_start, @function
get_vec_start:
.LFB15:
	.cfi_startproc
	movq	8(%rdi), %rax
	ret
	.cfi_endproc
.LFE15:
	.size	get_vec_start, .-get_vec_start
	.globl	set_vec_element
	.type	set_vec_element, @function
set_vec_element:
.LFB16:
	.cfi_startproc
	movl	$0, %eax
	testq	%rsi, %rsi
	js	.L16
	cmpq	%rsi, (%rdi)
	jle	.L16
	movq	8(%rdi), %rax
	movsd	%xmm0, (%rax,%rsi,8)
	movl	$1, %eax
.L16:
	ret
	.cfi_endproc
.LFE16:
	.size	set_vec_element, .-set_vec_element
	.globl	set_vec_length
	.type	set_vec_length, @function
set_vec_length:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	cmpq	%rsi, 16(%rdi)
	jl	.L23
.L21:
	movq	%rbp, (%rbx)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L23:
	.cfi_restore_state
	movq	8(%rdi), %rdi
	call	free
	movl	$8, %esi
	movq	%rbp, %rdi
	call	calloc
	movq	%rax, 8(%rbx)
	movq	%rbp, 16(%rbx)
	jmp	.L21
	.cfi_endproc
.LFE17:
	.size	set_vec_length, .-set_vec_length
	.ident	"GCC: (GNU) 8.3.0"
