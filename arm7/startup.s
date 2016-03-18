# *** Startup Code (executed after Reset) ***

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


# System Memory definitions

#  Internal RAM definitions
								
        .equ    RAM_Size,      0x00004000 	/* 32k by AT91SAM7X128 */
        									/* at 0x204000 used for flash programming */
        .equ    RAM_Base,      0x00200000	/* no remap! */	

# Stack definitions

        .equ    UND_Stack_Size,     1*4
        .equ    SVC_Stack_Size,   128*4
        .equ    ABT_Stack_Size,     1*4
        .equ    FIQ_Stack_Size,    32*4
        .equ    IRQ_Stack_Size,   128*4
        .equ    USR_Stack_Size,    32*4

# AIC Configuration definitions
		.equ	AIC_BASE,	0xFFFFF000	/* base address AIC controller  */ 
		.equ	AIC_IDCR,	0x124		/* AIC_IDCR register **/
		.equ	AIC_IVR, 	0x100		/* AIC_IVR register */	

# Power Mangement Controller (PMC) definitions
        .equ    PMC_SETUP,      1

        .equ    PMC_BASE,       0xFFFFFC00  /* PMC Base Address */
        .equ    PMC_MOR,        0x20        /* PMC_MOR Offset */
        .equ    PMC_MCFR,       0x24        /* PMC_MCFR Offset */
        .equ    PMC_PLLR,       0x2C        /* PMC_PLLR Offset */
        .equ    PMC_MCKR,       0x30        /* PMC_MCKR Offset */
        .equ    PMC_SR,         0x68        /* PMC_SR Offset */
        .equ    PMC_MOSCEN,     (1<<0)      /* Main Oscillator Enable */
        .equ    PMC_OSCBYPASS,  (1<<1)      /* Main Oscillator Bypass */
        .equ    PMC_OSCOUNT,    (0xFF<<8)   /* Main OScillator Start-up Time */
        .equ    PMC_DIV,        (0xFF<<0)   /* PLL Divider */
        .equ    PMC_PLLCOUNT,   (0x3F<<8)   /* PLL Lock Counter */
        .equ    PMC_OUT,        (0x03<<14)  /* PLL Clock Frequency Range */
        .equ    PMC_MUL,        (0x7FF<<16) /* PLL Multiplier */
        .equ    PMC_USBDIV,     (0x03<<28)  /* USB Clock Divider */
        .equ    PMC_CSS,        (3<<0)      /* Clock Source Selection */
        .equ    PMC_PRES,       (7<<2)      /* Prescaler Selection */
        .equ    PMC_MOSCS,      (1<<0)      /* Main Oscillator Stable */
        .equ    PMC_LOCK,       (1<<2)      /* PLL Lock Status */
        .equ    PMC_MOR_Val,    0x00000601	/* main oscillator enabled after 48 clocks */
        .equ    PMC_PLLR_Val,   0x00191A05	/* PLL out = 18.432*26/5 = 95,486 MHz */
        .equ    PMC_MCKR_Val,   0x00000007	/* Master clock = PLL/2 = 47,923MHz	*/


# Watchdog Timer (WDT) definitions
        .equ    WDT_BASE,       0xFFFFFD40  /* WDT Base Address */
        .equ    WDT_MR,         0x04        /* WDT_MR Offset */
        .equ    WDT_MR_Val,     0x00008000

# Embedded Flash Controller (EFC) definitions
        .equ    EFC_SETUP,      1

        .equ    EFC_BASE,       0xFFFFFF00  /* EFC Base Address */
        .equ    EFC_FMR,        0x60        /* EFC_FMR Offset */
        .equ    EFC_FMR_Val,    0x00320100

    	.equ    Top_Stack,     RAM_Base + RAM_Size

# Startup Code must be linked first at Address at which it expects to run.

        .text
		.arm
		.extern main
		.global _app_entry
        .global _startup
        .func   _startup

_startup:
ENTRY:

# Exception Vectors

