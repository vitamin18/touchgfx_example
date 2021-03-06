/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include <stdbool.h>

#include "../../oled160128/Inc/oled160128.h"
//#include "app_touchgfx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern bool buttonState;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId ButtonTaskHandle;
osTimerId signalVsyncHandle;
osSemaphoreId screen_frame_buffer_semHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void touchgfx_signalVSyncTimer(void);

void TransferComplete(DMA_HandleTypeDef *_hdma);
void TransferError(DMA_HandleTypeDef *_hdma);
bool PollingButton(uint8_t state);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartButtonTask(void const * argument);
void signalVsyncCallback(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of screen_frame_buffer_sem */
  osSemaphoreDef(screen_frame_buffer_sem);
  screen_frame_buffer_semHandle = osSemaphoreCreate(osSemaphore(screen_frame_buffer_sem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of signalVsync */
  osTimerDef(signalVsync, signalVsyncCallback);
  signalVsyncHandle = osTimerCreate(osTimer(signalVsync), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ButtonTask */
  osThreadDef(ButtonTask, StartButtonTask, osPriorityIdle, 0, 256);
  ButtonTaskHandle = osThreadCreate(osThread(ButtonTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
	HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_ERROR_CB_ID, TransferError);
	HAL_EnableFMCMemorySwapping();
	OLED_Init_160128RGB();
	osTimerStart(signalVsyncHandle, 10);
	MX_TouchGFX_Process();
  /* Infinite loop */
  for(;;)
  {

  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartButtonTask */
/**
* @brief Function implementing the ButtonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartButtonTask */
void StartButtonTask(void const * argument)
{
  /* USER CODE BEGIN StartButtonTask */
//	bool state = false;
  /* Infinite loop */
  for(;;)
  {
//  	if((PollingButton(GPIO_PIN_SET) == true) && state == false){
//  		state = true;
//  	}else if((PollingButton(GPIO_PIN_RESET) == true) && state == true){
//  		buttonState = true;
//  		state = false;
//  	}
  }
  /* USER CODE END StartButtonTask */
}

/* signalVsyncCallback function */
void signalVsyncCallback(void const * argument)
{
  /* USER CODE BEGIN signalVsyncCallback */
	touchgfx_signalVSyncTimer();
  /* USER CODE END signalVsyncCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void TransferComplete(DMA_HandleTypeDef *_hdma)
{
	osSemaphoreRelease(screen_frame_buffer_semHandle);
}

void TransferError(DMA_HandleTypeDef *_hdma)
{

}

bool PollingButton(uint8_t state)
{
	for(uint8_t i = 0; i < 10; i++){
		if(HAL_GPIO_ReadPin(BUTTON_USER_GPIO_Port, BUTTON_USER_Pin) != state){
			return false;
		}
		osDelay(1);
	}
	return true;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
