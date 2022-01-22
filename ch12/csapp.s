	.file	"csapp.c"
	.intel_syntax noprefix
	.text
	.type	sio_strlen, @function
sio_strlen:
.LFB100:
	.cfi_startproc
	mov	eax, 0
	jmp	.L2
.L3:
	add	eax, 1
.L2:
	movsx	rdx, eax
	cmp	BYTE PTR [rdi+rdx], 0
	jne	.L3
	mov	rax, rdx
	ret
	.cfi_endproc
.LFE100:
	.size	sio_strlen, .-sio_strlen
	.type	sio_reverse, @function
sio_reverse:
.LFB98:
	.cfi_startproc
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rdi
	call	strlen@PLT
	lea	ecx, -1[rax]
	mov	esi, 0
	jmp	.L5
.L6:
	movsx	rdx, esi
	add	rdx, rbx
	movzx	edi, BYTE PTR [rdx]
	movsx	rax, ecx
	add	rax, rbx
	movzx	r8d, BYTE PTR [rax]
	mov	BYTE PTR [rdx], r8b
	mov	BYTE PTR [rax], dil
	add	esi, 1
	sub	ecx, 1
.L5:
	cmp	esi, ecx
	jl	.L6
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE98:
	.size	sio_reverse, .-sio_reverse
	.type	sio_ltoa, @function
sio_ltoa:
.LFB99:
	.cfi_startproc
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	r11, rdi
	mov	r9, rsi
	mov	r10d, edx
	test	rdi, rdi
	js	.L17
	mov	rcx, rdi
.L9:
	mov	edi, 0
	jmp	.L12
.L17:
	mov	rcx, rdi
	neg	rcx
	jmp	.L9
.L19:
	add	edx, 48
.L11:
	lea	r8d, 1[rdi]
	movsx	rax, edi
	mov	BYTE PTR [r9+rax], dl
	mov	rax, rcx
	cqo
	idiv	rsi
	mov	rcx, rax
	test	rax, rax
	jle	.L18
	mov	edi, r8d
.L12:
	movsx	rsi, r10d
	mov	rax, rcx
	cqo
	idiv	rsi
	cmp	edx, 9
	jle	.L19
	add	edx, 87
	jmp	.L11
.L18:
	test	r11, r11
	js	.L20
.L13:
	movsx	r8, r8d
	mov	BYTE PTR [r9+r8], 0
	mov	rdi, r9
	call	sio_reverse
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L20:
	.cfi_restore_state
	movsx	r8, r8d
	mov	BYTE PTR [r9+r8], 45
	lea	r8d, 2[rdi]
	jmp	.L13
	.cfi_endproc
.LFE99:
	.size	sio_ltoa, .-sio_ltoa
	.type	rio_read, @function
rio_read:
.LFB157:
	.cfi_startproc
	push	r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	push	r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	push	rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	push	rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	sub	rsp, 8
	.cfi_def_cfa_offset 48
	mov	rbx, rdi
	mov	r13, rsi
	mov	r12, rdx
	jmp	.L22
.L33:
	call	__errno_location@PLT
	cmp	DWORD PTR [rax], 4
	jne	.L31
.L22:
	mov	ebp, DWORD PTR 4[rbx]
	test	ebp, ebp
	jg	.L32
	lea	rbp, 16[rbx]
	mov	edi, DWORD PTR [rbx]
	mov	edx, 8192
	mov	rsi, rbp
	call	read@PLT
	mov	DWORD PTR 4[rbx], eax
	test	eax, eax
	js	.L33
	je	.L29
	mov	QWORD PTR 8[rbx], rbp
	jmp	.L22
.L32:
	movsx	rax, ebp
	cmp	rax, r12
	jb	.L27
	mov	ebp, r12d
.L27:
	movsx	r12, ebp
	mov	rsi, QWORD PTR 8[rbx]
	mov	rdx, r12
	mov	rdi, r13
	call	memcpy@PLT
	add	QWORD PTR 8[rbx], r12
	sub	DWORD PTR 4[rbx], ebp
	mov	rax, r12
.L21:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbx
	.cfi_def_cfa_offset 32
	pop	rbp
	.cfi_def_cfa_offset 24
	pop	r12
	.cfi_def_cfa_offset 16
	pop	r13
	.cfi_def_cfa_offset 8
	ret
.L31:
	.cfi_restore_state
	mov	rax, -1
	jmp	.L21
.L29:
	mov	eax, 0
	jmp	.L21
	.cfi_endproc
.LFE157:
	.size	rio_read, .-rio_read
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%s: %s\n"
	.text
	.globl	unix_error
	.type	unix_error, @function