Vectors:
        LDR     PC, Reset_Addr 		/* 0x0000 */        
        LDR     PC, Undef_Addr 		/* 0x0004 */        
        LDR     PC, SWI_Addr 		/* 0x0008 */        
        LDR     PC, PAbt_Addr 		/* 0x000C */        
        LDR     PC, DAbt_Addr 		/* 0x0010 */        
resrvd: B       resrvd				/* 0x0014 Reserved Vector */
		LDR     PC,IRQ_Addr
#       LDR     PC,[PC,#-0xF20]     /* 0x0018 Vector from AIC_IVR	*/
#       LDR     PC,FIQ_Addr
        LDR     PC,[PC,#-0xF20]     /* 0x001C Vector from AIC_IVR	*/

Reset_Addr:     .word   Hard_Reset
Undef_Addr:     .word   Undef_Handler
SWI_Addr:       .word   SWI_Handler
PAbt_Addr:      .word   PAbt_Handler
DAbt_Addr:      .word   DAbt_Handler
			    .word   0                      /* Reserved Address */
IRQ_Addr:       .word   IRQ_Handler
FIQ_Addr:       .word   FIQ_Handler

Undef_Handler:  B       Undef_Handler
SWI_Handler:    B       SWI_Handler
PAbt_Handler:   B       PAbt_Handler
DAbt_Handler:   B       DAbt_Handler
#IRQ_Handler:	B		IRQ_Handler 		/* should never get here ... */
FIQ_Handler:    B       FIQ_Handler

AIC_BASE_Adr: 	.word 	AIC_BASE
FFFFFFFFh:		.word	0xFFFFFFFF
DelayMax:		.word	0x1000

# Reset Handler

Hard_Reset:  
_app_entry:
									/* 1st of all: disable interrupts... */
		mov 	r0, #FFFFFFFFh /* 17-03-2016. TODO: discover why this <Immediate Value> is not #FFFFFFFF  */

		ldr 	r2, AIC_BASE_Adr
		str 	r0, [r2, #AIC_IDCR]	/* disable interrupts globally */

# Setup WDT
        LDR     R0, =WDT_BASE
        LDR     R1, =WDT_MR_Val
        STR     R1, [R0, #WDT_MR]

# Delay for emulator
		ldr 	r1, DelayMax 
		mov 	r0, #0
DelayLoop:      
		CMP 	r0, r1
		add 	r0, r0, #1
	    BLO 	DelayLoop

# Setup EFC
        LDR     R0, =EFC_BASE
        LDR     R1, =EFC_FMR_Val
        STR     R1, [R0, #EFC_FMR]

# Setup PMC
.if PMC_SETUP
        LDR     R0, =PMC_BASE

#  Setup Main Oscillator
        LDR     R1, =PMC_MOR_Val
        STR     R1, [R0, #PMC_MOR]

#  Wait until Main Oscillator is stablilized
.if (PMC_MOR_Val & PMC_MOSCEN)
MOSCS_Loop:     
		LDR     R2, [R0, #PMC_SR]
        ANDS    R2, R2, #PMC_MOSCS
        BEQ     MOSCS_Loop
.endif

#  Setup the PLL to 95,486 MHz
.if (PMC_PLLR_Val & PMC_MUL)
        LDR     R1, =PMC_PLLR_Val
        STR     R1, [R0, #PMC_PLLR]

#  Wait until PLL is stabilized
PLL_Loop:
		LDR     R2, [R0, #PMC_SR]
        ANDS    R2, R2, #PMC_LOCK
        BEQ     PLL_Loop
.endif

#  Select Master clock = PLL/2 = 47,923MHz
        LDR     R1, =PMC_MCKR_Val
        STR     R1, [R0, #PMC_MCKR]	
.endif
		

# Setup Stack for each mode
	   	LDR     R0, =Top_Stack

# Set up Fast Interrupt Mode and set FIQ Mode Stack
        MSR     CPSR_c, #Mode_FIQ|I_BIT|F_BIT
        mov     r13, r0                     
        sub     r0, r0, #FIQ_Stack_Size

# Set up Interrupt Mode and set IRQ Mode Stack
        msr     CPSR_c, #Mode_IRQ|I_BIT|F_BIT
        mov     r13, r0                     
        sub     r0, r0, #IRQ_Stack_Size

# Set up Abort Mode and set Abort Mode Stack
        msr     CPSR_c, #Mode_ABT|I_BIT|F_BIT
        mov     r13, r0                     
        sub     r0, r0, #ABT_Stack_Size

# Set up Undefined Instruction Mode and set Undef Mode Stack
        msr     CPSR_c, #Mode_UND|I_BIT|F_BIT
        mov     r13, r0                     
        sub     r0, r0, #UND_Stack_Size

# 	Set up Supervisor Mode and set Supervisor Mode Stack
        msr     CPSR_c, #Mode_SVC|I_BIT|F_BIT
        mov     r13, r0                     

#  Setup a default Stack Limit (when compiled with "-mapcs-stack-check")
        SUB     SL, SP, #1<<10         /* 1kB */


# Relocate .data section (Copy from ROM to RAM)
        LDR     R1, =_etext
        LDR     R2, =_data
        LDR     R3, =_edata
LoopRel:
        CMP     R2, R3
        LDRLO   R0, [R1], #4
        STRLO   R0, [R2], #4
        BLO     LoopRel


# Clear .bss section (Zero init)                
	    MOV     R0, #0
		LDR     R1, =__bss_start__
	    LDR     R2, =__bss_end__
LoopZI: 
        CMP     R1, R2
		STRLO   R0, [R1], #4
	    BLO     LoopZI

# Enter the C code
	    B       modsim_main
        .size   _startup, . - _startup
        .endfunc



	.equ	AIC_BASE,	0xFFFFF000		/* base address AIC controller */
	.equ	AIC_IVR, 	0x00000100		/* offset AIC_IVR register */		 
	.equ	AIC_EOICR, 	0x00000130		/* offset AIC_EOICR register */


		.func	IRQ_Handler
		 
IRQ_Handler:						/* Manage Exception Entry				*/
		sub  	lr, lr, #4			/* Adjust and save LR_irq in IRQ stack	*/
		stmfd   sp!, {lr}
		mrs     r14, SPSR			/* Save SPSR need to be saved for nested interrupt */
		stmfd   sp!, {r14}
        stmfd   sp!, {r0}			/* Save lr and r0 in IRQ stack */
		ldr     r14, =AIC_BASE		/* Write in the IVR to support Protect Mode */
		ldr     r0 , [r14, #AIC_IVR]/* No effect in Normal Mode */
		str     r14, [r14, #AIC_IVR]/* De-assert the NIRQ and clear the source in Protect Mode */
#		msr     CPSR_c, #Mode_SVC	/* Enable Interrupt and Switch in Supervisor Mode */
		stmfd   sp!, {r1-r3,r12,r14}/* Save scratch/used registers and LR in User Stack */
        mov     r14, pc				/* Branch to the routine pointed by the AIC_IVR */
        bx      r0
        ldmia   sp!, {r1-r3,r12,r14}/* Restore scratch/used registers and LR from User Stack */
#        msr     CPSR_c, #I_BIT | Mode_IRQ /* Disable Interrupt and switch back in IRQ mode */
        ldr     r14, =AIC_BASE		/* Mark the End of Interrupt on the AIC */
        str     r14, [r14, #AIC_EOICR]
        ldmia   sp!, {r0}			/* Restore SPSR_irq and r0 from IRQ stack */
        ldmia   sp!, {r14}			/* Restore SPSR_irq and r0 from IRQ stack */
        msr     SPSR_cxsf, r14
        ldmia   sp!, {pc}^			/* Restore adjusted  LR_irq from IRQ stack directly in the PC */

        .size   IRQ_Handler, . - IRQ_Handler
        .endfunc

	
        .end

