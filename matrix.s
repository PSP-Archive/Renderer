	.file	1 "matrix.c"
	.section .mdebug.eabi32
	.section .gcc_compiled_long32
	.previous
	.section	.rodata.cst4,"aM",@progbits,4
	.align	2
$LC0:
	.word	1065353216
	.text
	.align	2
	.globl	matrix_create_identity
	.ent	matrix_create_identity
matrix_create_identity:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi($LC0)
	lwc1	$f0,%lo($LC0)($2)
	swc1	$f0,0($4)
	swc1	$f0,60($4)
	swc1	$f0,40($4)
	j	$31
	swc1	$f0,20($4)

	.set	macro
	.set	reorder
	.end	matrix_create_identity
	.size	matrix_create_identity, .-matrix_create_identity
	.align	2
	.globl	matrix_translate
	.ent	matrix_translate
matrix_translate:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lwc1	$f0,48($4)
	lwc1	$f1,52($4)
	lwc1	$f2,56($4)
	add.s	$f0,$f0,$f12
	add.s	$f1,$f1,$f13
	add.s	$f2,$f2,$f14
	swc1	$f0,48($4)
	swc1	$f1,52($4)
	j	$31
	swc1	$f2,56($4)

	.set	macro
	.set	reorder
	.end	matrix_translate
	.size	matrix_translate, .-matrix_translate
	.align	2
	.globl	matrix_add
	.ent	matrix_add
matrix_add:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	move	$2,$0
	li	$3,16			# 0x10
$L6:
	lwc1	$f0,0($4)
	lwc1	$f1,0($5)
	addiu	$2,$2,1
	addiu	$5,$5,4
	add.s	$f0,$f0,$f1
	swc1	$f0,0($4)
	bne	$2,$3,$L6
	addiu	$4,$4,4

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	matrix_add
	.size	matrix_add, .-matrix_add
	.align	2
	.globl	matrix_multiplyS
	.ent	matrix_multiplyS
matrix_multiplyS:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	move	$2,$0
	li	$3,16			# 0x10
$L13:
	lwc1	$f0,0($4)
	addiu	$2,$2,1
	mul.s	$f0,$f0,$f12
	swc1	$f0,0($4)
	bne	$2,$3,$L13
	addiu	$4,$4,4

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	matrix_multiplyS
	.size	matrix_multiplyS, .-matrix_multiplyS
	.align	2
	.globl	matrix_normalizeV
	.ent	matrix_normalizeV
matrix_normalizeV:
	.frame	$sp,8,$31		# vars= 0, regs= 2/0, args= 0, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$16,0($sp)
	sw	$31,4($sp)
	lwc1	$f0,0($4)
	lwc1	$f1,4($4)
	lwc1	$f2,8($4)
	mul.s	$f0,$f0,$f0
	mul.s	$f1,$f1,$f1
	mul.s	$f2,$f2,$f2
	move	$16,$4
	add.s	$f0,$f0,$f1
	mtc1	$0,$f1
	add.s	$f0,$f0,$f2
	c.eq.s	$f0,$f1
	nop
	bc1f	$L23
	mov.s	$f12,$f0

	lw	$31,4($sp)
	lw	$16,0($sp)
	j	$31
	addiu	$sp,$sp,8

$L23:
	jal	sqrt
	nop

	lwc1	$f1,0($16)
	lwc1	$f2,4($16)
	lwc1	$f3,8($16)
	div.s	$f1,$f1,$f0
	div.s	$f2,$f2,$f0
	div.s	$f3,$f3,$f0
	swc1	$f1,0($16)
	swc1	$f2,4($16)
	swc1	$f3,8($16)
	lw	$31,4($sp)
	lw	$16,0($sp)
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	matrix_normalizeV
	.size	matrix_normalizeV, .-matrix_normalizeV
	.align	2
	.globl	matrix_dotproduct
	.ent	matrix_dotproduct
