	.file	"psum-local.c"
	.intel_syntax noprefix
	.text
	.globl	sum_local
	.type	sum_local, @function


sum_local:
.LFB76:
	.cfi_startproc
	endbr64
	mov	rsi, QWORD PTR [rdi]
	mov	rcx, QWORD PTR nelems_per_thread[rip]
	mov	rax, rcx
	imul	rax, rsi
	add	rcx, rax
	mov	edx, 0
	jmp	.L2

.L3:
	add	rdx, rax
	add	rax, 1

.L2:
	cmp	rax, rcx
	jl	.L3
	lea	rax, psum[rip]
	mov	QWORD PTR [rax+rsi*8], rdx
	mov	eax, 0
	ret
	.cfi_endproc


.LFE76:
	.size	sum_local, .-sum_local
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Usage: %s <nthreads> <log_nelems>\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Error: invalid nelems"
.LC2:
	.string	"Error: result=%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB75:
	.cfi_startproc
	endbr64
	push	r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	push	r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	push	r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	push	r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	push	rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	push	rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	sub	rsp, 536
	.cfi_def_cfa_offset 592
	mov	rbx, rsi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 520[rsp], rax
	xor	eax, eax
	cmp	edi, 3
	je	.L5
	mov	rdx, QWORD PTR [rsi]
	lea	rsi, .LC0[rip]
	mov	edi, 1
	call	__printf_chk@PLT
	mov	edi, 0
	call	exit@PLT
.L5:
	mov	rdi, QWORD PTR 8[rsi]
	mov	edx, 10
	mov	esi, 0
	call	strtol@PLT
	movsx	rbp, eax
	mov	rdi, QWORD PTR 16[rbx]
	mov	edx, 10
	mov	esi, 0
	call	strtol@PLT
	mov	r15, rax
	mov	r14d, 1
	mov	ecx, eax
	sal	r14, cl
	mov	rax, r14
	cqo
	idiv	rbp
	mov	r13, rdx
	mov	r12, rdx
	test	rdx, rdx
	jne	.L6
	cmp	r15d, 31
	jle	.L7
.L6:
	lea	rdi, .LC1[rip]
	call	puts@PLT
	mov	edi, 0
	call	exit@PLT
.L7:
	mov	rax, r14
	cqo
	idiv	rbp
	mov	QWORD PTR nelems_per_thread[rip], rax
	mov	rbx, r13
	jmp	.L8
.L9:
	mov	QWORD PTR [rsp+rbx*8], rbx
	lea	rax, 0[0+rbx*8]
	lea	rcx, [rsp+rax]
	lea	rdi, 256[rsp+rax]
	lea	rdx, sum_local[rip]
	mov	esi, 0
	call	Pthread_create@PLT
	add	rbx, 1
.L8:
	cmp	rbx, rbp
	jl	.L9
	mov	rbx, r13
	jmp	.L10
.L11:
	mov	rdi, QWORD PTR 256[rsp+rbx*8]
	mov	esi, 0
	call	Pthread_join@PLT
	add	rbx, 1
.L10:
	cmp	rbx, rbp
	jl	.L11
	jmp	.L12
.L13:
	lea	rax, psum[rip]
	add	r13, QWORD PTR [rax+r12*8]
	add	r12, 1
.L12:
	cmp	r12, rbp
	jl	.L13
	lea	rax, -1[r14]
	mov	ecx, r15d
	sal	rax, cl
	mov	ecx, 2
	cqo
	idiv	rcx
	cmp	rax, r13
	jne	.L17
.L14:
	mov	edi, 0
	call	exit@PLT
.L17:
	mov	rdx, r13
	lea	rsi, .LC2[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	jmp	.L14
	.cfi_endproc
.LFE75:
	.size	main, .-main
	.globl	nelems_per_thread
	.bss
	.align 8
	.type	nelems_per_thread, @object
	.size	nelems_per_thread, 8
nelems_per_thread:
	.zero	8
	.globl	psum
	.align 32
	.type	psum, @object
	.size	psum, 256
psum:
	.zero	256
	.ident	"GCC: (Ubuntu 10.3.0-1ubuntu1~20.04) 10.3.0"
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
