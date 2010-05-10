/*
*********************************************************************************************************
*                                           Atmel ATmega128
*                                         Board Support Package
*
*                                (c) Copyright 2005, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
* File : BSP.H
* By   : Jean J. Labrosse
*
* 2010-04-12, IHA: 	BSP_Int0_Disable(void) and BSP_Int0_Enable(INT8U clear_pending) implemented.
* 2010-04-14, IHA:  SW_in(INT8U sw) implemented.
*********************************************************************************************************
*/

extern OS_EVENT *a_sem; // Pointer to a semaphore
/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  CPU_CLK_FREQ                  F_CPU


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_Init(void);

void  BSP_TickISR(void);
void  BSP_TickISR_Handler(void);
void  BSP_Int0ISR_Handler(void);
void  BSP_Int0_Disable(void);
void  BSP_Int0_Enable(INT8U clear_pending);

void  LED_On(INT8U led);
void  LED_Off(INT8U led);
void  LED_Toggle(INT8U led);

INT8U  SW_in(INT8U sw);

void App_TaskSwHook(void); // TODO Fjernes efter test
void App_TaskCreateHook(OS_TCB *ptcb);
void App_TaskDelHook(OS_TCB *ptcb);
void App_TaskIdleHook(void);
void App_TaskStatHook(void);
void App_TCBInitHook(OS_TCB *ptcb);
void App_TimeTickHook(void);
