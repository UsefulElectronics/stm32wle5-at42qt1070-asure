/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define     RTOS_BYTE_POOL_SIZE     8192
#define     UI_THREAD_STACK_SIZE    1024
//#define     RTOS_BYTE_POOL_SIZE     8192
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
VOID *threadx_memory;
uint8_t thread_stack_0[UI_THREAD_STACK_SIZE] = {0};
uint8_t thread_stack_1[UI_THREAD_STACK_SIZE] = {0};
TX_THREAD thread_0;
TX_THREAD thread_1;

TX_QUEUE queue_0;
TX_SEMAPHORE semaphore_0;
TX_MUTEX mutex_0;
TX_EVENT_FLAGS_GROUP event_flags_0;
TX_BYTE_POOL byte_pool_0;
TX_BLOCK_POOL block_pool_0;

TX_SEMAPHORE external_gpio_semaphore;
TX_MUTEX 	 periodic_read_mutex;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void thread_new_state_update(ULONG thread_input);
static void thread_periodic_read(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
	/* Allocate the stack for thread 0.  */

	/* Create the main thread.  */
	tx_thread_create(&thread_0, 			// Thread Pointer: This is a pointer to the TX_THREAD structure that represents the thread being created. This structure holds information about the thread, such as its stack, entry function, priority, and state.
					"thread 0", 			// Name: A string name for the thread, which can be used for identification and debugging purposes.
					thread_new_state_update,// Entry Function: The entry function is the starting point of execution for the thread. It's a function that takes a single argument of type ULONG and returns VOID.
					0, 						// Entry Input: This is the input parameter passed to the entry function when the thread starts.
					thread_stack_0,			// Stack Start: The starting address of the thread's stack. The stack is used for storing local variables, function call information, and other data related to thread execution.
					UI_THREAD_STACK_SIZE, 	// Stack Size: The size of the stack in bytes.
					1, 						// Priority: The priority of the thread, which determines its scheduling order relative to other threads in the system. Higher-priority threads are scheduled before lower-priority threads.
					1, 						// Preemption Threshold: This parameter is used to set the preemption threshold for the thread. It determines the minimum priority level at which the thread can be preempted by other threads. Setting it to TX_NO_PREEMPTION allows the thread to run without being preempted.
					TX_NO_TIME_SLICE,		// Time Slice: The time slice is the maximum amount of time (in ticks) that the thread can run before it is preempted if there are other ready threads of equal priority.
					TX_DONT_START);			// Auto Start: Specifies whether the thread should start automatically after creation (TX_AUTO_START) or if it should be manually started later.

	tx_thread_create(&thread_1, 			// Thread Pointer: This is a pointer to the TX_THREAD structure that represents the thread being created. This structure holds information about the thread, such as its stack, entry function, priority, and state.
					"thread 1", 			// Name: A string name for the thread, which can be used for identification and debugging purposes.
					thread_periodic_read,  // Entry Function: The entry function is the starting point of execution for the thread. It's a function that takes a single argument of type ULONG and returns VOID.
					0, 						// Entry Input: This is the input parameter passed to the entry function when the thread starts.
					thread_stack_1,			// Stack Start: The starting address of the thread's stack. The stack is used for storing local variables, function call information, and other data related to thread execution.
					UI_THREAD_STACK_SIZE, 	// Stack Size: The size of the stack in bytes.
					1, 						// Priority: The priority of the thread, which determines its scheduling order relative to other threads in the system. Higher-priority threads are scheduled before lower-priority threads.
					1, 						// Preemption Threshold: This parameter is used to set the preemption threshold for the thread. It determines the minimum priority level at which the thread can be preempted by other threads. Setting it to TX_NO_PREEMPTION allows the thread to run without being preempted.
					TX_NO_TIME_SLICE,		// Time Slice: The time slice is the maximum amount of time (in ticks) that the thread can run before it is preempted if there are other ready threads of equal priority.
					TX_DONT_START);

    tx_thread_resume(&thread_0);
    tx_thread_resume(&thread_1);

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/* Azure RTOS thread entry function */
static void thread_new_state_update(ULONG thread_input)
{
    /* Your thread code here */
	uint8_t key = 0;

    // Create semaphore
    tx_semaphore_create(&external_gpio_semaphore, "gpio interrupt", 0);

    tx_mutex_create(&periodic_read_mutex, "periodic read", TX_NO_WAIT);

    while (1)
    {
        // Wait for semaphore
    	if (tx_semaphore_get(&external_gpio_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
    	{
    		key = main_key_status_read();
    	}


        /* Thread actions */
//    	HAL_GPIO_TogglePin(LED_PIN_GPIO_Port, LED_PIN_Pin);
//
//    	key = main_key_status_read();
//
//        tx_thread_sleep(5); // Example: Sleep for 100 ticks
    }
}

static void thread_periodic_read(ULONG thread_input)
{
	/* Your thread code here */
	uint8_t key = 0;

	uint8_t register_address  = 3;

	while (1)
	{

		main_i2c_transmit(0x1B << 1,
						&register_address,
						1);

		main_i2c_receive(0x1B << 1,
						&key,
						1);


		/* Thread actions */
//    	HAL_GPIO_TogglePin(LED_PIN_GPIO_Port, LED_PIN_Pin);
//
//    	key = main_key_status_read();
//
        tx_thread_sleep(100); // Example: Sleep for 100 ticks
	}
}

/* USER CODE END 1 */