matrix_dotproduct:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lwc1	$f1,0($5)
	lwc1	$f3,4($5)
	lwc1	$f0,0($4)
	lwc1	$f4,4($4)
	lwc1	$f2,8($4)
	mul.s	$f0,$f0,$f1
	mul.s	$f4,$f4,$f3
	lwc1	$f5,8($5)
	lwc1	$f1,12($4)
	lwc1	$f3,12($5)
	mul.s	$f2,$f2,$f5
	mul.s	$f1,$f1,$f3
	add.s	$f0,$f0,$f4
	add.s	$f0,$f0,$f2
	j	$31
	add.s	$f0,$f0,$f1

	.set	macro
	.set	reorder
	.end	matrix_dotproduct
	.size	matrix_dotproduct, .-matrix_dotproduct
	.align	2
	.globl	matrix_crossproduct
	.ent	matrix_crossproduct
matrix_crossproduct:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lwc1	$f1,8($6)
	lwc1	$f3,4($6)
	lwc1	$f2,4($5)
	lwc1	$f0,8($5)
	mul.s	$f2,$f2,$f1
	mul.s	$f0,$f0,$f3
	sub.s	$f2,$f2,$f0
	swc1	$f2,0($4)
	lwc1	$f3,0($6)
	lwc1	$f2,8($6)
	lwc1	$f1,8($5)
	lwc1	$f0,0($5)
	mul.s	$f1,$f1,$f3
	mul.s	$f0,$f0,$f2
	sub.s	$f1,$f1,$f0
	swc1	$f1,4($4)
	lwc1	$f2,4($5)
	lwc1	$f3,0($6)
	lwc1	$f0,0($5)
	lwc1	$f1,4($6)
	mul.s	$f2,$f2,$f3
	mul.s	$f0,$f0,$f1
	sub.s	$f0,$f0,$f2
	j	$31
	swc1	$f0,8($4)

	.set	macro
	.set	reorder
	.end	matrix_crossproduct
	.size	matrix_crossproduct, .-matrix_crossproduct
	.align	2
	.globl	matrix_normal
	.ent	matrix_normal
matrix_normal:
	.frame	$sp,40,$31		# vars= 24, regs= 3/0, args= 0, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lwc1	$f8,0($5)
	lwc1	$f6,4($5)
	lwc1	$f7,8($5)
	lwc1	$f5,0($6)
	lwc1	$f3,4($6)
	lwc1	$f4,8($6)
	lwc1	$f2,0($7)
	lwc1	$f0,4($7)
	lwc1	$f1,8($7)
	sub.s	$f5,$f2,$f5
	sub.s	$f3,$f0,$f3
	sub.s	$f4,$f1,$f4
	sub.s	$f0,$f0,$f6
	sub.s	$f1,$f1,$f7
	sub.s	$f2,$f2,$f8
	addiu	$16,$sp,12
	move	$17,$4
	move	$4,$sp
	swc1	$f2,0($sp)
	swc1	$f0,4($sp)
	swc1	$f1,8($sp)
	swc1	$f5,12($sp)
	swc1	$f3,16($sp)
	jal	matrix_normalizeV
	swc1	$f4,20($sp)

	jal	matrix_normalizeV
	move	$4,$16

	move	$4,$17
	move	$5,$sp
	jal	matrix_crossproduct
	move	$6,$16

	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	matrix_normal
	.size	matrix_normal, .-matrix_normal
	.align	2
	.globl	matrix_multiplyV
	.ent	matrix_multiplyV
