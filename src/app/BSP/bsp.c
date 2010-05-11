/*
*********************************************************************************************************
*                                           Atmel ATmega2560/ATMEGA2561
*                                         Board Support Package
*
*                                (c) Copyright 2005, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
* File : BSP.C
* By   : Jean J. Labrosse - Ported to ATMEGA2560/ATMEGA2561 by Ib Havn, 2010
*
* 2010-04-12, IHA: 	BSP_Int0_Disable(void) and BSP_Int0_Enable(INT8U clear_pending) implemented.
* 2010-04-14, IHA:  SW_in(INT8U sw) implemented.
* 2010-04-14a, IHA:  SW_in(INT8U sw) bug solved - sorry!
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_InitTickISR(void);
static  void  BSP_InitInt0ISR(void);
static  void  LED_Init(void);


/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    LED_Init();

    BSP_InitTickISR();
    BSP_InitInt0ISR();
}


/*
*********************************************************************************************************
*                                        SETUP THE TICK RATE
*
* Note(s): 1) OCR0 = CPU_CLK_FREQ / (2 * Prescaler * OC_freq) - 1
*          2) The equation actually performs rounding by multiplying by 2, adding 1 and then divising by 2
*             in integer math, this is equivalent to adding 0.5
*********************************************************************************************************
*/

static  void  BSP_InitTickISR (void)
{
    TCCR0A  |= _BV(WGM01);            									/* Set TIMER0 to CTC Mode              */
    TCCR0B |= _BV(CS02) | _BV(CS00); 				   					/* Set TIMER0 prescaler CLK/1024       */
    TCNT0  =    0;                                                      /* Start TCNT at 0 for a new cycle                          */
    OCR0A = CPU_CLK_FREQ / OS_TICKS_PER_SEC / 1024 - 1;
    TIFR0  |= _BV(OCF0A);                                               /* Clear  TIMER0 compare Interrupt Flag                     */
    TIMSK0 |= _BV(OCIE0A);                                              /* Enable TIMER0 compare Interrupt                          */
}

/*
*********************************************************************************************************
*                                        SETUP INT0 interrupt
* Interrupt on rising edge of INT0
*********************************************************************************************************
*/

static void  BSP_InitInt0ISR (void)
{
	EICRA |= _BV(ISC00) | _BV(ISC01);
	EIMSK |= _BV(INT0);
}

/**
*********************************************************************************************************
* Disable INT0 interrupt
*********************************************************************************************************
*/
void  inline BSP_Int0_Disable(void)
{
	EIMSK &= ~_BV(INT0);
}

/**
********************************************************************************************************
* Enable INT0 interrupt
* \param clear_pending true: clears pending INT0 interrupt before enabling
*********************************************************************************************************
*/
void  inline BSP_Int0_Enable(uint8_t clear_pending)
{
	if (clear_pending)
	{
		EIFR &= ~_BV(INTF0); // Clear INT0 Flag
	}
	EIMSK |= _BV(INT0);
}

/**
********************************************************************************************************
* Read the status of a switch connected to PORT D.
* \param sw switch to read the status of. [1..8]
* \return 0: switch is not activated, <>0: switch is activated.
*********************************************************************************************************
*/
INT8U  SW_in(INT8U sw)
{
	return (PIND & _BV(--sw)) == 0;
}

/*
*********************************************************************************************************
*                                     HANDLE THE TICK INTERRUPT
*
* Note(s): 1) No need to clear the interrupt source since we use output compare mode for Timer #0 and
*             the interrupt is automatically cleared in hardware when the ISR runs.
*********************************************************************************************************
*/

void  BSP_TickISR_Handler(void)
{
    OSTimeTick();
}


/*
*********************************************************************************************************
*                                     HANDLE THE INT0 INTERRUPT
*
* Note(s): 1) No need to clear the interrupt source since we use INT0 and
*             the interrupt is automatically cleared in hardware when the ISR runs.
*********************************************************************************************************
*/

