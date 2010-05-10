/*
*************************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                                         ATmega2560  Sample code
*
* File : APP.C
* By   : Fabiano Kovalski - portet to ATMEGA2560 by Ib Havn, 2010
*************************************************************************************************************
*/

#include  <includes.h>

/*
**************************************************************************************************************
*                                               VARIABLES
**************************************************************************************************************
*/

OS_STK  AppTaskStartStk[OS_TASK_START_STK_SIZE];
OS_STK  AppTask1Stk[OS_TASK_1_STK_SIZE];
OS_STK  AppTask2Stk[OS_TASK_2_STK_SIZE];

OS_EVENT *a_sem; // Pointer to a semaphore

/*
**************************************************************************************************************
*                                           FUNCTION PROTOTYPES
**************************************************************************************************************
*/

static void  Task1(void *p_arg);
static void  AppTaskCreate(void);



/*
**************************************************************************************************************
*                                                MAIN
*
* Note(s): 1) You SHOULD use OS_TASK_STK_SIZE (see OS_CFG.H) when setting OSTaskStkSize prior to calling
*             OSInit() because OS_TASK_IDLE_STK_SIZE and OS_TASK_STAT_STK_SIZE are set to this value in
*             OS_CFG.H.
**************************************************************************************************************
*/

int  main (void)
{
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    INT8U  err;
#endif
	//git test
  //  DDRB = 0x0F;  // TODO IHA Remove after test

    /*---- Any initialization code prior to calling OSInit() goes HERE -------------------------------------*/

                                                     /* IMPORTANT: MUST be setup before calling 'OSInit()'  */
    OSTaskStkSize     = OS_TASK_IDLE_STK_SIZE;       /* Setup the default stack size                        */
//    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;       /* Setup the default hardware stack size               */

    OSInit();                                        /* Initialize "uC/OS-II, The Real-Time Kernel"         */

    /*---- Any initialization code before starting multitasking --------------------------------------------*/
    OSTaskStkSize     = OS_TASK_START_STK_SIZE;

    OSTaskCreateExt(Task1,
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[OSTaskStkSize - 1],
                    OS_TASK_START_PRIO,
                    OS_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    OSTaskStkSize,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_TASK_START_PRIO, "Start Task", &err);
#endif

    a_sem = OSSemCreate(0);   // create a semaphore

    /*---- Create any other task you want before we start multitasking -------------------------------------*/

    OSStart();                                       /* Start multitasking (i.e. give control to uC/OS-II)  */
    return 0;
}

/*
**************************************************************************************************************
*                                              STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
**************************************************************************************************************
*/

static void  Task1 (void *p_arg)
{
    INT8U  i;
    INT8U err;

    (void)p_arg;                                 /* Prevent compiler warnings                          */

    BSP_Init();                                  /* Initialize the BSP                                 */

    AppTaskCreate();

    while (1) {                                  /* Task body, always written as an infinite loop.     */

    	INT8U motor_no = 1;
    	INT8S speed = 100;

    	init_lego_interface();

    	motor_speed(motor_no, speed);
    }
}


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
void App_TaskSwHook(void)  // TODO IHA Remove after test
{
	PORTB = ~(OSTCBHighRdy->OSTCBPrio);
}

void App_TaskCreateHook(OS_TCB *ptcb){}
void App_TaskDelHook(OS_TCB *ptcb){}
void App_TaskIdleHook(void){}
void App_TaskStatHook(void){}
void App_TCBInitHook(OS_TCB *ptcb){}
void App_TimeTickHook(void){}
