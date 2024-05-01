	.file	"temp.c"
	.text
	.section	.rodata
.LC0:
	.string	"/bin/bash"
.LC1:
	.string	"-c"
.LC2:
	.string	"echo Hello World"
.LC3:
	.string	"ENV=World"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	$.LC0, -8(%rbp)
	movq	$.LC0, -48(%rbp)
	movq	$.LC1, -40(%rbp)
	movq	$.LC2, -32(%rbp)
	movq	$0, -24(%rbp)
	movq	$.LC3, -64(%rbp)
	movq	$0, -56(%rbp)
	leaq	-48(%rbp), %rcx
	movq	-8(%rbp), %rax
	movl	$0, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	execve
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 13.2.1 20240316 (Red Hat 13.2.1-7)"
	.section	.note.GNU-stack,"",@progbits
