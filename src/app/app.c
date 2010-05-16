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
 * COLOR READINGS:
 * yellow : 174, 172, 168-169
 * black : 187-8, 194-3
 *
 */

/* SENSORS */
#define SENSOR_COUNT			0
#define SENSOR_COLOR			1
#define SENSOR_RT				2 	// DOESN'T WORK
#define SENSOR_TOUCH			3	// DOESN'T WORK

#define SENSOR_COUNT_DEF_VAL	209
#define SENSOR_COLOR_DEF_VAL	203
/* MOTORS */
#define MOTOR_BELT_1			0
#define MOTOR_BUF				1
#define MOTOR_SORT				2
#define MOTOR_BELT_2			3

/* BUFFER  */
#define BUFFER_SIZE				3

/* COLORS */
#define COLOR_YELLOW_LOW		168
#define COLOR_YELLOW_HIGH		180
#define COLOR_BLACK_LOW			185
#define COLOR_BLACK_HIGH		195
/*
**************************************************************************************************************
*                                               VARIABLES
**************************************************************************************************************
*/

OS_STK  AppTaskStartStk[OS_TASK_START_STK_SIZE];
OS_STK  AppTask1Stk[OS_TASK_1_STK_SIZE];
OS_STK  AppTask2Stk[OS_TASK_2_STK_SIZE];
OS_STK  AppTask3Stk[OS_TASK_3_STK_SIZE];

/* Pointers to semaphores */
OS_EVENT *count_sem;
OS_EVENT *a_sem;
OS_EVENT *dispatch_sem;

INT16U count = 0;
INT8U brick_color = 0;
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
void motor_run_ext(INT8U motor_no, INT8U speed, INT16U ticks, INT8U postmode, INT8U restore);
INT8U inRange(INT8U number, INT8U number2, INT8U threshold);
INT8U is_in_range(INT8U color_current, INT8U color_low, INT8U color_high);
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
	dispatch_sem = OSSemCreate(1);
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

    /*--- TASK 1 --- INITIALIZATION ---*/
    OSTaskStkSize     = OS_TASK_1_STK_SIZE;        /* Setup the default stack size                     */
//    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size          */
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

    /*--- TASK 2 --- INITIALIZATION ---*/
    OSTaskStkSize     = OS_TASK_2_STK_SIZE;        /* Setup the default stack size                     */
    //OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size          */
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

    /*--- TASK 3 --- INITIALIZATION ---*/
    OSTaskStkSize     = OS_TASK_3_STK_SIZE;        /* Setup the default stack size                     */
    //OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size          */
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
		if(count < BUFFER_SIZE ){
			motor_speed(MOTOR_BELT_1, -40);					//  start MOTOR_BELT_1
		}else{
			brake_motor(MOTOR_BELT_1);
		}

		light_value = light_sensor(SENSOR_COUNT) >> 2; 		//  SENSOR_COUNT reads value in 8 bits
		OSTimeDly(OS_TICKS_PER_SEC / 2);

		//if there's no default value
		if(light_value < SENSOR_COUNT_DEF_VAL)
		{
			OSSemPend(count_sem, 0, &err); 					// pend semaphore
			count++; 										// increase the count of the bricks
			//LED_Show(count);								// show the count of brick on the LEDs
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
    INT8U err;
    while (1)
    {
    	OSTimeDly(OS_TICKS_PER_SEC);

    	//if we got a brick
		if (count > 0)
		{
			brick_color = light_sensor(SENSOR_COLOR) >> 2;	 					//get brick color
			OSTimeDly(OS_TICKS_PER_SEC / 5);
			LED_Show(brick_color);												// show the brick color on the LEDs

			if(brick_color < SENSOR_COLOR_DEF_VAL){
				motor_run_ext(MOTOR_BUF, 10, OS_TICKS_PER_SEC/2, 2, 0);			// release a brick
				motor_run_ext(MOTOR_BELT_2,-100, OS_TICKS_PER_SEC*0.8, 0, 0);	// start MOTOR_BELT_2
				brake_motor(MOTOR_BUF);											// stop release

				OSSemPend(dispatch_sem, 0, &err);
				dispatched = 1;													//finish dispatch
				OSSemPost(dispatch_sem);

				//decrease the count
				count--;
				OSSemPost(count_sem);
			}
		}
    }
}

/*
**************************************************************************************************************
*                                                  TASK #2
**************************************************************************************************************
*/

