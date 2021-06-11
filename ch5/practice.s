	.file	"practice.c"
	.text
	.globl	poly
	.type	poly, @function


poly:
# double poly(double a[], double x, long degree)
# a in rdi, x in xmm0, degree in rsi
.LFB11:
	.cfi_startproc
	movsd	(%rdi), %xmm2          # xmm2 = result = a[0]
	movapd	%xmm0, %xmm1           # xmm1 = xpwr
	movl	$1, %eax               # rax = i
.L2:
	cmpq	%rsi, %rax          
	jg	.L4
	movapd	%xmm1, %xmm3           # xmm3 = xpwr
	mulsd	(%rdi,%rax,8), %xmm3   # a[i] * xpwr
	addsd	%xmm3, %xmm2           # result += a[i] * xpwr
	mulsd	%xmm0, %xmm1           # xpwr *= x
	addq	$1, %rax               # i++
	jmp	.L2
.L4:
	movapd	%xmm2, %xmm0           
	ret
	.cfi_endproc




.LFE11:
	.size	poly, .-poly
	.globl	polyh
	.type	polyh, @function


polyh:
.LFB12:
	.cfi_startproc
	movsd	(%rdi,%rsi,8), %xmm1
	subq	$1, %rsi
.L6:
	testq	%rsi, %rsi
	js	.L8
	mulsd	%xmm0, %xmm1
	addsd	(%rdi,%rsi,8), %xmm1
	subq	$1, %rsi
	jmp	.L6
.L8:
	movapd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE12:
	.size	polyh, .-polyh
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC6:
	.string	"poly result: %f\npolyh result: %f\n"
	.text
	.globl	main
	.type	main, @function


main:
.LFB13:
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movq	.LC0(%rip), %rax
	movq	%rax, 16(%rsp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, 24(%rsp)
	movq	.LC2(%rip), %rax
	movq	%rax, 32(%rsp)
	movsd	%xmm0, 40(%rsp)
	movq	.LC3(%rip), %rax
	movq	%rax, 48(%rsp)
	movq	.LC4(%rip), %rax
	movq	%rax, 56(%rsp)

	movl	$5, %esi
	movsd	.LC5(%rip), %xmm0
	leaq	16(%rsp), %rdi
	call	poly

	movsd	%xmm0, 8(%rsp)
	movl	$5, %esi
	movsd	.LC5(%rip), %xmm0
	leaq	16(%rsp), %rdi
	call	polyh

	movapd	%xmm0, %xmm1
	movsd	8(%rsp), %xmm0
	movl	$.LC6, %edi
	movl	$2, %eax
	call	printf
	movl	$0, %eax
	addq	$72, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1074266112
	.align 8
.LC1:
	.long	0
	.long	1072693248
	.align 8
.LC2:
	.long	0
	.long	1074790400
	.align 8
.LC3:
	.long	0
	.long	1075052544
	.align 8
.LC4:
	.long	0
	.long	1075970048
	.align 8
.LC5:
	.long	1374389535
	.long	1074339512
	.ident	"GCC: (GNU) 8.3.0"
