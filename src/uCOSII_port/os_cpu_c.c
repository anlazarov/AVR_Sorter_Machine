/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                                       ATmega128 Specific code
*
* File     : OS_CPU_C.C
* By       : Jean J. Labrosse
* By	   : Ib Havn ported to GNU avr-gcc 2010 - ATMEGA2560
*********************************************************************************************************
*/

#define   OS_CPU_GLOBALS
#include  <ucos_ii.h>

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#if      (OS_VERSION >= 281) && (OS_TMR_EN > 0)
  static  INT16U  OSTmrCtr;
#endif                                                                  /* #if (OS_VERSION >= 281) && (OS_TMR_EN > 0)               */

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
#if OS_VERSION >= 281 && OS_TMR_EN > 0
    OSTmrCtr = 0;
#endif
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskCreateHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskCreateHook(ptcb);
#else
    (void)ptcb;                                                         /* Prevent compiler warning                                 */
#endif
}
#endif


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void  OSTaskDelHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskDelHook(ptcb);
#else
    (void)ptcb;                                                         /* Prevent compiler warning                                 */
#endif
}
#endif

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskIdleHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/

#if OS_CPU_HOOKS_EN > 0
void  OSTaskStatHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskStatHook();
#endif
}
#endif

/*$PAGE*/
/*
**********************************************************************************************************
*                                       INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created. This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              p_arg         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack. It is assumed that 'ptos' points to the
*                            highest valid address on the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              SREG to 0x00 instead. In this case, interrupts would be disabled upon task startup. The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code. You will need to modify OSTaskIdle() and OSTaskStat() so that they enable interrupts.
*              Failure to do this will make your system crash!
*
*
**********************************************************************************************************
*/

OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *p_arg, OS_STK *ptos, INT16U opt)
{
    INT8U  *stk;
    INT16U  tmp;

    (void)opt;                              /* 'opt' is not used, prevent warning                       */
    stk     = (INT8U *)ptos;				/* AVR return stack                                        */

    tmp     = (INT16U)task;
                                            /* Put task start address on top of "hardware stack"        */
    *stk--  = (INT8U)(tmp & 0xFF);    		/* Save PC return address Three Bytes PC in ATMEGA2560/ATMEGA2561 IHA*/
    tmp >>= 8;
    *stk--  = (INT8U)(tmp & 0xFF);
    tmp = 0x0000;
    *stk--  = (INT8U)(tmp & 0xFF);

    *stk--  = (INT8U)0x00;            /* R0    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R1    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R2    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R3    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R4    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R5    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R6    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R7    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R8    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R9    = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R10   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R11   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R12   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R13   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R14   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R15   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R16   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R17   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R18   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R19   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R20   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R21   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R22   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R23   = 0x00                                             */
    tmp     = (INT16U)p_arg;
     *stk--  = (INT8U)tmp;             /* 'p_arg' passed in R24:R25                                */
     *stk--  = (INT8U)(tmp >> 8);

    *stk--  = (INT8U)0x00;            /* R26   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R27   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R28   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R29   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R30   = 0x00                                             */
    *stk--  = (INT8U)0x00;            /* R31   = 0x00                                             */
#ifdef RAMPZ
    *stk--  = (INT8U)0x00;            /* RAMPZ = 0x00                                             */
#endif
    *stk--  = (INT8U)0x80;            /* SREG  = Interrupts enabled                               */

    return ((OS_STK *)stk);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TASK_SW_HOOK_EN > 0)
void  OSTaskSwHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TaskSwHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                           OS_TCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
#if OS_APP_HOOKS_EN > 0
    App_TCBInitHook(ptcb);
#else
    (void)ptcb;                                                         /* Prevent compiler warning                                 */
#endif
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if (OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)
void  OSTimeTickHook (void)
{
#if OS_APP_HOOKS_EN > 0
    App_TimeTickHook();
#endif

#if OS_VERSION >= 281 && OS_TMR_EN > 0
    OSTmrCtr++;
    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
        OSTmrCtr = 0;
        OSTmrSignal();
    }
#endif
}
#endif
