;********************************************************************************************************
;                                       APPLICATION SPECIFIC ISRs
;
;                                (c) Copyright 2005, Micrium, Weston, FL
;                                          All Rights Reserved
;
; File     : APP_ISR.asm
; By       : Jean J. Labrosse Ported by Ib Havn to AVR ATMEGA2560
;********************************************************************************************************

#include        <os_cpu_i.asm>

;********************************************************************************************************
;                                         EXTERNAL DECLARATIONS
;********************************************************************************************************

                .extern  OSIntExit
                .extern  OSIntNesting
                .extern  OSTCBCur

                .extern  BSP_TickISR_Handler  ; C part of Timer intterrupt
                .extern  BSP_Int0ISR_Handler  ; C part of Key handler intterrupt

;********************************************************************************************************
;                                          PUBLIC DECLARATIONS
;********************************************************************************************************
                .global	TIMER0_COMPA_vect
                .global	INT0_vect


;/*$PAGE*/
;********************************************************************************************************
;                                           SYSTEM TICK ISR
;
; Description : This function is the Tick ISR.
;
;               The following C-like pseudo-code describe the operation being performed in the code below.
;
;               Save all registers on the current task's stack:
;                      Use the PUSH_ALL macro
;                      Get the SREG, set Bit #7 and save onto the task's stack
;     TODO IHA fjerens??                 Use the PUSH_SP macro to save the task's hardware stack pointer onto the current task's stack
;               OSIntNesting++;
;               if (OSIntNesting == 1) {
;                  OSTCBCur->OSTCBStkPtr = SP
;               }
;               Clear the interrupt;                  Not needed for the timer we used.
;               OSTimeTick();                         Notify uC/OS-II that a tick has occured
;               OSIntExit();                          Notify uC/OS-II about end of ISR
;               Restore all registers that were save on the current task's stack:
;                      Use the POP_SP macro to restore the task's hardware stack pointer
;                      Use the POP_SREG macro to restore the SREG register
;                      Use the POP_ALL macro to restore the remaining registers
;               Return from interrupt
;********************************************************************************************************
TIMER0_COMPA_vect:
                PUSH_ALL                            ; Save all registers and status register
                IN      R16,_SFR_IO_ADDR(SREG)      ; Save the SREG but with interrupts enabled
                SBR     R16,(1<<SREG_I)
                PUSH    R16
;                                                   ; Save the task's hardware stack pointer onto task's stack

                LDS     R16,OSIntNesting            ; Notify uC/OS-II of ISR
                INC     R16                         ;
                STS     OSIntNesting,R16            ;

                CPI     R16,1                       ; if (OSIntNesting == 1) {
                BRNE    BSP_TickISR_1

                LDS     R30,OSTCBCur                ;     OSTCBCur->OSTCBStkPtr = SP
                LDS     R31,OSTCBCur+1

                IN		R28,_SFR_IO_ADDR(SPL)	    ;     Save SP in TCB
				ST		Z+,R28
				IN		R29,_SFR_IO_ADDR(SPH)
				ST		Z+,R29                      ; }

BSP_TickISR_1:
                CALL    BSP_TickISR_Handler         ; Call tick ISR Handler written in C

                CALL    OSIntExit                   ; Notify uC/OS-II about end of ISR

                                                    ; Restore the stack pointer from TCB
                LDS     R30,OSTCBCur
                LDS     R31,OSTCBCur+1
				LD		R28,Z+						; Restore stack pointer
				OUT		_SFR_IO_ADDR(SPL),R28
				LD		R29,Z+
				OUT		_SFR_IO_ADDR(SPH),R29

                POP_SREG_INT
                POP_ALL                             ; Restore all registers
                RETI

 ;/*$PAGE*/
;********************************************************************************************************
;                                           INT0 ISR
;
; Description : This function is the INT0 isr.
;
;               The following C-like pseudo-code describe the operation being performed in the code below.
;
;               Save all registers on the current task's stack:
;                      Use the PUSH_ALL macro
;                      Get the SREG, set Bit #7 and save onto the task's stack
;               OSIntNesting++;
;               if (OSIntNesting == 1) {
;                  OSTCBCur->OSTCBStkPtr = SP
;               }
;               Call C-part of isr //Must Clear the interrupt reason
;               OSIntExit();                          Notify uC/OS-II about end of ISR
;               Restore all registers that were save on the current task's stack:
;                      Use the POP_SP macro to restore the task's hardware stack pointer
;                      Use the POP_SREG macro to restore the SREG register
;                      Use the POP_ALL macro to restore the remaining registers
;               Return from interrupt
;********************************************************************************************************
INT0_vect:
                PUSH_ALL                            ; Save all registers and status register
                IN      R16,_SFR_IO_ADDR(SREG)      ; Save the SREG but with interrupts enabled
                SBR     R16,(1<<SREG_I)
                PUSH    R16
;                                                   ; Save the task's hardware stack pointer onto task's stack

                LDS     R16,OSIntNesting            ; Notify uC/OS-II of ISR
                INC     R16                         ;
                STS     OSIntNesting,R16            ;

                CPI     R16,1                       ; if (OSIntNesting == 1) {
                BRNE    BSP_Int0ISR_1

                LDS     R30,OSTCBCur                ;     OSTCBCur->OSTCBStkPtr = SP
                LDS     R31,OSTCBCur+1

                IN		R28,_SFR_IO_ADDR(SPL)	    ;     Save SP in TCB
				ST		Z+,R28
				IN		R29,_SFR_IO_ADDR(SPH)
				ST		Z+,R29                      ; }

BSP_Int0ISR_1:
                CALL    BSP_Int0ISR_Handler         ; Call INT4 ISR Handler written in C

                CALL    OSIntExit                   ; Notify uC/OS-II about end of ISR

                                                    ; Restore the stack pointer from TCB
                LDS     R30,OSTCBCur
                LDS     R31,OSTCBCur+1
				LD		R28,Z+						; Restore stack pointer
				OUT		_SFR_IO_ADDR(SPL),R28
				LD		R29,Z+
				OUT		_SFR_IO_ADDR(SPH),R29

                POP_SREG_INT
                POP_ALL                             ; Restore all registers
                RETI


