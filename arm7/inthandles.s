# 
# interrupt enable/disable

# Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

    .equ    Mode_USR,   0x10
    .equ    Mode_FIQ,   0x11
    .equ    Mode_IRQ,   0x12
    .equ    Mode_SVC,   0x13
    .equ    Mode_ABT,   0x17
    .equ    Mode_UND,   0x1B
    .equ    Mode_SYS,   0x1F

    .equ    I_BIT, 0x80        /* when I bit is set, IRQ is disabled */
    .equ    F_BIT, 0x40        /* when F bit is set, FIQ is disabled */


       .text
# generate arm instructions (.code32)
	.arm
	.global _disableInterrupts	
	.global _enableInterrupts

# -------------------------------------------------------------
# enable IRQ interrupts 
# -------------------------------------------------------------
	.func   _enableInterrupts
_enableInterrupts:
	stmfd	sp!, {r1}
	mrs		r1, cpsr
	bic		r1, r1, #0x80
	msr		cpsr_c, r1
	ldmfd	sp!, {r1}
	bx 		lr
	.size   _enableInterrupts, . - _enableInterrupts
	.endfunc

# -------------------------------------------------------------
# disable IRQ interrupts 
# -------------------------------------------------------------
	.func   _disableInterrupts

_disableInterrupts:	
	stmfd	sp!, {r1}
	mrs		r1, CPSR
	orr		r1, r1, #0x80
	msr		CPSR_c, r1
	ldmfd	sp!, {r1}
	bx		lr
	.size   _disableInterrupts, . - _disableInterrupts
	.endfunc

	.end