matrix_multiplyV:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lwc1	$f3,0($6)
	lwc1	$f6,4($6)
	lwc1	$f1,0($5)
	lwc1	$f2,16($5)
	lwc1	$f5,8($6)
	mul.s	$f1,$f3,$f1
	mul.s	$f2,$f6,$f2
	lwc1	$f0,32($5)
	lwc1	$f4,48($5)
	mul.s	$f0,$f5,$f0
	add.s	$f1,$f1,$f2
	add.s	$f1,$f1,$f0
	add.s	$f1,$f1,$f4
	swc1	$f1,0($4)
	lwc1	$f0,4($5)
	lwc1	$f1,20($5)
	lwc1	$f2,36($5)
	mul.s	$f0,$f3,$f0
	mul.s	$f1,$f6,$f1
	mul.s	$f2,$f5,$f2
	lwc1	$f4,52($5)
	add.s	$f0,$f0,$f1
	add.s	$f0,$f0,$f2
	add.s	$f0,$f0,$f4
	swc1	$f0,4($4)
	lwc1	$f0,24($5)
	lwc1	$f1,8($5)
	lwc1	$f2,40($5)
	mul.s	$f6,$f6,$f0
	mul.s	$f3,$f3,$f1
	mul.s	$f5,$f5,$f2
	lwc1	$f0,56($5)
	add.s	$f3,$f3,$f6
	add.s	$f3,$f3,$f5
	add.s	$f3,$f3,$f0
	j	$31
	swc1	$f3,8($4)

	.set	macro
	.set	reorder
	.end	matrix_multiplyV
	.size	matrix_multiplyV, .-matrix_multiplyV
	.align	2
	.globl	matrix_rotate
	.ent	matrix_rotate
matrix_rotate:
	.frame	$sp,24,$31		# vars= 0, regs= 4/2, args= 0, gp= 0
	.mask	0x80070000,-12
	.fmask	0x00300000,-4
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	trunc.w.s $f0,$f12
	sw	$18,8($sp)
	sw	$16,0($sp)
	move	$18,$4
	mfc1	$16,$f0
	mfc1	$4,$f0
	sw	$31,12($sp)
	sw	$17,4($sp)
	swc1	$f21,20($sp)
	swc1	$f20,16($sp)
	move	$17,$5
	jal	sin
	mov.s	$f20,$f12

	move	$4,$16
	jal	cos
	mov.s	$f21,$f0

	mov.s	$f1,$f0
	mtc1	$0,$f0
	nop
	c.eq.s	$f20,$f0
	nop
	bc1t	$L39
	li	$2,1			# 0x1

	beq	$17,$2,$L37
	neg.s	$f0,$f21

	li	$2,2			# 0x2
	beql	$17,$2,$L38
	swc1	$f0,16($18)

	bne	$17,$0,$L40
	lw	$31,12($sp)

	swc1	$f1,40($18)
	swc1	$f0,36($18)
	swc1	$f1,20($18)
	swc1	$f21,24($18)
$L39:
	lw	$31,12($sp)
$L40:
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	lwc1	$f21,20($sp)
	lwc1	$f20,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L37:
	swc1	$f21,32($18)
	swc1	$f1,40($18)
	swc1	$f0,8($18)
	swc1	$f1,0($18)
	lw	$31,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	lwc1	$f21,20($sp)
	lwc1	$f20,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L38:
	swc1	$f1,20($18)
	swc1	$f1,0($18)
	swc1	$f21,4($18)
	lw	$31,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	lwc1	$f21,20($sp)
	lwc1	$f20,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	matrix_rotate
	.size	matrix_rotate, .-matrix_rotate
	.section	.rodata.cst4
	.align	2
$LC1:
	.word	1065353216
	.text
	.align	2
	.globl	matrix_rotate_around_axis
	.ent	matrix_rotate_around_axis
