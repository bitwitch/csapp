	.file	"vec.c"
	.text
	.globl	new_vec
	.type	new_vec, @function
new_vec:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbp
	movl	$24, %edi
	call	malloc@PLT
	movq	%rax, %rbx
	testq	%rax, %rax
	je	.L1
	movq	%rbp, (%rax)
	movq	%rbp, 16(%rax)
	testq	%rbp, %rbp
	jg	.L6
	movl	$0, %ebp
.L3:
	movq	%rbp, 8(%rbx)
.L1:
	movq	%rbx, %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	movl	$4, %esi
	movq	%rbp, %rdi
	call	calloc@PLT
	movq	%rax, %rbp
	testq	%rax, %rax
	jne	.L3
	movq	%rbx, %rdi
	call	free@PLT
	movq	%rbp, %rbx
	jmp	.L1
	.cfi_endproc
.LFE16:
	.size	new_vec, .-new_vec
	.globl	free_vec
	.type	free_vec, @function
free_vec:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.L8
	call	free@PLT
.L8:
	movq	%rbx, %rdi
	call	free@PLT
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE17:
	.size	free_vec, .-free_vec
	.globl	get_vec_element
	.type	get_vec_element, @function
get_vec_element:
.LFB18:
	.cfi_startproc
	endbr64
	testq	%rsi, %rsi
	js	.L12
	cmpq	%rsi, (%rdi)
	jle	.L13
	movq	8(%rdi), %rax
	movss	(%rax,%rsi,4), %xmm0
	movss	%xmm0, (%rdx)
	movl	$1, %eax
	ret
.L12:
	movl	$0, %eax
	ret
.L13:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE18:
	.size	get_vec_element, .-get_vec_element
	.globl	vec_length
	.type	vec_length, @function
vec_length:
.LFB19:
	.cfi_startproc
	endbr64
	movq	(%rdi), %rax
	ret
	.cfi_endproc
.LFE19:
	.size	vec_length, .-vec_length
	.globl	get_vec_start
	.type	get_vec_start, @function
get_vec_start:
.LFB20:
	.cfi_startproc
	endbr64
	movq	8(%rdi), %rax
	ret
	.cfi_endproc
.LFE20:
	.size	get_vec_start, .-get_vec_start
	.globl	set_vec_element
	.type	set_vec_element, @function
set_vec_element:
.LFB21:
	.cfi_startproc
	endbr64
	testq	%rsi, %rsi
	js	.L18
	cmpq	%rsi, (%rdi)
	jle	.L19
	salq	$2, %rsi
	addq	8(%rdi), %rsi
	movss	%xmm0, (%rsi)
	movl	$1, %eax
	ret
.L18:
	movl	$0, %eax
	ret
.L19:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE21:
	.size	set_vec_element, .-set_vec_element
	.globl	set_vec_length
	.type	set_vec_length, @function
set_vec_length:
.LFB22:
	.cfi_startproc
	endbr64
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
	call	free@PLT
	movl	$4, %esi
	movq	%rbp, %rdi
	call	calloc@PLT
	movq	%rax, 8(%rbx)
	movq	%rbp, 16(%rbx)
	jmp	.L21
	.cfi_endproc
.LFE22:
	.size	set_vec_length, .-set_vec_length
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