static void AppTask3(void *p_arg)
{
    (void)p_arg;
    INT8U err;
    INT8U speed = 100;
    INT8U delay = OS_TICKS_PER_SEC / 5;

    while (1){
    	//if we have dispatched brick
    	if(dispatched == 1){
    		//if color = SOME_COLOR move cw
    		if(inRange(brick_color, COLOR_YELLOW_LOW+5, 10) == 1){
    			motor_run_ext(MOTOR_SORT, speed, delay, 0, 1);		//sort brick on one side
			//or ANOTHER_COLOR move ccw
    		}else if(inRange(brick_color, COLOR_BLACK_LOW+5, 10) == 1){
    			motor_run_ext(MOTOR_SORT, -speed, delay, 0, 1);		//sort brick on the other side
    		//or ANY_OTHER_COLOR
    		}else{
    			//motor_run_ext(MOTOR_SORT, speed, OS_TICKS_PER_SEC / 5.2, 0, 1); //let it pass through
    		}

    		// SECOND WAY
    		//if(is_in_range(brick_color, COLOR_YELLOW_LOW, COLOR_YELLOW_HIGH) == 1){
    		//   			motor_run_ext(MOTOR_SORT, speed, delay, 0, 1);		//sort brick on one side
    		//			//or ANOTHER_COLOR move ccw
    		//   		}else if(is_in_range(brick_color, COLOR_BLACK_LOW, COLOR_BLACK_HIGH) == 1){
    		//  			motor_run_ext(MOTOR_SORT, -speed, delay, 0, 1);		//sort brick on the other side
    		//    		//or ANY_OTHER_COLOR
    		//  		}else{
    		//    			//motor_run_ext(MOTOR_SORT, speed, OS_TICKS_PER_SEC / 5.2, 0, 1); //let it pass through
    		//   		}

    		OSSemPend(dispatch_sem, 0, &err);
    		dispatched = 0;									// signal task 2 we have finished sorting
    		OSSemPost(dispatch_sem);
    	}
    }
}
/*
*********************************************************************************************************
* 											Additional methods
*********************************************************************************************************
*/
void LED_Show(INT8U n) {
	int i = 0;

	/* Turn off all LEDs before switching them */
	for (i = 0; i < 8; i++) {
		LED_Off(i);
	}

	/* For all bits in datatype */
	for (i = 1; i <= 8; i++) {
		if (n & (1 << (i - 1))) /* Test if i^th bit is set in "n" */
			LED_On(i);
	}
}

/* Checks if 'number' is in range of 'number2' including 'threshold'
 * \param number specifies value to check
 * \param number2 specifies reference value
 * \param threshold specifies threshold of reference value
 * \returns 1 if the number is in range, 0 otherwise
 */
INT8U inRange(INT8U number, INT8U number2, INT8U threshold){
	if((number >= abs(number2 - threshold)) && (number <= abs(number2 + threshold)))
		return 1;
	return 0;
}

INT8U is_in_range(INT8U color_current, INT8U color_low, INT8U color_high)
{
	if ((color_current >= color_low) && (color_current <= color_high))
		return 1;
	return 0;
}

/*	Runs motor for the specified time
 *  \param motor_no specifies the motor output on the interface board [0..3]
 *  \param speed in percent specifies speed and way of rotation: negative CCW; positive CW
 *  \param ticks specifies number of CPU ticks motor should run (OSTimeDly)
 *  \param postmode specifies mode to set after delay [0 : brake, 1 : free run,  2 : keep running]
 *  \param restore if we want to restore previous position of the motor set to 1, else 0
 *	@returns : void
 *	\see motor_speed OSTimeDly
 *	TODO : should initiate a parallel task to don't block program flow with OSTimeDly()
 */
void motor_run_ext(INT8U motor_no, INT8U speed, INT16U ticks, INT8U postmode, INT8U restore){
	motor_speed(motor_no, speed);
	OSTimeDly(ticks);

	switch(postmode){
		case 0 : brake_motor(motor_no);
			break;
		case 1 : motor_speed(motor_no, 0);
			break;
		case 2 : //do nothing
			break;
		default : brake_motor(motor_no);
			break;
	}
	//if we want to restore motor position, we can run with the same values, but backwards
	if(restore == 1 && postmode < 2){
		motor_run_ext(motor_no, -speed, ticks, postmode, 0);
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
