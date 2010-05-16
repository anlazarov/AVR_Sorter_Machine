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
*												CONSTANTS
**************************************************************************************************************
*/
/*
 * yellow : 174, 172, 169
 * black : 187-8, 194-3,
 *
 */

#define TASK_1_PRIO				5
#define TASK_2_PRIO 			4

/* SENSORS */
#define SENSOR_COUNT			0
#define SENSOR_COLOR			1
#define SENSOR_RT				2
#define SENSOR_TOUCH			3

#define SENSOR_COUNT_DEF_VAL	209
#define SENSOR_COLOR_DEF_VAL	203
/* MOTORS */
#define MOTOR_BELT_1			0
#define MOTOR_BUF				1
#define MOTOR_SORT				2
#define MOTOR_BELT_2			3

/* BUFFER  */
#define BUFFER_SIZE				3

/*
**************************************************************************************************************
*                                               VARIABLES
**************************************************************************************************************
*/

OS_STK  AppTaskStartStk[OS_TASK_START_STK_SIZE];
OS_STK  AppTask1Stk[OS_TASK_1_STK_SIZE];
OS_STK  AppTask2Stk[OS_TASK_2_STK_SIZE];
OS_STK  AppTask3Stk[OS_TASK_3_STK_SIZE];


OS_EVENT *count_sem;
OS_EVENT *a_sem; // Pointer to a semaphore

INT16U count = 0;
INT8U brick_found = 0;
INT8U dispatched = 0;
/*
**************************************************************************************************************
*                                           FUNCTION PROTOTYPES
**************************************************************************************************************
*/