unix_error:
.LFB75:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rdi
	call	__errno_location@PLT
	mov	edi, DWORD PTR [rax]
	call	strerror@PLT
	mov	r8, rax
	mov	rcx, rbx
	lea	rdx, .LC0[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	edi, 0
	call	exit@PLT
	.cfi_endproc
.LFE75:
	.size	unix_error, .-unix_error
	.globl	posix_error
	.type	posix_error, @function
posix_error:
.LFB76:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rsi
	call	strerror@PLT
	mov	r8, rax
	mov	rcx, rbx
	lea	rdx, .LC0[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	edi, 0
	call	exit@PLT
	.cfi_endproc
.LFE76:
	.size	posix_error, .-posix_error
	.globl	gai_error
	.type	gai_error, @function
gai_error:
.LFB77:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rsi
	call	gai_strerror@PLT
	mov	r8, rax
	mov	rcx, rbx
	lea	rdx, .LC0[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	edi, 0
	call	exit@PLT
	.cfi_endproc
.LFE77:
	.size	gai_error, .-gai_error
	.section	.rodata.str1.1
.LC1:
	.string	"%s\n"
	.text
	.globl	app_error
	.type	app_error, @function
app_error:
.LFB78:
	.cfi_startproc
	endbr64
	push	rax
	.cfi_def_cfa_offset 16
	pop	rax
	.cfi_def_cfa_offset 8
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	rcx, rdi
	lea	rdx, .LC1[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	edi, 0
	call	exit@PLT
	.cfi_endproc
.LFE78:
	.size	app_error, .-app_error
	.globl	dns_error
	.type	dns_error, @function
dns_error:
.LFB79:
	.cfi_startproc
	endbr64
	push	rax
	.cfi_def_cfa_offset 16
	pop	rax
	.cfi_def_cfa_offset 8
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	rcx, rdi
	lea	rdx, .LC1[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	edi, 0
	call	exit@PLT
	.cfi_endproc
.LFE79:
	.size	dns_error, .-dns_error
	.section	.rodata.str1.1
.LC2:
	.string	"Fork error"
	.text
	.globl	Fork
	.type	Fork, @function
Fork:
.LFB80:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	fork@PLT
	test	eax, eax
	js	.L47
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L47:
	.cfi_restore_state
	lea	rdi, .LC2[rip]
	call	unix_error
	.cfi_endproc
.LFE80:
	.size	Fork, .-Fork
	.section	.rodata.str1.1
.LC3:
	.string	"Execve error"
	.text
	.globl	Execve
	.type	Execve, @function
Execve:
.LFB81:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	execve@PLT
	test	eax, eax
	js	.L51
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L51:
	.cfi_restore_state
	lea	rdi, .LC3[rip]
	call	unix_error
	.cfi_endproc
.LFE81:
	.size	Execve, .-Execve
	.section	.rodata.str1.1
.LC4:
	.string	"Wait error"
	.text
	.globl	Wait
	.type	Wait, @function
Wait:
.LFB82:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	wait@PLT
	test	eax, eax
	js	.L55
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L55:
	.cfi_restore_state
	lea	rdi, .LC4[rip]
	call	unix_error
	.cfi_endproc
.LFE82:
	.size	Wait, .-Wait
	.section	.rodata.str1.1
.LC5:
	.string	"Waitpid error"
	.text
	.globl	Waitpid
	.type	Waitpid, @function
Waitpid:
.LFB83:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	waitpid@PLT
	test	eax, eax
	js	.L59
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L59:
	.cfi_restore_state
	lea	rdi, .LC5[rip]
	call	unix_error
	.cfi_endproc
.LFE83:
	.size	Waitpid, .-Waitpid
	.section	.rodata.str1.1
.LC6:
	.string	"Kill error"
	.text
	.globl	Kill
	.type	Kill, @function
Kill:
.LFB84:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	kill@PLT
	test	eax, eax
	js	.L63
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L63:
	.cfi_restore_state
	lea	rdi, .LC6[rip]
	call	unix_error
	.cfi_endproc
.LFE84:
	.size	Kill, .-Kill
	.globl	Pause
	.type	Pause, @function
Pause:
.LFB85:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pause@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE85:
	.size	Pause, .-Pause
	.globl	Sleep
	.type	Sleep, @function
Sleep:
.LFB86:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sleep@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE86:
	.size	Sleep, .-Sleep
	.globl	Alarm
	.type	Alarm, @function
Alarm:
.LFB87:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	alarm@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE87:
	.size	Alarm, .-Alarm
	.section	.rodata.str1.1
.LC7:
	.string	"Setpgid error"
	.text
	.globl	Setpgid
	.type	Setpgid, @function
Setpgid:
.LFB88:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	setpgid@PLT
	test	eax, eax
	js	.L73
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L73:
	.cfi_restore_state
	lea	rdi, .LC7[rip]
	call	unix_error
	.cfi_endproc
.LFE88:
	.size	Setpgid, .-Setpgid
	.globl	Getpgrp
	.type	Getpgrp, @function
Getpgrp:
.LFB89:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	getpgrp@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE89:
	.size	Getpgrp, .-Getpgrp
	.section	.rodata.str1.1
.LC8:
	.string	"Signal error"
	.text
	.globl	Signal
	.type	Signal, @function
Signal:
.LFB90:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 328
	.cfi_def_cfa_offset 352
	mov	ebx, edi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 312[rsp], rax
	xor	eax, eax
	mov	QWORD PTR [rsp], rsi
	mov	rbp, rsp
	lea	rdi, 8[rsp]
	call	sigemptyset@PLT
	mov	DWORD PTR 136[rsp], 268435456
	lea	rdx, 160[rsp]
	mov	rsi, rbp
	mov	edi, ebx
	call	sigaction@PLT
	test	eax, eax
	js	.L80
	mov	rax, QWORD PTR 160[rsp]
	mov	rcx, QWORD PTR 312[rsp]
	sub	rcx, QWORD PTR fs:40
	jne	.L81
	add	rsp, 328
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L80:
	.cfi_restore_state
	lea	rdi, .LC8[rip]
	call	unix_error
.L81:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE90:
	.size	Signal, .-Signal
	.section	.rodata.str1.1
.LC9:
	.string	"Sigprocmask error"
	.text
	.globl	Sigprocmask
	.type	Sigprocmask, @function
Sigprocmask:
.LFB91:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sigprocmask@PLT
	test	eax, eax
	js	.L85
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L85:
	.cfi_restore_state
	lea	rdi, .LC9[rip]
	call	unix_error
	.cfi_endproc
.LFE91:
	.size	Sigprocmask, .-Sigprocmask
	.section	.rodata.str1.1
.LC10:
	.string	"Sigemptyset error"
	.text
	.globl	Sigemptyset
	.type	Sigemptyset, @function
Sigemptyset:
.LFB92:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sigemptyset@PLT
	test	eax, eax
	js	.L89
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L89:
	.cfi_restore_state
	lea	rdi, .LC10[rip]
	call	unix_error
	.cfi_endproc
.LFE92:
	.size	Sigemptyset, .-Sigemptyset
	.section	.rodata.str1.1
.LC11:
	.string	"Sigfillset error"
	.text
	.globl	Sigfillset
	.type	Sigfillset, @function
Sigfillset:
.LFB93:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sigfillset@PLT
	test	eax, eax
	js	.L93
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L93:
	.cfi_restore_state
	lea	rdi, .LC11[rip]
	call	unix_error
	.cfi_endproc
.LFE93:
	.size	Sigfillset, .-Sigfillset
	.section	.rodata.str1.1
.LC12:
	.string	"Sigaddset error"
	.text
	.globl	Sigaddset
	.type	Sigaddset, @function
Sigaddset:
.LFB94:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sigaddset@PLT
	test	eax, eax
	js	.L97
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L97:
	.cfi_restore_state
	lea	rdi, .LC12[rip]
	call	unix_error
	.cfi_endproc
.LFE94:
	.size	Sigaddset, .-Sigaddset
	.section	.rodata.str1.1
.LC13:
	.string	"Sigdelset error"
	.text
	.globl	Sigdelset
	.type	Sigdelset, @function
Sigdelset:
.LFB95:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sigdelset@PLT
	test	eax, eax
	js	.L101
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L101:
	.cfi_restore_state
	lea	rdi, .LC13[rip]
	call	unix_error
	.cfi_endproc
.LFE95:
	.size	Sigdelset, .-Sigdelset
	.section	.rodata.str1.1
.LC14:
	.string	"Sigismember error"
	.text
	.globl	Sigismember
	.type	Sigismember, @function
Sigismember:
.LFB96:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sigismember@PLT
	test	eax, eax
	js	.L105
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L105:
	.cfi_restore_state
	lea	rdi, .LC14[rip]
	call	unix_error
	.cfi_endproc
.LFE96:
	.size	Sigismember, .-Sigismember
	.section	.rodata.str1.1
.LC15:
	.string	"Sigsuspend error"
	.text
	.globl	Sigsuspend
	.type	Sigsuspend, @function
Sigsuspend:
.LFB97:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	call	sigsuspend@PLT
	mov	ebx, eax
	call	__errno_location@PLT
	cmp	DWORD PTR [rax], 4
	jne	.L109
	mov	eax, ebx
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L109:
	.cfi_restore_state
	lea	rdi, .LC15[rip]
	call	unix_error
	.cfi_endproc
.LFE97:
	.size	Sigsuspend, .-Sigsuspend
	.globl	sio_puts
	.type	sio_puts, @function
sio_puts:
.LFB101:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rdi
	call	sio_strlen
	mov	rdx, rax
	mov	rsi, rbx
	mov	edi, 1
	call	write@PLT
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE101:
	.size	sio_puts, .-sio_puts
	.globl	sio_putl
	.type	sio_putl, @function
sio_putl:
.LFB102:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	sub	rsp, 144
	.cfi_def_cfa_offset 160
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 136[rsp], rax
	xor	eax, eax
	mov	rbx, rsp
	mov	edx, 10
	mov	rsi, rbx
	call	sio_ltoa
	mov	rdi, rbx
	call	sio_puts
	mov	rcx, QWORD PTR 136[rsp]
	sub	rcx, QWORD PTR fs:40
	jne	.L115
	add	rsp, 144
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
.L115:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE102:
	.size	sio_putl, .-sio_putl
	.globl	sio_error
	.type	sio_error, @function
sio_error:
.LFB103:
	.cfi_startproc
	endbr64
	push	rax
	.cfi_def_cfa_offset 16
	pop	rax
	.cfi_def_cfa_offset 8
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sio_puts
	mov	edi, 1
	call	_exit@PLT
	.cfi_endproc
.LFE103:
	.size	sio_error, .-sio_error
	.section	.rodata.str1.1
.LC16:
	.string	"Sio_putl error"
	.text
	.globl	Sio_putl
	.type	Sio_putl, @function
Sio_putl:
.LFB104:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sio_putl
	test	rax, rax
	js	.L121
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L121:
	.cfi_restore_state
	lea	rdi, .LC16[rip]
	call	sio_error
	.cfi_endproc
.LFE104:
	.size	Sio_putl, .-Sio_putl
	.section	.rodata.str1.1
.LC17:
	.string	"Sio_puts error"
	.text
	.globl	Sio_puts
	.type	Sio_puts, @function
Sio_puts:
.LFB105:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sio_puts
	test	rax, rax
	js	.L125
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L125:
	.cfi_restore_state
	lea	rdi, .LC17[rip]
	call	sio_error
	.cfi_endproc
.LFE105:
	.size	Sio_puts, .-Sio_puts
	.globl	Sio_error
	.type	Sio_error, @function
Sio_error:
.LFB106:
	.cfi_startproc
	endbr64
	push	rax
	.cfi_def_cfa_offset 16
	pop	rax
	.cfi_def_cfa_offset 8
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sio_error
	.cfi_endproc
.LFE106:
	.size	Sio_error, .-Sio_error
	.section	.rodata.str1.1
.LC18:
	.string	"Open error"
	.text
	.globl	Open
	.type	Open, @function
Open:
.LFB107:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	eax, 0
	call	open@PLT
	test	eax, eax
	js	.L131
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L131:
	.cfi_restore_state
	lea	rdi, .LC18[rip]
	call	unix_error
	.cfi_endproc
.LFE107:
	.size	Open, .-Open
	.section	.rodata.str1.1
.LC19:
	.string	"Read error"
	.text
	.globl	Read
	.type	Read, @function
Read:
.LFB108:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	read@PLT
	test	rax, rax
	js	.L135
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L135:
	.cfi_restore_state
	lea	rdi, .LC19[rip]
	call	unix_error
	.cfi_endproc
.LFE108:
	.size	Read, .-Read
	.section	.rodata.str1.1
.LC20:
	.string	"Write error"
	.text
	.globl	Write
	.type	Write, @function
Write:
.LFB109:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	write@PLT
	test	rax, rax
	js	.L139
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L139:
	.cfi_restore_state
	lea	rdi, .LC20[rip]
	call	unix_error
	.cfi_endproc
.LFE109:
	.size	Write, .-Write
	.section	.rodata.str1.1
.LC21:
	.string	"Lseek error"
	.text
	.globl	Lseek
	.type	Lseek, @function
Lseek:
.LFB110:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	lseek@PLT
	test	rax, rax
	js	.L143
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L143:
	.cfi_restore_state
	lea	rdi, .LC21[rip]
	call	unix_error
	.cfi_endproc
.LFE110:
	.size	Lseek, .-Lseek
	.section	.rodata.str1.1
.LC22:
	.string	"Close error"
	.text
	.globl	Close
	.type	Close, @function
Close:
.LFB111:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	close@PLT
	test	eax, eax
	js	.L147
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L147:
	.cfi_restore_state
	lea	rdi, .LC22[rip]
	call	unix_error
	.cfi_endproc
.LFE111:
	.size	Close, .-Close
	.section	.rodata.str1.1
.LC23:
	.string	"Select error"
	.text
	.globl	Select
	.type	Select, @function
Select:
.LFB112:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	select@PLT
	test	eax, eax
	js	.L151
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L151:
	.cfi_restore_state
	lea	rdi, .LC23[rip]
	call	unix_error
	.cfi_endproc
.LFE112:
	.size	Select, .-Select
	.section	.rodata.str1.1
.LC24:
	.string	"Dup2 error"
	.text
	.globl	Dup2
	.type	Dup2, @function
Dup2:
.LFB113:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	dup2@PLT
	test	eax, eax
	js	.L155
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L155:
	.cfi_restore_state
	lea	rdi, .LC24[rip]
	call	unix_error
	.cfi_endproc
.LFE113:
	.size	Dup2, .-Dup2
	.section	.rodata.str1.1
.LC25:
	.string	"Stat error"
	.text
	.globl	Stat
	.type	Stat, @function
Stat:
.LFB114:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	rdx, rsi
	mov	rsi, rdi
	mov	edi, 1
	call	__xstat@PLT
	test	eax, eax
	js	.L159
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L159:
	.cfi_restore_state
	lea	rdi, .LC25[rip]
	call	unix_error
	.cfi_endproc
.LFE114:
	.size	Stat, .-Stat
	.section	.rodata.str1.1
.LC26:
	.string	"Fstat error"
	.text
	.globl	Fstat
	.type	Fstat, @function
Fstat:
.LFB115:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	rdx, rsi
	mov	esi, edi
	mov	edi, 1
	call	__fxstat@PLT
	test	eax, eax
	js	.L163
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L163:
	.cfi_restore_state
	lea	rdi, .LC26[rip]
	call	unix_error
	.cfi_endproc
.LFE115:
	.size	Fstat, .-Fstat
	.section	.rodata.str1.1
.LC27:
	.string	"opendir error"
	.text
	.globl	Opendir
	.type	Opendir, @function
Opendir:
.LFB116:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	opendir@PLT
	test	rax, rax
	je	.L167
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L167:
	.cfi_restore_state
	lea	rdi, .LC27[rip]
	call	unix_error
	.cfi_endproc
.LFE116:
	.size	Opendir, .-Opendir
	.section	.rodata.str1.1
.LC28:
	.string	"readdir error"
	.text
	.globl	Readdir
	.type	Readdir, @function
Readdir:
.LFB117:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 8
	.cfi_def_cfa_offset 32
	mov	rbp, rdi
	call	__errno_location@PLT
	mov	rbx, rax
	mov	DWORD PTR [rax], 0
	mov	rdi, rbp
	call	readdir@PLT
	test	rax, rax
	je	.L171
.L168:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L171:
	.cfi_restore_state
	cmp	DWORD PTR [rbx], 0
	je	.L168
	lea	rdi, .LC28[rip]
	call	unix_error
	.cfi_endproc
.LFE117:
	.size	Readdir, .-Readdir
	.section	.rodata.str1.1
.LC29:
	.string	"closedir error"
	.text
	.globl	Closedir
	.type	Closedir, @function
Closedir:
.LFB118:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	closedir@PLT
	test	eax, eax
	js	.L175
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L175:
	.cfi_restore_state
	lea	rdi, .LC29[rip]
	call	unix_error
	.cfi_endproc
.LFE118:
	.size	Closedir, .-Closedir
	.section	.rodata.str1.1
.LC30:
	.string	"mmap error"
	.text
	.globl	Mmap
	.type	Mmap, @function
Mmap:
.LFB119:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	mmap@PLT
	cmp	rax, -1
	je	.L179
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L179:
	.cfi_restore_state
	lea	rdi, .LC30[rip]
	call	unix_error
	.cfi_endproc
.LFE119:
	.size	Mmap, .-Mmap
	.section	.rodata.str1.1
.LC31:
	.string	"munmap error"
	.text
	.globl	Munmap
	.type	Munmap, @function
Munmap:
.LFB120:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	munmap@PLT
	test	eax, eax
	js	.L183
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L183:
	.cfi_restore_state
	lea	rdi, .LC31[rip]
	call	unix_error
	.cfi_endproc
.LFE120:
	.size	Munmap, .-Munmap
	.section	.rodata.str1.1
.LC32:
	.string	"Malloc error"
	.text
	.globl	Malloc
	.type	Malloc, @function
Malloc:
.LFB121:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	malloc@PLT
	test	rax, rax
	je	.L187
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L187:
	.cfi_restore_state
	lea	rdi, .LC32[rip]
	call	unix_error
	.cfi_endproc
.LFE121:
	.size	Malloc, .-Malloc
	.section	.rodata.str1.1
.LC33:
	.string	"Realloc error"
	.text
	.globl	Realloc
	.type	Realloc, @function
Realloc:
.LFB122:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	realloc@PLT
	test	rax, rax
	je	.L191
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L191:
	.cfi_restore_state
	lea	rdi, .LC33[rip]
	call	unix_error
	.cfi_endproc
.LFE122:
	.size	Realloc, .-Realloc
	.section	.rodata.str1.1
.LC34:
	.string	"Calloc error"
	.text
	.globl	Calloc
	.type	Calloc, @function
Calloc:
.LFB123:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	calloc@PLT
	test	rax, rax
	je	.L195
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L195:
	.cfi_restore_state
	lea	rdi, .LC34[rip]
	call	unix_error
	.cfi_endproc
.LFE123:
	.size	Calloc, .-Calloc
	.globl	Free
	.type	Free, @function
Free:
.LFB124:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	free@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE124:
	.size	Free, .-Free
	.section	.rodata.str1.1
.LC35:
	.string	"Fclose error"
	.text
	.globl	Fclose
	.type	Fclose, @function
Fclose:
.LFB125:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	fclose@PLT
	test	eax, eax
	jne	.L201
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L201:
	.cfi_restore_state
	lea	rdi, .LC35[rip]
	call	unix_error
	.cfi_endproc
.LFE125:
	.size	Fclose, .-Fclose
	.section	.rodata.str1.1
.LC36:
	.string	"Fdopen error"
	.text
	.globl	Fdopen
	.type	Fdopen, @function
Fdopen:
.LFB126:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	fdopen@PLT
	test	rax, rax
	je	.L205
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L205:
	.cfi_restore_state
	lea	rdi, .LC36[rip]
	call	unix_error
	.cfi_endproc
.LFE126:
	.size	Fdopen, .-Fdopen
	.section	.rodata.str1.1
.LC37:
	.string	"Fgets error"
	.text
	.globl	Fgets
	.type	Fgets, @function
Fgets:
.LFB127:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 8
	.cfi_def_cfa_offset 32
	mov	rbp, rdx
	call	fgets@PLT
	mov	rbx, rax
	test	rax, rax
	je	.L209
.L206:
	mov	rax, rbx
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L209:
	.cfi_restore_state
	mov	rdi, rbp
	call	ferror@PLT
	test	eax, eax
	je	.L206
	lea	rdi, .LC37[rip]
	call	app_error
	.cfi_endproc
.LFE127:
	.size	Fgets, .-Fgets
	.section	.rodata.str1.1
.LC38:
	.string	"Fopen error"
	.text
	.globl	Fopen
	.type	Fopen, @function
Fopen:
.LFB128:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	fopen@PLT
	test	rax, rax
	je	.L213
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L213:
	.cfi_restore_state
	lea	rdi, .LC38[rip]
	call	unix_error
	.cfi_endproc
.LFE128:
	.size	Fopen, .-Fopen
	.section	.rodata.str1.1
.LC39:
	.string	"Fputs error"
	.text
	.globl	Fputs
	.type	Fputs, @function
Fputs:
.LFB129:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	fputs@PLT
	cmp	eax, -1
	je	.L217
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L217:
	.cfi_restore_state
	lea	rdi, .LC39[rip]
	call	unix_error
	.cfi_endproc
.LFE129:
	.size	Fputs, .-Fputs
	.section	.rodata.str1.1
.LC40:
	.string	"Fread error"
	.text
	.globl	Fread
	.type	Fread, @function
Fread:
.LFB130:
	.cfi_startproc
	endbr64
	push	r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	push	rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	push	rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	mov	rbx, rdx
	mov	r12, rcx
	call	fread@PLT
	mov	rbp, rax
	cmp	rbx, rax
	ja	.L221
.L218:
	mov	rax, rbp
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbp
	.cfi_def_cfa_offset 16
	pop	r12
	.cfi_def_cfa_offset 8
	ret
.L221:
	.cfi_restore_state
	mov	rdi, r12
	call	ferror@PLT
	test	eax, eax
	je	.L218
	lea	rdi, .LC40[rip]
	call	unix_error
	.cfi_endproc
.LFE130:
	.size	Fread, .-Fread
	.section	.rodata.str1.1
.LC41:
	.string	"Fwrite error"
	.text
	.globl	Fwrite
	.type	Fwrite, @function
Fwrite:
.LFB131:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rdx
	call	fwrite@PLT
	cmp	rax, rbx
	jb	.L225
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L225:
	.cfi_restore_state
	lea	rdi, .LC41[rip]
	call	unix_error
	.cfi_endproc
.LFE131:
	.size	Fwrite, .-Fwrite
	.section	.rodata.str1.1
.LC42:
	.string	"Socket error"
	.text
	.globl	Socket
	.type	Socket, @function
Socket:
.LFB132:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	socket@PLT
	test	eax, eax
	js	.L229
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L229:
	.cfi_restore_state
	lea	rdi, .LC42[rip]
	call	unix_error
	.cfi_endproc
.LFE132:
	.size	Socket, .-Socket
	.section	.rodata.str1.1
.LC43:
	.string	"Setsockopt error"
	.text
	.globl	Setsockopt
	.type	Setsockopt, @function
Setsockopt:
.LFB133:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	setsockopt@PLT
	test	eax, eax
	js	.L233
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L233:
	.cfi_restore_state
	lea	rdi, .LC43[rip]
	call	unix_error
	.cfi_endproc
.LFE133:
	.size	Setsockopt, .-Setsockopt
	.section	.rodata.str1.1
.LC44:
	.string	"Bind error"
	.text
	.globl	Bind
	.type	Bind, @function
Bind:
.LFB134:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	bind@PLT
	test	eax, eax
	js	.L237
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L237:
	.cfi_restore_state
	lea	rdi, .LC44[rip]
	call	unix_error
	.cfi_endproc
.LFE134:
	.size	Bind, .-Bind
	.section	.rodata.str1.1
.LC45:
	.string	"Listen error"
	.text
	.globl	Listen
	.type	Listen, @function
Listen:
.LFB135:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	listen@PLT
	test	eax, eax
	js	.L241
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L241:
	.cfi_restore_state
	lea	rdi, .LC45[rip]
	call	unix_error
	.cfi_endproc
.LFE135:
	.size	Listen, .-Listen
	.section	.rodata.str1.1
.LC46:
	.string	"Accept error"
	.text
	.globl	Accept
	.type	Accept, @function
Accept:
.LFB136:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	accept@PLT
	test	eax, eax
	js	.L245
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L245:
	.cfi_restore_state
	lea	rdi, .LC46[rip]
	call	unix_error
	.cfi_endproc
.LFE136:
	.size	Accept, .-Accept
	.section	.rodata.str1.1
.LC47:
	.string	"Connect error"
	.text
	.globl	Connect
	.type	Connect, @function
Connect:
.LFB137:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	connect@PLT
	test	eax, eax
	js	.L249
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L249:
	.cfi_restore_state
	lea	rdi, .LC47[rip]
	call	unix_error
	.cfi_endproc
.LFE137:
	.size	Connect, .-Connect
	.section	.rodata.str1.1
.LC48:
	.string	"Getaddrinfo error"
	.text
	.globl	Getaddrinfo
	.type	Getaddrinfo, @function
Getaddrinfo:
.LFB138:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	getaddrinfo@PLT
	test	eax, eax
	jne	.L253
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L253:
	.cfi_restore_state
	lea	rsi, .LC48[rip]
	mov	edi, eax
	call	gai_error
	.cfi_endproc
.LFE138:
	.size	Getaddrinfo, .-Getaddrinfo
	.section	.rodata.str1.1
.LC49:
	.string	"Getnameinfo error"
	.text
	.globl	Getnameinfo
	.type	Getnameinfo, @function
Getnameinfo:
.LFB139:
	.cfi_startproc
	endbr64
	sub	rsp, 16
	.cfi_def_cfa_offset 24
	mov	eax, DWORD PTR 24[rsp]
	push	rax
	.cfi_def_cfa_offset 32
	call	getnameinfo@PLT
	add	rsp, 16
	.cfi_def_cfa_offset 16
	test	eax, eax
	jne	.L257
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L257:
	.cfi_restore_state
	lea	rsi, .LC49[rip]
	mov	edi, eax
	call	gai_error
	.cfi_endproc
.LFE139:
	.size	Getnameinfo, .-Getnameinfo
	.globl	Freeaddrinfo
	.type	Freeaddrinfo, @function
Freeaddrinfo:
.LFB140:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	freeaddrinfo@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE140:
	.size	Freeaddrinfo, .-Freeaddrinfo
	.section	.rodata.str1.1
.LC50:
	.string	"Inet_ntop error"
	.text
	.globl	Inet_ntop
	.type	Inet_ntop, @function
Inet_ntop:
.LFB141:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	inet_ntop@PLT
	test	rax, rax
	je	.L263
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L263:
	.cfi_restore_state
	lea	rdi, .LC50[rip]
	call	unix_error
	.cfi_endproc
.LFE141:
	.size	Inet_ntop, .-Inet_ntop
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC51:
	.string	"inet_pton error: invalid dotted-decimal address"
	.section	.rodata.str1.1
.LC52:
	.string	"Inet_pton error"
	.text
	.globl	Inet_pton
	.type	Inet_pton, @function
Inet_pton:
.LFB142:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	inet_pton@PLT
	test	eax, eax
	je	.L268
	js	.L269
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L268:
	.cfi_restore_state
	lea	rdi, .LC51[rip]
	call	app_error
.L269:
	lea	rdi, .LC52[rip]
	call	unix_error
	.cfi_endproc
.LFE142:
	.size	Inet_pton, .-Inet_pton
	.section	.rodata.str1.1
.LC53:
	.string	"Gethostbyname error"
	.text
	.globl	Gethostbyname
	.type	Gethostbyname, @function
Gethostbyname:
.LFB143:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	gethostbyname@PLT
	test	rax, rax
	je	.L273
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L273:
	.cfi_restore_state
	lea	rdi, .LC53[rip]
	call	dns_error
	.cfi_endproc
.LFE143:
	.size	Gethostbyname, .-Gethostbyname
	.section	.rodata.str1.1
.LC54:
	.string	"Gethostbyaddr error"
	.text
	.globl	Gethostbyaddr
	.type	Gethostbyaddr, @function
Gethostbyaddr:
.LFB144:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	gethostbyaddr@PLT
	test	rax, rax
	je	.L277
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L277:
	.cfi_restore_state
	lea	rdi, .LC54[rip]
	call	dns_error
	.cfi_endproc
.LFE144:
	.size	Gethostbyaddr, .-Gethostbyaddr
	.section	.rodata.str1.1
.LC55:
	.string	"Pthread_create error"
	.text
	.globl	Pthread_create
	.type	Pthread_create, @function
Pthread_create:
.LFB145:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_create@PLT
	test	eax, eax
	jne	.L281
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L281:
	.cfi_restore_state
	lea	rsi, .LC55[rip]
	mov	edi, eax
	call	posix_error
	.cfi_endproc
.LFE145:
	.size	Pthread_create, .-Pthread_create
	.section	.rodata.str1.1
.LC56:
	.string	"Pthread_cancel error"
	.text
	.globl	Pthread_cancel
	.type	Pthread_cancel, @function
Pthread_cancel:
.LFB146:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_cancel@PLT
	test	eax, eax
	jne	.L285
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L285:
	.cfi_restore_state
	lea	rsi, .LC56[rip]
	mov	edi, eax
	call	posix_error
	.cfi_endproc
.LFE146:
	.size	Pthread_cancel, .-Pthread_cancel
	.section	.rodata.str1.1
.LC57:
	.string	"Pthread_join error"
	.text
	.globl	Pthread_join
	.type	Pthread_join, @function
Pthread_join:
.LFB147:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_join@PLT
	test	eax, eax
	jne	.L289
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L289:
	.cfi_restore_state
	lea	rsi, .LC57[rip]
	mov	edi, eax
	call	posix_error
	.cfi_endproc
.LFE147:
	.size	Pthread_join, .-Pthread_join
	.section	.rodata.str1.1
.LC58:
	.string	"Pthread_detach error"
	.text
	.globl	Pthread_detach
	.type	Pthread_detach, @function
Pthread_detach:
.LFB148:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_detach@PLT
	test	eax, eax
	jne	.L293
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L293:
	.cfi_restore_state
	lea	rsi, .LC58[rip]
	mov	edi, eax
	call	posix_error
	.cfi_endproc
.LFE148:
	.size	Pthread_detach, .-Pthread_detach
	.globl	Pthread_exit
	.type	Pthread_exit, @function
Pthread_exit:
.LFB149:
	.cfi_startproc
	endbr64
	push	rax
	.cfi_def_cfa_offset 16
	pop	rax
	.cfi_def_cfa_offset 8
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_exit@PLT
	.cfi_endproc
.LFE149:
	.size	Pthread_exit, .-Pthread_exit
	.globl	Pthread_self
	.type	Pthread_self, @function
Pthread_self:
.LFB150:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_self@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE150:
	.size	Pthread_self, .-Pthread_self
	.globl	Pthread_once
	.type	Pthread_once, @function
Pthread_once:
.LFB151:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	pthread_once@PLT
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE151:
	.size	Pthread_once, .-Pthread_once
	.section	.rodata.str1.1
.LC59:
	.string	"Sem_init error"
	.text
	.globl	Sem_init
	.type	Sem_init, @function
Sem_init:
.LFB152:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sem_init@PLT
	test	eax, eax
	js	.L303
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L303:
	.cfi_restore_state
	lea	rdi, .LC59[rip]
	call	unix_error
	.cfi_endproc
.LFE152:
	.size	Sem_init, .-Sem_init
	.section	.rodata.str1.1
.LC60:
	.string	"P error"
	.text
	.globl	P
	.type	P, @function
P:
.LFB153:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sem_wait@PLT
	test	eax, eax
	js	.L307
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L307:
	.cfi_restore_state
	lea	rdi, .LC60[rip]
	call	unix_error
	.cfi_endproc
.LFE153:
	.size	P, .-P
	.section	.rodata.str1.1
.LC61:
	.string	"V error"
	.text
	.globl	V
	.type	V, @function
V:
.LFB154:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	sem_post@PLT
	test	eax, eax
	js	.L311
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L311:
	.cfi_restore_state
	lea	rdi, .LC61[rip]
	call	unix_error
	.cfi_endproc
.LFE154:
	.size	V, .-V
	.globl	rio_readn
	.type	rio_readn, @function
rio_readn:
.LFB155:
	.cfi_startproc
	endbr64
	push	r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	push	r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	push	rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	push	rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	sub	rsp, 8
	.cfi_def_cfa_offset 48
	mov	r12d, edi
	mov	rbp, rsi
	mov	r13, rdx
	mov	rbx, rdx
	jmp	.L313
.L322:
	call	__errno_location@PLT
	cmp	DWORD PTR [rax], 4
	jne	.L321
	mov	eax, 0
.L315:
	sub	rbx, rax
	add	rbp, rax
.L313:
	test	rbx, rbx
	je	.L317
	mov	rdx, rbx
	mov	rsi, rbp
	mov	edi, r12d
	call	read@PLT
	test	rax, rax
	js	.L322
	jne	.L315
.L317:
	mov	rax, r13
	sub	rax, rbx
.L312:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbx
	.cfi_def_cfa_offset 32
	pop	rbp
	.cfi_def_cfa_offset 24
	pop	r12
	.cfi_def_cfa_offset 16
	pop	r13
	.cfi_def_cfa_offset 8
	ret
.L321:
	.cfi_restore_state
	mov	rax, -1
	jmp	.L312
	.cfi_endproc
.LFE155:
	.size	rio_readn, .-rio_readn
	.globl	rio_writen
	.type	rio_writen, @function
rio_writen:
.LFB156:
	.cfi_startproc
	endbr64
	push	r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	push	r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	push	rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	push	rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	sub	rsp, 8
	.cfi_def_cfa_offset 48
	mov	r12d, edi
	mov	rbp, rsi
	mov	r13, rdx
	mov	rbx, rdx
	jmp	.L324
.L325:
	sub	rbx, rax
	add	rbp, rax
.L324:
	test	rbx, rbx
	je	.L330
	mov	rdx, rbx
	mov	rsi, rbp
	mov	edi, r12d
	call	write@PLT
	test	rax, rax
	jg	.L325
	call	__errno_location@PLT
	cmp	DWORD PTR [rax], 4
	jne	.L328
	mov	eax, 0
	jmp	.L325
.L330:
	mov	rax, r13
.L323:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbx
	.cfi_def_cfa_offset 32
	pop	rbp
	.cfi_def_cfa_offset 24
	pop	r12
	.cfi_def_cfa_offset 16
	pop	r13
	.cfi_def_cfa_offset 8
	ret
.L328:
	.cfi_restore_state
	mov	rax, -1
	jmp	.L323
	.cfi_endproc
.LFE156:
	.size	rio_writen, .-rio_writen
	.globl	rio_readinitb
	.type	rio_readinitb, @function
rio_readinitb:
.LFB158:
	.cfi_startproc
	endbr64
	mov	DWORD PTR [rdi], esi
	mov	DWORD PTR 4[rdi], 0
	lea	rax, 16[rdi]
	mov	QWORD PTR 8[rdi], rax
	ret
	.cfi_endproc
.LFE158:
	.size	rio_readinitb, .-rio_readinitb
	.globl	rio_readnb
	.type	rio_readnb, @function
rio_readnb:
.LFB159:
	.cfi_startproc
	endbr64
	push	r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	push	r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	push	rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	push	rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	sub	rsp, 8
	.cfi_def_cfa_offset 48
	mov	r12, rdi
	mov	rbp, rsi
	mov	r13, rdx
	mov	rbx, rdx
	jmp	.L333
.L339:
	sub	rbx, rax
	add	rbp, rax
.L333:
	test	rbx, rbx
	je	.L335
	mov	rdx, rbx
	mov	rsi, rbp
	mov	rdi, r12
	call	rio_read
	test	rax, rax
	js	.L337
	jne	.L339
.L335:
	mov	rax, r13
	sub	rax, rbx
.L332:
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbx
	.cfi_def_cfa_offset 32
	pop	rbp
	.cfi_def_cfa_offset 24
	pop	r12
	.cfi_def_cfa_offset 16
	pop	r13
	.cfi_def_cfa_offset 8
	ret
.L337:
	.cfi_restore_state
	mov	rax, -1
	jmp	.L332
	.cfi_endproc
.LFE159:
	.size	rio_readnb, .-rio_readnb
	.globl	rio_readlineb
	.type	rio_readlineb, @function
rio_readlineb:
.LFB160:
	.cfi_startproc
	endbr64
	push	r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	push	r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	push	rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	push	rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	sub	rsp, 24
	.cfi_def_cfa_offset 64
	mov	r13, rdi
	mov	rbp, rsi
	mov	r12, rdx
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 8[rsp], rax
	xor	eax, eax
	mov	ebx, 1
	jmp	.L341
.L342:
	test	eax, eax
	jne	.L348
	cmp	ebx, 1
	jne	.L344
	mov	eax, 0
	jmp	.L340
.L343:
	add	ebx, 1
	mov	rbp, rdx
.L341:
	movsx	rax, ebx
	cmp	rax, r12
	jnb	.L344
	lea	rsi, 7[rsp]
	mov	edx, 1
	mov	rdi, r13
	call	rio_read
	cmp	eax, 1
	jne	.L342
	lea	rdx, 1[rbp]
	movzx	eax, BYTE PTR 7[rsp]
	mov	BYTE PTR 0[rbp], al
	cmp	al, 10
	jne	.L343
	add	ebx, 1
	mov	rbp, rdx
.L344:
	mov	BYTE PTR 0[rbp], 0
	lea	eax, -1[rbx]
	cdqe
.L340:
	mov	rcx, QWORD PTR 8[rsp]
	sub	rcx, QWORD PTR fs:40
	jne	.L351
	add	rsp, 24
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pop	rbx
	.cfi_def_cfa_offset 32
	pop	rbp
	.cfi_def_cfa_offset 24
	pop	r12
	.cfi_def_cfa_offset 16
	pop	r13
	.cfi_def_cfa_offset 8
	ret
.L348:
	.cfi_restore_state
	mov	rax, -1
	jmp	.L340
.L351:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE160:
	.size	rio_readlineb, .-rio_readlineb
	.section	.rodata.str1.1
.LC62:
	.string	"Rio_readn error"
	.text
	.globl	Rio_readn
	.type	Rio_readn, @function
Rio_readn:
.LFB161:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	rio_readn
	test	rax, rax
	js	.L355
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L355:
	.cfi_restore_state
	lea	rdi, .LC62[rip]
	call	unix_error
	.cfi_endproc
.LFE161:
	.size	Rio_readn, .-Rio_readn
	.section	.rodata.str1.1
.LC63:
	.string	"Rio_writen error"
	.text
	.globl	Rio_writen
	.type	Rio_writen, @function
Rio_writen:
.LFB162:
	.cfi_startproc
	endbr64
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	rbx, rdx
	call	rio_writen
	cmp	rax, rbx
	jne	.L359
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L359:
	.cfi_restore_state
	lea	rdi, .LC63[rip]
	call	unix_error
	.cfi_endproc
.LFE162:
	.size	Rio_writen, .-Rio_writen
	.globl	Rio_readinitb
	.type	Rio_readinitb, @function
Rio_readinitb:
.LFB163:
	.cfi_startproc
	endbr64
	call	rio_readinitb
	ret
	.cfi_endproc
.LFE163:
	.size	Rio_readinitb, .-Rio_readinitb
	.section	.rodata.str1.1
.LC64:
	.string	"Rio_readnb error"
	.text
	.globl	Rio_readnb
	.type	Rio_readnb, @function
Rio_readnb:
.LFB164:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	rio_readnb
	test	rax, rax
	js	.L364
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L364:
	.cfi_restore_state
	lea	rdi, .LC64[rip]
	call	unix_error
	.cfi_endproc
.LFE164:
	.size	Rio_readnb, .-Rio_readnb
	.section	.rodata.str1.1
.LC65:
	.string	"Rio_readlineb error"
	.text
	.globl	Rio_readlineb
	.type	Rio_readlineb, @function
Rio_readlineb:
.LFB165:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	rio_readlineb
	test	rax, rax
	js	.L368
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L368:
	.cfi_restore_state
	lea	rdi, .LC65[rip]
	call	unix_error
	.cfi_endproc
.LFE165:
	.size	Rio_readlineb, .-Rio_readlineb
	.section	.rodata.str1.8
	.align 8
.LC66:
	.string	"getaddrinfo failed (%s:%s): %s\n"
	.align 8
.LC67:
	.string	"open_clientfd: close failed: %s\n"
	.text
	.globl	open_clientfd
	.type	open_clientfd, @function
open_clientfd:
.LFB166:
	.cfi_startproc
	endbr64
	push	r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	push	rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	push	rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	sub	rsp, 80
	.cfi_def_cfa_offset 112
	mov	rbx, rdi
	mov	r12, rsi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 72[rsp], rax
	xor	eax, eax
	lea	rdx, 16[rsp]
	mov	QWORD PTR 16[rsp], 0
	mov	QWORD PTR 24[rsp], 0
	mov	QWORD PTR 32[rsp], 0
	mov	QWORD PTR 40[rsp], 0
	mov	QWORD PTR 48[rsp], 0
	mov	QWORD PTR 56[rsp], 0
	mov	DWORD PTR 24[rsp], 1
	mov	DWORD PTR 16[rsp], 1024
	mov	DWORD PTR 16[rsp], 1056
	lea	rcx, 8[rsp]
	call	getaddrinfo@PLT
	test	eax, eax
	jne	.L378
	mov	rbx, QWORD PTR 8[rsp]
	jmp	.L372
.L378:
	mov	edi, eax
	call	gai_strerror@PLT
	mov	r9, rax
	mov	r8, r12
	mov	rcx, rbx
	lea	rdx, .LC66[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	ebp, -2
	jmp	.L369
.L373:
	mov	rbx, QWORD PTR 40[rbx]
.L372:
	test	rbx, rbx
	je	.L374
	mov	edx, DWORD PTR 12[rbx]
	mov	esi, DWORD PTR 8[rbx]
	mov	edi, DWORD PTR 4[rbx]
	call	socket@PLT
	mov	ebp, eax
	test	eax, eax
	js	.L373
	mov	rsi, QWORD PTR 24[rbx]
	mov	edx, DWORD PTR 16[rbx]
	mov	edi, eax
	call	connect@PLT
	mov	r12d, eax
	cmp	eax, -1
	jne	.L374
	mov	edi, ebp
	call	close@PLT
	test	eax, eax
	jns	.L373
	call	__errno_location@PLT
	mov	edi, DWORD PTR [rax]
	call	strerror@PLT
	mov	rcx, rax
	lea	rdx, .LC67[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	ebp, r12d
	jmp	.L369
.L374:
	mov	rdi, QWORD PTR 8[rsp]
	call	freeaddrinfo@PLT
	test	rbx, rbx
	je	.L379
.L369:
	mov	rax, QWORD PTR 72[rsp]
	sub	rax, QWORD PTR fs:40
	jne	.L380
	mov	eax, ebp
	add	rsp, 80
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	pop	rbx
	.cfi_def_cfa_offset 24
	pop	rbp
	.cfi_def_cfa_offset 16
	pop	r12
	.cfi_def_cfa_offset 8
	ret
.L379:
	.cfi_restore_state
	mov	ebp, -1
	jmp	.L369
.L380:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE166:
	.size	open_clientfd, .-open_clientfd
	.section	.rodata.str1.8
	.align 8
.LC68:
	.string	"getaddrinfo failed (port %s): %s\n"
	.align 8
.LC69:
	.string	"open_listenfd close failed: %s\n"
	.text
	.globl	open_listenfd
	.type	open_listenfd, @function
open_listenfd:
.LFB167:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	sub	rsp, 88
	.cfi_def_cfa_offset 112
	mov	rbx, rdi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR 72[rsp], rax
	xor	eax, eax
	mov	DWORD PTR 4[rsp], 1
	lea	rdx, 16[rsp]
	mov	QWORD PTR 16[rsp], 0
	mov	QWORD PTR 24[rsp], 0
	mov	QWORD PTR 32[rsp], 0
	mov	QWORD PTR 40[rsp], 0
	mov	QWORD PTR 48[rsp], 0
	mov	QWORD PTR 56[rsp], 0
	mov	DWORD PTR 24[rsp], 1
	mov	DWORD PTR 16[rsp], 33
	mov	DWORD PTR 16[rsp], 1057
	lea	rcx, 8[rsp]
	mov	rsi, rdi
	mov	edi, 0
	call	getaddrinfo@PLT
	test	eax, eax
	jne	.L391
	mov	rbx, QWORD PTR 8[rsp]
	jmp	.L384
.L391:
	mov	edi, eax
	call	gai_strerror@PLT
	mov	r8, rax
	mov	rcx, rbx
	lea	rdx, .LC68[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	ebp, -2
	jmp	.L381
.L385:
	mov	rbx, QWORD PTR 40[rbx]
.L384:
	test	rbx, rbx
	je	.L386
	mov	edx, DWORD PTR 12[rbx]
	mov	esi, DWORD PTR 8[rbx]
	mov	edi, DWORD PTR 4[rbx]
	call	socket@PLT
	mov	ebp, eax
	test	eax, eax
	js	.L385
	lea	rcx, 4[rsp]
	mov	r8d, 4
	mov	edx, 2
	mov	esi, 1
	mov	edi, eax
	call	setsockopt@PLT
	mov	rsi, QWORD PTR 24[rbx]
	mov	edx, DWORD PTR 16[rbx]
	mov	edi, ebp
	call	bind@PLT
	test	eax, eax
	je	.L386
	mov	edi, ebp
	call	close@PLT
	test	eax, eax
	jns	.L385
	call	__errno_location@PLT
	mov	edi, DWORD PTR [rax]
	call	strerror@PLT
	mov	rcx, rax
	lea	rdx, .LC69[rip]
	mov	esi, 1
	mov	rdi, QWORD PTR stderr[rip]
	mov	eax, 0
	call	__fprintf_chk@PLT
	mov	ebp, -1
	jmp	.L381
.L386:
	mov	rdi, QWORD PTR 8[rsp]
	call	freeaddrinfo@PLT
	test	rbx, rbx
	je	.L389
	mov	esi, 1024
	mov	edi, ebp
	call	listen@PLT
	test	eax, eax
	js	.L392
.L381:
	mov	rax, QWORD PTR 72[rsp]
	sub	rax, QWORD PTR fs:40
	jne	.L393
	mov	eax, ebp
	add	rsp, 88
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L392:
	.cfi_restore_state
	mov	edi, ebp
	call	close@PLT
	mov	ebp, -1
	jmp	.L381
.L389:
	mov	ebp, -1
	jmp	.L381
.L393:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE167:
	.size	open_listenfd, .-open_listenfd
	.section	.rodata.str1.1
.LC70:
	.string	"Open_clientfd error"
	.text
	.globl	Open_clientfd
	.type	Open_clientfd, @function
Open_clientfd:
.LFB168:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	open_clientfd
	test	eax, eax
	js	.L397
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L397:
	.cfi_restore_state
	lea	rdi, .LC70[rip]
	call	unix_error
	.cfi_endproc
.LFE168:
	.size	Open_clientfd, .-Open_clientfd
	.section	.rodata.str1.1
.LC71:
	.string	"Open_listenfd error"
	.text
	.globl	Open_listenfd
	.type	Open_listenfd, @function
Open_listenfd:
.LFB169:
	.cfi_startproc
	endbr64
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	call	open_listenfd
	test	eax, eax
	js	.L401
	add	rsp, 8
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L401:
	.cfi_restore_state
	lea	rdi, .LC71[rip]
	call	unix_error
	.cfi_endproc
.LFE169:
	.size	Open_listenfd, .-Open_listenfd
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