void  BSP_Int0ISR_Handler(void)
{
	OSSemPost(a_sem);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                       uC/Probe, INITIALIZE TIMER
*
* Description: This function initializes a 16 or 32-bit free-running timer for uC/Probe.
*
* Arguments  : none
*
* Note(s)    : none
*********************************************************************************************************
*/
#if OS_PROBE_HOOKS_EN > 0                                               /* Do not compile this section if Probe is disabled         */
void  OSProbe_TmrInit (void)
{
#if (OS_PROBE_TIMER_SEL == 1)
    TCCR1B  =   0x00;                                                   /* Stop timer                                               */
    TCNT1   = 0xFFFF;                                                   /* Setup TCNT                                               */
    TCCR1A  =   0x00;                                                   /* Normal Mode, count up 0x0000 to 0xFFFF                   */
    TCCR1B  =   0x04;                                                   /* Start timer, prescale by 256                             */
#endif

#if (OS_PROBE_TIMER_SEL == 3)
    TCCR3B  =   0x00;                                                   /* Stop timer                                               */
    TCNT3   = 0xFFFF;                                                   /* Setup TCNT                                               */
    TCCR3A  =   0x00;                                                   /* Normal Mode, count up 0x0000 to 0xFFFF                   */
    TCCR3B  =   0x04;                                                   /* Start timer, prescale by 256                             */
#endif
}
#endif                                                                  /* #if OS_PROBE_HOOKS_EN > 0                                */

/*$PAGE*/
/*
*********************************************************************************************************
*                                       uC/Probe, TIMER READ
*
* Description: This function reads a 16-bit free-running timer for uC/Probe.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/
#if OS_PROBE_HOOKS_EN > 0                                               /* Do not compile this section if Probe is disabled         */
CPU_INT32U  OSProbe_TmrRd (void)
{
    INT16U  cnts;


#if (OS_PROBE_TIMER_SEL == 1)
    cnts    = (INT16U)TCNT1;                                            /* Read current counts of Timer #1                          */
#endif

#if (OS_PROBE_TIMER_SEL == 3)
    cnts    = (INT16U)TCNT3;                                            /* Read current counts of Timer #3                          */
#endif

    return ((INT32U)cnts);
}
#endif                                                                  /* #if OS_PROBE_HOOKS_EN > 0                                */


/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

static  void  LED_Init (void)
{
    DDRA  = 0xFF;                                                       /* All PORTA pins are outputs                               */
    LED_Off(0);                                                         /* Turn ON all the LEDs                                     */
}

/*
*********************************************************************************************************
*                                             LED ON
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to control
*                      0    indicates that you want ALL the LEDs to be ON
*                      1    turns ON LED1 on the board
*                      .
*                      .
*                      8    turns ON LED8 on the board
*********************************************************************************************************
*/

void  LED_On (INT8U led)
{
#if OS_CRITICAL_METHOD == 3                                             /* Allocate storage for CPU status register                 */
    OS_CPU_SR  cpu_sr = 0;
#endif


    OS_ENTER_CRITICAL();
    switch (led) {
        case 0:
             PORTA  =  0x00;
             break;

        case 1:
             PORTA &= ~0x01;
             break;

        case 2:
             PORTA &= ~0x02;
             break;

        case 3:
             PORTA &= ~0x04;
             break;

        case 4:
             PORTA &= ~0x08;
             break;

        case 5:
             PORTA &= ~0x10;
             break;

        case 6:
             PORTA &= ~0x20;
             break;

        case 7:
             PORTA &= ~0x40;
             break;

        case 8:
             PORTA &= ~0x80;
             break;
    }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
*                                             LED OFF
*
* Description : This function is used to control any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to turn OFF
*                      0    indicates that you want ALL the LEDs to be OFF
*                      1    turns OFF LED1 on the board
*                      .
*                      .
*                      8    turns OFF LED8 on the board
*********************************************************************************************************
*/

void  LED_Off (INT8U led)
{
#if OS_CRITICAL_METHOD == 3                                             /* Allocate storage for CPU status register                 */
    OS_CPU_SR  cpu_sr = 0;
#endif


    OS_ENTER_CRITICAL();
    switch (led) {
        case 0:
             PORTA  =  0xFF;
             break;

        case 1:
             PORTA |=  0x01;
             break;

        case 2:
             PORTA |=  0x02;
             break;

        case 3:
             PORTA |=  0x04;
             break;

        case 4:
             PORTA |=  0x08;
             break;

        case 5:
             PORTA |=  0x10;
             break;

        case 6:
             PORTA |=  0x20;
             break;

        case 7:
             PORTA |=  0x40;
             break;

        case 8:
             PORTA |=  0x80;
             break;
    }
    OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
*                                        LED TOGGLE
*
* Description : This function is used to toggle the state of any or all the LEDs on the board.
*
* Arguments   : led    is the number of the LED to toggle
*                      0    indicates that you want ALL the LEDs to toggle
*                      1    Toggle LED0 on the board
*                      .
*                      .
*                      8    Toggle LED7 on the board
*********************************************************************************************************
*/

void  LED_Toggle (INT8U led)
{
#if OS_CRITICAL_METHOD == 3                                             /* Allocate storage for CPU status register                 */
    OS_CPU_SR  cpu_sr = 0;
#endif


    OS_ENTER_CRITICAL();
    switch (led) {
        case 0:
             PORTA ^=  0xFF;
             break;

        case 1:
             PORTA ^=  0x01;
             break;

        case 2:
             PORTA ^=  0x02;
             break;

        case 3:
             PORTA ^=  0x04;
             break;

        case 4:
             PORTA ^=  0x08;
             break;

        case 5:
             PORTA ^=  0x10;
             break;

        case 6:
             PORTA ^=  0x20;
             break;

        case 7:
             PORTA ^=  0x40;
             break;

        case 8:
             PORTA ^=  0x80;
             break;
    }
    OS_EXIT_CRITICAL();
}
