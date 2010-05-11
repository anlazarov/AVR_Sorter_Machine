;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                                       ATMEL  AVR Specific code
;
;
; File     : OS_CPU_A.S90
; By       : Jean J. Labrosse
; By	   : Ib Havn ported to GNU avr-gcc 2009 pv 1.0
;********************************************************************************************************

#include <os_cpu_i.asm>
#include <../app/os_cfg.h>

;********************************************************************************************************
;                                          PUBLIC DECLARATIONS
;********************************************************************************************************

; IHA                MODULE  OS_CPU_A

                .global  OS_CPU_SR_Save
                .global  OS_CPU_SR_Restore
                .global  OSStartHighRdy
                .global  OSCtxSw
                .global  OSIntCtxSw

;********************************************************************************************************
;                                         EXTERNAL DECLARATIONS
;********************************************************************************************************

                .extern  OSIntExit
                .extern  OSIntNesting
                .extern  OSPrioCur
                .extern  OSPrioHighRdy
                .extern  OSRunning
                .extern  OSTaskSwHook
                .extern  OSTCBCur
                .extern  OSTCBHighRdy

;/*$PAGE*/
;********************************************************************************************************
;               DISABLE/ENABLE INTERRUPTS USING OS_CRITICAL_METHOD #3
;
; Description : These functions are used to disable and enable interrupts using OS_CRITICAL_METHOD #3.
;
;               OS_CPU_SR  OSCPUSaveSR (void)
;                     Get current value of SREG
;                     Disable interrupts
;                     Return original value of SREG
;
;               void  OSCPURestoreSR (OS_CPU_SR cpu_sr)
;                     Set SREG to cpu_sr
;                     Return
;********************************************************************************************************

OS_CPU_SR_Save:    IN      R24,_SFR_IO_ADDR(SREG)   ; Get current state of interrupts disable flag
                   CLI                              ; Disable interrupts
                   RET                              ; Return original SREG value in R24


OS_CPU_SR_Restore: OUT     _SFR_IO_ADDR(SREG),R24   ; Restore SREG
                   RET                              ; Return

;/*$PAGE*/
;********************************************************************************************************
;                               START HIGHEST PRIORITY TASK READY-TO-RUN
;
; Description : This function is called by OSStart() to start the highest priority task that was created
;               by your application before calling OSStart().
;
; Note(s)     : 1) The (data)stack frame is assumed to look as follows:
;
;
;                  OSTCBHighRdy->OSTCBStkPtr --> SPL of (return) stack pointer    (Low memory)
;                                                SPH of (return) stack pointer    --------
;																						  |
;																						  |
;                                                Flags to load in status register  <------
;                                                RAMPZ
;                                                R31
;                                                R30
;                                                R27
;                                                .
;                                                .
;                                                R0                                      (High memory)
;
;                  where the stack pointer points to the task start address.
;
;               2) OSStartHighRdy() MUST:
;                      a) Call OSTaskSwHook() then,
;                      b) Set OSRunning to TRUE,
;                      c) Switch to the highest priority task.
;********************************************************************************************************

OSStartHighRdy:
#if OS_TASK_SW_HOOK_EN > 0
				CALL    OSTaskSwHook                ; Invoke user defined context switch hook
#endif
                LDS     R16,OSRunning               ; Indicate that we are multitasking
                INC     R16                         ;
                STS     OSRunning,R16               ;

                LDS     R30,OSTCBHighRdy            ; Let Z point to TCB of highest priority task
                LDS     R31,OSTCBHighRdy+1          ; ready to run

;                                                   ; Restore stack pointer
				LD		R28,Z+
				OUT		_SFR_IO_ADDR(SPL),R28
				LD		R29,Z+
				OUT		_SFR_IO_ADDR(SPH),R29

                POP_SREG_INT                        ; Restore status register (DISABLE interrupts)
                POP_ALL                             ; Restore all registers
                RETI

;/*$PAGE*/
;********************************************************************************************************
;                                       TASK LEVEL CONTEXT SWITCH
;
; Description : This function is called when a task makes a higher priority task ready-to-run.
;
; Note(s)     : 1) Upon entry,
;                  OSTCBCur     points to the OS_TCB of the task to suspend
;                  OSTCBHighRdy points to the OS_TCB of the task to resume
;
;               2) The stack frame of the task to suspend looks as follows:
;
;                                       SP+0 --> LSB of task code address
;                                         +1     MSB of task code address                (High memory)
;
;               3) The saved context of the task to resume looks as follows:
;
;                                                       Hardware Stack points to EMPTY

