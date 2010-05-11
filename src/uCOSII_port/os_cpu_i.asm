;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                                       ATmega128  Specific code
;
;                                             INCLUDE FILE
;
; File     : OS_CPU_I.S90  ported to OS_CPU_i.ASM
; By       : Jean J. Labrosse
; By	   : Ib Havn ported to GNU avr-gcc 2009 pv 1.0
;********************************************************************************************************
;
#include <avr/io.h>

//#include <os_cfg.h>
#include <../app/os_cfg.h>


;********************************************************************************************************
;                                                MACROS
;********************************************************************************************************

.macro PUSH_ALL                 ; Save all registers
                PUSH	R0
                PUSH	R1
                PUSH	R2
                PUSH	R3
                PUSH	R4
                PUSH	R5
                PUSH	R6
                PUSH	R7
                PUSH	R8
                PUSH	R9
                PUSH	R10
                PUSH	R11
                PUSH	R12
                PUSH	R13
                PUSH	R14
                PUSH	R15
                PUSH	R16
                PUSH	R17
                PUSH	R18
                PUSH	R19
                PUSH	R20
                PUSH	R21
                PUSH	R22
                PUSH	R23
                PUSH	R24
                PUSH	R25
                PUSH	R26
                PUSH	R27
                PUSH	R28
                PUSH	R29
                PUSH	R30
                PUSH	R31
#ifdef RAMPZ
                IN      R16,_SFR_IO_ADDR(RAMPZ)
#endif
                PUSH	R16
.endm

.macro POP_ALL                  ; Restore all registers
#ifdef RAMPZ
                POP		R16
                OUT     _SFR_IO_ADDR(RAMPZ),R16
#endif
                POP		R31
                POP		R30
                POP		R29
                POP		R28
                POP		R27
                POP		R26
                POP		R25
                POP		R24
                POP		R23
                POP		R22
                POP		R21
                POP		R20
                POP		R19
                POP		R18
                POP		R17
                POP		R16
                POP		R15
                POP		R14
                POP		R13
                POP		R12
                POP		R11
                POP		R10
                POP		R9
                POP		R8
                POP		R7
                POP		R6
                POP		R5
                POP		R4
                POP		R3
                POP		R2
                POP		R1
                POP		R0
.endm

;.macro PUSH_SP                  ; Save stack pointer
;                IN      R16,_SFR_IO_ADDR(SPH)
;                PUSH	R16
;                IN      R16,_SFR_IO_ADDR(SPL)
;                PUSH	R16
;.endm

;.macro POP_SP                   ; Restore stack pointer
;                POP		R16
;                OUT     _SFR_IO_ADDR(SPL),R16
;                POP		R16
;                OUT     _SFR_IO_ADDR(SPH),R16
;.endm

.macro PUSH_SREG                ; Save status register
                IN      R16,_SFR_IO_ADDR(SREG)
                PUSH	R16
.endm

.macro POP_SREG                 ; Restore status registers
                POP		R16
                OUT     _SFR_IO_ADDR(SREG),R16
.endm

.macro PUSH_SREG_INT            ; Save status register with interrupts ENABLED
                IN      R16,_SFR_IO_ADDR(SREG)
                SBR     R16,(1<<SREG_I)    ;BIT07
                PUSH	R16
.endm

.macro POP_SREG_INT             ; Restore status registers but DISABLE interrupts
                POP		R16
                CBR     R16,(1<<SREG_I)    ;BIT07
                OUT     _SFR_IO_ADDR(SREG),R16
.endm