matrix_rotate_around_axis:
	.frame	$sp,24,$31		# vars= 0, regs= 4/1, args= 0, gp= 0
	.mask	0x80070000,-12
	.fmask	0x00100000,-8
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	trunc.w.s $f12,$f12
	sw	$18,8($sp)
	sw	$17,4($sp)
	move	$18,$4
	mfc1	$17,$f12
	mfc1	$4,$f12
	sw	$31,12($sp)
	sw	$16,0($sp)
	swc1	$f20,16($sp)
	jal	cos
	move	$16,$5

	move	$4,$17
	jal	sin
	mov.s	$f20,$f0

	lui	$2,%hi($LC1)
	lwc1	$f4,%lo($LC1)($2)
	lwc1	$f2,0($16)
	sub.s	$f4,$f4,$f20
	mul.s	$f1,$f4,$f2
	mul.s	$f2,$f2,$f1
	add.s	$f2,$f20,$f2
	swc1	$f2,0($18)
	lwc1	$f1,0($16)
	lwc1	$f3,4($16)
	lwc1	$f2,8($16)
	mul.s	$f1,$f4,$f1
	mul.s	$f2,$f0,$f2
	mul.s	$f1,$f1,$f3
	sub.s	$f1,$f1,$f2
	swc1	$f1,4($18)
	lwc1	$f2,0($16)
	lwc1	$f3,8($16)
	lwc1	$f1,4($16)
	mul.s	$f2,$f4,$f2
	mul.s	$f1,$f0,$f1
	mul.s	$f2,$f2,$f3
	add.s	$f2,$f2,$f1
	swc1	$f2,8($18)
	lwc1	$f3,0($16)
	lwc1	$f2,4($16)
	lwc1	$f1,8($16)
	mul.s	$f3,$f4,$f3
	mul.s	$f1,$f0,$f1
	mul.s	$f3,$f3,$f2
	add.s	$f3,$f3,$f1
	swc1	$f3,16($18)
	lwc1	$f1,4($16)
	mul.s	$f2,$f4,$f1
	mul.s	$f1,$f1,$f2
	add.s	$f1,$f20,$f1
	swc1	$f1,20($18)
	lwc1	$f2,4($16)
	lwc1	$f3,8($16)
	lwc1	$f1,0($16)
	mul.s	$f2,$f4,$f2
	mul.s	$f1,$f0,$f1
	mul.s	$f2,$f2,$f3
	sub.s	$f2,$f2,$f1
	swc1	$f2,24($18)
	lwc1	$f1,0($16)
	lwc1	$f3,8($16)
	lwc1	$f2,4($16)
	mul.s	$f1,$f4,$f1
	mul.s	$f2,$f0,$f2
	mul.s	$f1,$f1,$f3
	sub.s	$f1,$f1,$f2
	swc1	$f1,32($18)
	lwc1	$f2,4($16)
	lwc1	$f3,8($16)
	lwc1	$f1,0($16)
	mul.s	$f2,$f4,$f2
	mul.s	$f0,$f0,$f1
	mul.s	$f2,$f2,$f3
	add.s	$f2,$f2,$f0
	swc1	$f2,36($18)
	lwc1	$f0,8($16)
	mul.s	$f4,$f4,$f0
	mul.s	$f0,$f0,$f4
	add.s	$f20,$f20,$f0
	swc1	$f20,40($18)
	lw	$31,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	lwc1	$f20,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	matrix_rotate_around_axis
	.size	matrix_rotate_around_axis, .-matrix_rotate_around_axis
	.align	2
	.globl	matrix_multiplyM
	.ent	matrix_multiplyM
matrix_multiplyM:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	move	$8,$0
	li	$9,4			# 0x4
	li	$10,4			# 0x4
$L44:
	lwc1	$f7,0($6)
	lwc1	$f6,4($6)
	lwc1	$f5,8($6)
	lwc1	$f4,12($6)
	move	$2,$5
	move	$3,$4
	move	$7,$0
$L45:
	lwc1	$f0,0($2)
	lwc1	$f2,16($2)
	lwc1	$f3,32($2)
	mul.s	$f0,$f7,$f0
	mul.s	$f2,$f6,$f2
	mul.s	$f3,$f5,$f3
	lwc1	$f1,48($2)
	addiu	$7,$7,1
	addiu	$2,$2,4
	mul.s	$f1,$f4,$f1
	add.s	$f0,$f0,$f2
	add.s	$f0,$f0,$f3
	add.s	$f0,$f0,$f1
	swc1	$f0,0($3)
	bne	$7,$9,$L45
	addiu	$3,$3,4

	addiu	$8,$8,1
	addiu	$6,$6,16
	bne	$8,$10,$L44
	addiu	$4,$4,16

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	matrix_multiplyM
	.size	matrix_multiplyM, .-matrix_multiplyM
	.rdata
	.align	2
	.type	C.6.2032, @object
	.size	C.6.2032, 64