;                  OSTCBHighRdy->OSTCBStkPtr --> SPL of (return) stack pointer      (Low memory)
;                                                SPH of (return) stack pointer
;                                                Flags to load in status register
;                                                RAMPZ
;                                                R31
;                                                R30
;                                                R27
;                                                .
;                                                .
;                                                R0                                      (High memory)
;********************************************************************************************************

OSCtxSw:        PUSH_ALL                            ; Save current task's context
                PUSH_SREG


                LDS     R30,OSTCBCur                ; Z = OSTCBCur->OSTCBStkPtr
                LDS     R31,OSTCBCur+1              ;

                IN		R28,_SFR_IO_ADDR(SPL)					; Save SP in TCB
				ST		Z+,R28
				IN		R29,_SFR_IO_ADDR(SPH)
				ST		Z+,R29


#if OS_TASK_SW_HOOK_EN > 0
                CALL    OSTaskSwHook                ; Call user defined task switch hook
#endif

                LDS     R16,OSPrioHighRdy           ; OSPrioCur = OSPrioHighRdy
                STS     OSPrioCur,R16

                LDS     R30,OSTCBHighRdy            ; Let Z point to TCB of highest priority task
                LDS     R31,OSTCBHighRdy+1          ; ready to run
                STS     OSTCBCur,R30                ; OSTCBCur = OSTCBHighRdy
                STS     OSTCBCur+1,R31              ;

				LD		R28,Z+						; Restore stack pointer
				OUT		_SFR_IO_ADDR(SPL),R28
				LD		R29,Z+
				OUT		_SFR_IO_ADDR(SPH),R29

                POP     R16		                    ; Restore status register
                SBRC    R16,7                       ; Skip next instruction if interrupts DISABLED
                RJMP    OSCtxSw_1

                OUT     _SFR_IO_ADDR(SREG),R16      ; Interrupts of task to return to are DISABLED
                POP_ALL
                RET

OSCtxSw_1:      CBR     R16,(1<<SREG_I)             ; Interrupts of task to return to are ENABLED
                OUT     _SFR_IO_ADDR(SREG),R16
                POP_ALL                             ; Restore all registers
                RETI

;/*$PAGE*/
;*********************************************************************************************************
;                                INTERRUPT LEVEL CONTEXT SWITCH
;
; Description : This function is called by OSIntExit() to perform a context switch to a task that has
;               been made ready-to-run by an ISR.
;
; Note(s)     : 1) Upon entry,
;                  OSTCBCur     points to the OS_TCB of the task to suspend
;                  OSTCBHighRdy points to the OS_TCB of the task to resume
;
;               2) The stack frame of the task to suspend looks as follows:
;
;                  OSTCBCur->OSTCBStkPtr ------> SPL of (return) stack pointer     (Low memory)
;                                                SPH of (return) stack pointer   ---
;                                                                                   |
;                                                Flags to load in status register <-
;                                                RAMPZ
;                                                R31
;                                                R30
;                                                R27
;                                                .
;                                                .
;                                                R0                                      (High memory)
;
;               3) The saved context of the task to resume looks as follows:
;
;
;                  OSTCBHighRdy->OSTCBStkPtr --> SPL of (return) stack pointer       (Low memory)
;                                                SPH of (return) stack pointer   ---
;                                                                                   |
;                                                Flags to load in status register <-
;                                                RAMPZ
;                                                R31
;                                                R30
;                                                R27
;                                                .
;                                                .
;                                                R0                                      (High memory)
;*********************************************************************************************************

OSIntCtxSw:
#if OS_TASK_SW_HOOK_EN > 0
                CALL    OSTaskSwHook                ; Call user defined task switch hook
#endif

                LDS     R16,OSPrioHighRdy           ; OSPrioCur = OSPrioHighRdy
                STS     OSPrioCur,R16

                LDS     R30,OSTCBHighRdy            ; Let Z point to TCB of highest priority task
                LDS     R31,OSTCBHighRdy+1          ; ready to run
                STS     OSTCBCur,R30                ; OSTCBCur = OSTCBHighRdy
                STS     OSTCBCur+1,R31              ;

				LD		R28,Z+
				OUT		_SFR_IO_ADDR(SPL),R28		; Restore stack pointer
				LD		R29,Z+
				OUT		_SFR_IO_ADDR(SPH),R29

                POP     R16                         ; Restore status register
                SBRC    R16,7                       ; Skip next instruction in interrupts DISABLED
                RJMP    OSIntCtxSw_1

                OUT     _SFR_IO_ADDR(SREG),R16      ; Interrupts of task to return to are DISABLED
                POP_ALL
                RET

OSIntCtxSw_1:   CBR     R16,(1<<SREG_I)             ; Interrupts of task to return to are ENABLED
                OUT     _SFR_IO_ADDR(SREG),R16
                POP_ALL                             ; Restore all registers
                RETI


