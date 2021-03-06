/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "tim.h"

/* Extern variables	----------------------------------------------------------*/
extern void initializeLED_IO			(void);
extern void start_Thread_LED			(void);
extern void Thread_LED					(void const *argument);
extern void initializeKeypad_IO 		(void);
extern void start_Thread_Keypad 		(void);
extern void Thread_Keypad 				(void const *argument);
extern void initializeSegment_IO 		(void);
extern void start_Thread_Segment 		(void);
extern void Thread_Segment				(void const *argument);
extern void initializeADC_IO 			(void);
extern void start_Thread_ADC 			(void);
extern void Thread_ADC 					(void const *argument);
extern void initializeAccelerometer_IO 	(void);
extern void start_Thread_Accelerometer 	(void);
extern void Thread_Accelerometer 		(void const *argument);

extern osThreadId tid_Thread_LED;
extern osThreadId tid_Thread_Keypad;
extern osThreadId tid_Thread_Segment;
extern osThreadId tid_Thread_Accelerometer;
extern osThreadId tid_Thread_ADC;

/* Private variables ---------------------------------------------------------*/
int flagTim;
int flagTim2;
int flagTim3;

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * Main function
  */
 int main (void) {

  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */

  SystemClock_Config();                     /* Configure the System Clock     */

	Timer_Config();
	/* User codes goes here*/
  //initializeLED_IO();                       /* Initialize LED GPIO Buttons    */
	initializeKeypad_IO();										/* Initialize Keypad GPIO Buttons */
	initializeADC_IO ();											/* Initialize ADC                 */
	initializeAccelerometer_IO();							/* Initialize Accelerometer       */
	initializeSegment_IO();										/* Initialize Keypad GPIO         */
  //start_Thread_LED();                       /* Create LED thread              */
	start_Thread_Keypad();										/* Create Keypad thread           */
	start_Thread_ADC();                       /* Create ADC thread              */
	start_Thread_Accelerometer();             /* Create Accelerometer thread    */
	start_Thread_Segment();             		  /* Create Segment thread          */	
	flagTim = 0;
	flagTim2 = 0;
	/* User codes ends here*/
	osKernelStart();                          /* start thread execution         */

}
