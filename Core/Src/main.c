/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bluetooth.h"
#include "App_Drone.h"
#include "IMU.h"
#include "Int_TB6612.h"
#include "Com_PID.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Int_MPU6050.h"
// #include "Int_TOF050C.h"
#include "Int_TOF400F.h"
#include "oled.h"
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

/* USER CODE BEGIN PV */
uint8_t buff_uart1[1];
uint8_t buff_uart2[1];

extern uint16_t MPU_Offset[6];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* 启动TIM1和TIM2的PWM模式 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  /* 启动串口1��??2的接收中��?? */
  HAL_UART_Receive_IT(&huart1, buff_uart1, 1);
  HAL_UART_Receive_IT(&huart2, buff_uart2, 1);

  /* 初始化MPU6050 */
  Int_MPU6050_Init();
  Int_MPU6050_WhoAmI();
  App_Flight_MPU_Offsets();

  Int_TB6612_All_Motor();
  /* 对应第一、 二、三、四*/
  // Int_TB6612_SetPWM(500,500,500,500);
  BT_Control_Angle.throttle = 400;
  BT_Control_Angle.BTpitch  = 0;
  BT_Control_Angle.BTroll   = 0;
  BT_Control_Angle.BTyaw    = 0;

  /* 初始��? TOF400F ��?光测距传感器 (UART3: PB10=TX, PB11=RX, Modbus RTU) */
  Int_TOF400F_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  PID_Reset(pids, 6);
  uint32_t next     = HAL_GetTick() + 10;  /* 固定 10ms 控制节拍（100Hz，匹配 MPU6050 采样率） */
  uint32_t next_dbg = HAL_GetTick() + 200; /* 调试打印节拍 200ms（5Hz），慢速不拖控制环 */
  while (1)
  {
    Bluetooth_Control(); /* 每圈都查，命令不漏 */

    if (HAL_GetTick() >= next) /* 到 10ms 才跑一次控制环 */
    {
      next += 10; /* +=10 避免累积漂移 */

      App_Flight_MPU_DATA();              /* 1. 读 IMU */
      GetAngle(&MPU6050, &Angle, 0.01f); /* 2. 算姿态角（dt=10ms） */
      App_Flight_PID__Control(0.01f);    /* 3. 串级 PID */
      App_Flight_Motor_Control();         /* 4. 混控输出 */
    }

    if (HAL_GetTick() >= next_dbg)         /* 慢速打印，不碰 10ms 控制节拍 */
    {
      next_dbg += 200;
      printf("thr=%d pit=%d rol=%d yaw=%d | P=%d R=%d Y=%d (deg*10)\r\n",
             (int)BT_Control_Angle.throttle,
             (int)BT_Control_Angle.BTpitch,
             (int)BT_Control_Angle.BTroll,
             (int)BT_Control_Angle.BTyaw,
             (int)(Angle.pitch * 10),
             (int)(Angle.roll * 10),
             (int)(Angle.yaw * 10));
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* USER CODE END 3 */
  }
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