static void  AppTaskStart(void *p_arg);
static void  AppTaskCreate(void);
static void  AppTask1(void *p_arg);
static void  AppTask2(void *p_arg);
static void  AppTask3(void *p_arg);
void LED_Show(INT8U n);

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

    OSInit();                                        /* Initialize "uC/OS-II, The Real-Time Kernel"         */

    /*---- Any initialization code before starting multitasking --------------------------------------------*/
    OSTaskStkSize     = OS_TASK_START_STK_SIZE;

    OSTaskCreateExt(AppTaskStart,
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

	/* CREATING THE SEMAPHORES */

    a_sem = OSSemCreate(0);
	count_sem = OSSemCreate(BUFFER_SIZE);

    /*---- Create any other task you want before we start multitasking -------------------------------------*/

    OSStart();                                       /* Start multitasking (i.e. give control to uC/OS-II)  */
    return 0;
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

    /*--- TASK 3 --- */
    OSTaskStkSize     = OS_TASK_3_STK_SIZE;        /* Setup the default stack size                     */
    //OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size            */
    OSTaskCreateExt(AppTask3,
                    (void *)0,
                    (OS_STK *)&AppTask3Stk[OSTaskStkSize - 1],
                    OS_TASK_3_PRIO,
                    OS_TASK_3_PRIO,
                    (OS_STK *)&AppTask3Stk[0],
                    OSTaskStkSize,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
 #if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
   OSTaskNameSet(OS_TASK_3_PRIO, "Task 3", &err);
 #endif
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

static void  AppTaskStart (void *p_arg)
{
	(void)p_arg;								// Prevent compiler warnings

    INT8U err;									// needed for the semaphore

    BSP_Init();									// Initialize the BSP
	init_lego_interface(); 						// Initalize LEGO_interface
    AppTaskCreate();							// create the other tasks
	while (1) 									// Task body, always written as an infinite loop.
	{
		/*
		 * If you want to check the values of the sensors
		 * comment the pending of the semaphore a_sem ( OSSemPend(a_sem, 0, &err); )
		 * and use the following code only in AppTaskStart
		 *
		 *		INT8U sens_value = light_sensor(SENSOR_RT) >> 2;
		 *		LED_Show(sens_value);
		 *		OSTimeDly(OS_TICKS_PER_SEC / 2);
		 */
		// from ib's code
        OSSemPend(a_sem, 0, &err);  // Wait for the semaphore to be signaled
    }
}

/*
**************************************************************************************************************
*                                                   TASK #1
**************************************************************************************************************
*/

static void  AppTask1(void *p_arg)
{
	(void)p_arg;
	INT8U err;
	INT8U light_value;

	OSTimeDly(OS_TICKS_PER_SEC / 2); // delay that will prevent give the sensor time to read correct values

	while (1)
	{


		motor_speed(MOTOR_BELT_1, -40);						//  start MOTOR_BELT_1
		light_value = light_sensor(SENSOR_COUNT) >> 2; 		//  SENSOR_COUNT reads value in 8 bits
		LED_Show(light_value);
		OSTimeDly(OS_TICKS_PER_SEC / 2);

		if(light_value < SENSOR_COUNT_DEF_VAL)
		{
			OSSemPend(count_sem, 0, &err); 					// pend semaphore
			count++; 										// increase the count of the bricks
			LED_Show(count);								// show the count of brick on the LEDs
			OSTimeDly(OS_TICKS_PER_SEC);					//
		}
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
   // INT8U err;
    while (1)
    {
    	OSTimeDly(OS_TICKS_PER_SEC );
		if (count > 0)
		{
			OSSemPost(count_sem);
			count--;
			LED_Show(count);

			motor_speed(MOTOR_BELT_2, -40);					// start MOTOR_BELT_2
			OSTimeDly(OS_TICKS_PER_SEC / 5);				// wait for a while
			motor_speed(MOTOR_BUF, 10);						// release a brick
			OSTimeDly(OS_TICKS_PER_SEC / 2);
		}
		motor_speed(MOTOR_BUF, 0);
		OSTimeDly(OS_TICKS_PER_SEC / 10);
    }
}

/*
**************************************************************************************************************
*                                                  TASK #2
**************************************************************************************************************
*/

static void  AppTask3(void *p_arg)
{
    (void)p_arg;
    INT8U brick_color;
    INT8U speed = 0;
    INT8U sort_type = 0; //1 - cw; 2 - ccw; 3 - pass
    while (1){
    	if(dispatched == 1){
    		motor_speed(MOTOR_BELT_2, -40);					// start MOTOR_BELT_2
    		OSTimeDly(OS_TICKS_PER_SEC / 5);				// wait for a while
    		motor_speed(MOTOR_BELT_2, 0);					// start MOTOR_BELT_2

    		//if color = SOME_COLOR move cw
    		if(brick_color > 100 && brick_color < 150){
				sort_type = 1;
			//or ANOTHER_COLOR move ccw
    		}else if(brick_color < 100 && brick_color > 0){
    			sort_type = 2;
    		//or OTHER_COLOR - let it pass through
    		}else{
				sort_type = 3;
    		}

			switch(sort_type){
			case 1 :
				speed = -40;
				break;
			case 2 : break;
			case 3 : break;
			default : break;
			}
    		motor_speed(MOTOR_SORT, speed);
    		OSTimeDly(OS_TICKS_PER_SEC / 5);				// wait for a while

    		//return motor to the previous position, we assume constant motor speed
    		motor_speed(MOTOR_SORT, -speed);
    		OSTimeDly(OS_TICKS_PER_SEC / 5);				// wait for a while

    		dispatched = 0;									// signal task 2 we have finished sorting
    	}else{

    	}
    }
}
/*
*********************************************************************************************************
* 											Additional methods
*********************************************************************************************************
*/
void LED_Show(INT8U n) {
	/* Turn off all LEDs before switching them */
	int i;
	for (i = 0; i < 8; i++) {
		LED_Off(i);
		//printf("%s%d\n", "Switching off LED #", i);
	}

	/* For all bits in datatype */
	for (i = 1; i <= 8; i++) {

		if (n & (1 << (i - 1))) /* Test if i^th bit is set in "n" */
			LED_On(i);
		//printf("%s%d\n", "Switching on LED #", i); /* if yes, turn on the i^th LED */
	}
}

INT8U checkRange(INT8U number, INT8U number2, INT8U threshold){
	if((number > abs(number2 - threshold)) && (number < abs(number2 + threshold)))
		return 1;
	return 0;
}

/*	Runs motor for the specified time
 *  \param motor_no specifies the motor output on the interface board [0..3]
 *  \param speed in percent specifies speed and way of rotation: negative CCW; positive CW
 *  \param time specifies time to run motor in system ticks using OSTimeDly
 *  \param restore if we want to restore previous position of the motor set to 1, else 0
 *	@returns : void
 *	\see motor_speed OSTimeDly
 */
void motor_run_ext(INT8U motor_no, INT8U speed, INT8U time, INT8U restore){
	motor_speed(motor_no, speed);
	OSTimeDly(time);

	//stop motor
	brake_motor(motor_no);

	brake_motor(motor_no);

	//if we want to restore motor position, we can run with the same values, but backwards
	if(restore == 1){
		motor_run_ext(motor_no, -speed, time, 0);
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
	//PORTB = ~(OSTCBHighRdy->OSTCBPrio);
}

void App_TaskCreateHook(OS_TCB *ptcb){}
void App_TaskDelHook(OS_TCB *ptcb){}
void App_TaskIdleHook(void){}
void App_TaskStatHook(void){}
void App_TCBInitHook(OS_TCB *ptcb){}
void App_TimeTickHook(void){}