C.6.2032:
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.align	2
	.type	C.5.2031, @object
	.size	C.5.2031, 64
C.5.2031:
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.align	2
	.type	C.4.2030, @object
	.size	C.4.2030, 64
C.4.2030:
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1065353216
	.text
	.align	2
	.globl	matrix_rotate_point
	.ent	matrix_rotate_point
matrix_rotate_point:
	.frame	$sp,312,$31		# vars= 272, regs= 6/3, args= 0, gp= 0
	.mask	0x801f0000,-20
	.fmask	0x00700000,-8
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-312
	lui	$2,%hi(C.4.2030)
	sw	$19,284($sp)
	swc1	$f22,304($sp)
	swc1	$f21,300($sp)
	addiu	$6,$2,%lo(C.4.2030)
	addiu	$19,$sp,12
	mov.s	$f21,$f13
	mov.s	$f22,$f14
	sw	$17,276($sp)
	sw	$31,292($sp)
	sw	$20,288($sp)
	sw	$18,280($sp)
	sw	$16,272($sp)
	swc1	$f20,296($sp)
	move	$17,$4
	move	$7,$19
	addiu	$8,$6,64
$L54:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L54
	addiu	$7,$7,16

	lui	$2,%hi(C.5.2031)
	addiu	$6,$2,%lo(C.5.2031)
	addiu	$20,$sp,76
	move	$7,$20
	addiu	$8,$6,64
$L55:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L55
	addiu	$7,$7,16

	lui	$2,%hi(C.6.2032)
	addiu	$6,$2,%lo(C.6.2032)
	addiu	$18,$sp,140
	move	$7,$18
	addiu	$8,$6,64
$L56:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L56
	addiu	$7,$7,16

	mtc1	$0,$f20
	nop
	c.eq.s	$f12,$f20
	nop
	bc1f	$L73
	move	$4,$19

$L57:
	c.eq.s	$f21,$f20
	nop
	bc1f	$L74
	mov.s	$f12,$f21

$L60:
	c.eq.s	$f22,$f20
	nop
	bc1t	$L75
	addiu	$16,$sp,204

	mov.s	$f12,$f22
	move	$4,$18
	jal	matrix_rotate
	li	$5,2			# 0x2

	addiu	$16,$sp,204
$L75:
	move	$6,$20
	move	$4,$16
	jal	matrix_multiplyM
	move	$5,$19

	move	$4,$19
	move	$5,$16
	jal	matrix_multiplyM
	move	$6,$18

	move	$4,$sp
	move	$5,$19
	jal	matrix_multiplyV
	move	$6,$17

	lwc1	$f1,4($sp)
	lwc1	$f2,8($sp)
	lwc1	$f0,0($sp)
	swc1	$f1,4($17)
	swc1	$f2,8($17)
	swc1	$f0,0($17)
	lw	$31,292($sp)
	lw	$20,288($sp)
	lw	$19,284($sp)
	lw	$18,280($sp)
	lw	$17,276($sp)
	lw	$16,272($sp)
	lwc1	$f22,304($sp)
	lwc1	$f21,300($sp)
	lwc1	$f20,296($sp)
	j	$31
	addiu	$sp,$sp,312

$L74:
	move	$4,$20
	jal	matrix_rotate
	li	$5,1			# 0x1

	j	$L60
	nop

$L73:
	jal	matrix_rotate
	move	$5,$0

	j	$L57
	nop

	.set	macro
	.set	reorder
	.end	matrix_rotate_point
	.size	matrix_rotate_point, .-matrix_rotate_point
	.ident	"GCC: (GNU) 4.0.0"
