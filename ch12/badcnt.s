	.file	"badcnt.c"
	.intel_syntax noprefix
	.text
	.globl	thread
	.type	thread, @function


thread:
.LFB76:
	.cfi_startproc
	endbr64
	mov	rcx, QWORD PTR [rdi]        # load niters
	mov	edx, 0                      # initialize i
	jmp	.L2

.L3:
	mov	rax, QWORD PTR cnt[rip]     # load cnt
	add	rax, 1                      # update cnt
	mov	QWORD PTR cnt[rip], rax     # store cnt

	add	rdx, 1                      # increment i
.L2:
	cmp	rdx, rcx                    # while i < niters
	jl	.L3                         # go to .L3 
	mov	eax, 0
	ret
	.cfi_endproc






.LFE76:
	.size	thread, .-thread
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"usage: %s <niters>\n"
.LC1:
	.string	"BOOM! cnt=%ld\n"
.LC2:
	.string	"OK cnt=%ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB75:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	sub	rsp, 32
	.cfi_def_cfa_offset 48
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 24[rsp], rax
	xor	eax, eax
	cmp	edi, 2
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
	cdqe
	mov	QWORD PTR [rsp], rax
	mov	rbx, rsp
	lea	rdi, 8[rsp]
	mov	rcx, rbx
	lea	rdx, thread[rip]
	mov	esi, 0
	call	Pthread_create@PLT
	lea	rdi, 16[rsp]
	mov	rcx, rbx
	lea	rdx, thread[rip]
	mov	esi, 0
	call	Pthread_create@PLT
	mov	esi, 0
	mov	rdi, QWORD PTR 8[rsp]
	call	Pthread_join@PLT
	mov	esi, 0
	mov	rdi, QWORD PTR 16[rsp]
	call	Pthread_join@PLT
	mov	rax, QWORD PTR [rsp]
	add	rax, rax
	mov	rdx, QWORD PTR cnt[rip]
	cmp	rax, rdx
	je	.L6
	mov	rdx, QWORD PTR cnt[rip]
	lea	rsi, .LC1[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
.L7:
	mov	edi, 0
	call	exit@PLT
.L6:
	mov	rdx, QWORD PTR cnt[rip]
	lea	rsi, .LC2[rip]
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk@PLT
	jmp	.L7
	.cfi_endproc
.LFE75:
	.size	main, .-main
	.globl	cnt
	.bss
	.align 8
	.type	cnt, @object
	.size	cnt, 8
cnt:
	.zero	8
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
