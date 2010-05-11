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
static void  AppTask1(void *p_arg);
static void  AppTask2(void *p_arg);
void LED_show(INT16U n);

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
    //DDRB = 0x0F;  // TODO IHA Remove after test


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

    (void)p_arg;                                 /* Prevent compiler warnings                          */

    BSP_Init();                                  /* Initialize the BSP                                 */
    init_lego_interface();						 /* Initalize LEGO_interface						   */

    AppTaskCreate();

    while (1) {                                  /* Task body, always written as an infinite loop.     */

    	/* INT8U motor_no = 1;
    	INT8S speed = 100;



    	motor_speed(motor_no, speed); */

    	INT16U light_value;
    	light_value = light_sensor(0);
    	light_value = light_value >> 2;
    	LED_Show(light_value);
    	OSTimeDly(OS_TICKS_PER_SEC / 5);
    }
}


/*
**************************************************************************************************************
*                                        CREATE APPLICATION TASKS
*
* Description : This function creates the application tasks.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
**************************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    INT8U  err;
#endif


    /*---- Task initialization code goes HERE! --------------------------------------------------------*/
    OSTaskStkSize     = OS_TASK_1_STK_SIZE;        /* Setup the default stack size                     */
//    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size            */
    OSTaskCreateExt(AppTask1,
                    (void *)0,
                    (OS_STK *)&AppTask1Stk[OSTaskStkSize - 1],
                    OS_TASK_1_PRIO,
                    OS_TASK_1_PRIO,
                    (OS_STK *)&AppTask1Stk[0],
                    OSTaskStkSize,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_TASK_1_PRIO, "Task 1", &err);
#endif

    /*---- Task initialization code goes HERE! --------------------------------------------------------*/
    OSTaskStkSize     = OS_TASK_2_STK_SIZE;        /* Setup the default stack size                     */
    //OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size            */
    OSTaskCreateExt(AppTask2,
                    (void *)0,
                    (OS_STK *)&AppTask2Stk[OSTaskStkSize - 1],
                    OS_TASK_2_PRIO,
                    OS_TASK_2_PRIO,
                    (OS_STK *)&AppTask2Stk[0],
                    OSTaskStkSize,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_TASK_2_PRIO, "Task 2", &err);
#endif
}

/*
**************************************************************************************************************
*                                                   TASK #1
**************************************************************************************************************
*/

static void  AppTask1(void *p_arg)
{
    (void)p_arg;

   // int i = 0;
    while (1) {
        //LED_Toggle(7);
    	//LED_show(++i);
       // OSTimeDly(OS_TICKS_PER_SEC / 5);
        //i = (i > 254) ? 0 : i;
    }
}

/*
**************************************************************************************************************
*                                                  TASK #2
**************************************************************************************************************
*/

static void  AppTask2(void *p_arg)
{
    (void)p_arg;

    while (1) {
      //  LED_Toggle(8);
      //  OSTimeDly(OS_TICKS_PER_SEC / 5);
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

/*
 * Custom functions
*/

void LED_Show(INT16U n){
	/* Turn off all LEDs before switching them */
	int i;
	for(i = 0; i < 8; i++){
		LED_Off(i);
		//printf("%s%d\n", "Switching off LED #", i);
	}

	/* For all bits in datatype */
	for(i = 0; i < 8; i++){
		if(n & (1 << i))						/* Test if i^th bit is set in "n" */
			LED_On(i);
			//printf("%s%d\n", "Switching on LED #", i); /* if yes, turn on the i^th LED */
	}
}
