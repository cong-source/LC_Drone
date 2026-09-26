/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */

/**
 * @brief  printf 閲嶅畾鍚戝埌 USART1锛堥樆濉炲彂閫侊�??
 * @note   鎵€�???printf 杈撳嚭閫氳繃姝ゅ嚱鏁板彂閫佸�?? USART1_TX (PB6)
 *         娉㈢壒鐜?115200�??? 瀛楄妭绾﹂渶 0.087ms
 *         濡傛�?? USART1 姝ｅ湪杞�??�?? UART2 鐨勬暟鎹紝printf 浼氱瓑寰呴攣閲婃�??
 */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
    return (ch);
}

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART1_ENABLE();

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* ================================================================
 *  UART 閫忎紶妯″潡
 *  ================================================================
 *
 *  纭欢鎷撴墤:
 *    USART1 (PB6=TX, PB7=RX) 鈫愨�?? 澶栭儴閬ユ帶�???
 *    USART2 (PA2=TX, PA3=RX) 鈫愨�?? 钃濈墮妯″潡
 *
 *  鏁版嵁娴?
 *    閬ユ帶鍣?�???UART1 RX �???杞�?? �???UART2 TX �???钃濈墮妯″潡
 *    钃濈墮妯″潡 �???UART2 RX �???杞�?? �???UART1 TX �???閬ユ帶鍣?
 *
 *  鍒濆鍖栨祦�???
 *    1. MX_USART1_UART_Init() / MX_USART2_UART_Init()  閰嶇疆纭欢鍙傛�??
 *    2. main() 涓皟鐢?HAL_UART_Receive_IT() 鍚姩涓柇鎺ユ�??
 *    3. 涓诲惊鐜笉鏂皟鐢?UART_Forward_Poll() 妫€鏌ュ苟杞�??彂鏁版嵁
 *
 *  ================================================================ */

/* ---- 澶栭儴寮曠敤: main.c 涓畾涔夌殑鎺ユ敹缂撳啿鍖猴紙鍗曞瓧鑺傦�?? ---- */
extern uint8_t buff_uart1[1];
extern uint8_t buff_uart2[1];

/* ---- 閫忎紶鐢ㄥ叏灞€鍙橀�?? ---- */
volatile uint8_t uart1_rx_data = 0;   /* UART1 �??跺埌鐨勫瓧鑺傛暟�???*/
volatile uint8_t uart2_rx_data = 0;   /* UART2 �??跺埌鐨勫瓧鑺傛暟�???*/
volatile uint8_t uart1_rx_flag = 0;   /* UART1 鏈夋柊鏁版嵁鏍囧�??: 0=�??? 1=�???*/
volatile uint8_t uart2_rx_flag = 0;   /* UART2 鏈夋柊鏁版嵁鏍囧�??: 0=�??? 1=�???*/

/**
 * @brief  UART 鎺ユ敹�?�屾垚涓柇鍥炶皟锛圚AL 搴撹嚜鍔ㄨ皟鐢�??
 * @param  huart: 瑙﹀彂涓柇�???UART 鍙ユ焺锛圲SART1 �???USART2�???
 *
 * @note   銆愬叧閿璁★細鎵嬪姩閲嶆柊浣胯兘 RX 涓柇銆?
 *
 *  涓轰粈涔堣鎵嬪姩鎿嶄綔瀵勫瓨鍣ㄨ€屼笉鏄皟鐢?HAL_UART_Receive_IT()�???
 *
 *    HAL_UART_Receive_IT() 鍐呴儴浼氳皟�???__HAL_LOCK(huart) 鑾峰彇閿併€?
 *    浣嗕富寰幆涓�?? HAL_UART_Transmit() 涔熷彲鑳介攣浣忓悓涓€涓?huart锛堜緥濡?
 *    UART_Forward_Poll() 姝ｅ�?? huart1 涓婂彂閫佹暟鎹椂锛孶SART1 �???RX 涓�??
 *    鍒拌揪锛屾�???HAL_UART_Receive_IT 浼氬洜涓洪攣宸茶�?? TX 鎸佹湁鑰岃繑�???HAL_BUSY�???
 *    RXNE 涓柇姘歌繙涓嶄細琚噸鏂版墦寮€锛屽�???**姝婚�??**鈥斺€旀�?? UART 鐨勬帴鏀舵案涔呭仠姝€?
 *
 *  瑙ｅ喅鏂规:
 *    鐩存帴鎿嶄綔 HAL 搴撶殑鍐呴儴鐘舵€佸彉閲?(RxState, pRxBuffPtr, RxXferCount)
 *    鍜屽瘎�?�樺�?? (RXNE 涓柇浣胯兘�???锛屽畬鍏ㄧ粫�???HAL_LOCK 鏈哄埗銆?
 *    RX �???TX 鍦ㄧ‖浠朵笂鏄嫭绔嬬殑鈥斺€斿悓涓€涓?UART 鍙互鍚屾椂�??跺彂�???
 *
 * @note   銆愭敞鎰忎簨椤广�???
 *    1. 杩欓噷鍙噸鏂颁娇鑳戒簡 RXNE (鎺ユ敹鏁版嵁瀵勫瓨鍣ㄩ潪绌轰腑鏂?�???
 *       娌℃湁浣胯兘 PE (鏍￠獙閿? �???ERR (甯ч敊/鍣�??/婧㈠�??) 涓柇銆?
 *       濡傛�?? UART 绾胯矾涓婂嚭鐜板共鎵板鑷村抚閿欒锛孯X 浼氶潤榛樺仠姝�???
 *       鐢熶骇鐜涓缓璁悓鏃朵娇鑳介敊璇腑鏂苟�???HAL_UART_ErrorCallback 涓仮澶嶃€?
 *
 *    2. buff_uart1/2 �???main.c 涓畾涔夌殑鍗曞瓧鑺傜紦鍐插尯锛?
 *       姣忔鏈€澶氱紦�???1 瀛楄妭銆傚揩閫熻繛缁敹鍙戞椂鏁版嵁鍙兘涓㈠け�???
 *       鍗囩骇鏂规: �??圭敤鐜舰缂撳啿�???(ring buffer)�???
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* ---- USART1 �??跺埌 1 瀛楄妭锛堟潵鑷仴鎺у櫒锛?---- */
        uart1_rx_data = buff_uart1[0];   /* 璇诲嚭鏁版嵁 */
        uart1_rx_flag = 1;               /* 閫氱煡涓诲惊�??? 鏈夋暟鎹緟杞�?? */

        /* 鎵嬪姩閲嶆柊浣胯�?? RX锛岀粫杩?HAL_LOCK锛堣瑙佸嚱鏁版敞閲婏級 */
        huart1.RxState = HAL_UART_STATE_BUSY_RX;
        huart1.pRxBuffPtr = buff_uart1;
        huart1.RxXferCount = 1;
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    }
    else if (huart->Instance == USART2)
    {
        /* ---- USART2 �??跺埌 1 瀛楄妭锛堟潵鑷摑鐗欐ā鍧楋級 ---- */
        uart2_rx_data = buff_uart2[0];
        uart2_rx_flag = 1;

        huart2.RxState = HAL_UART_STATE_BUSY_RX;
        huart2.pRxBuffPtr = buff_uart2;
        huart2.RxXferCount = 1;
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    }
/* USART3 handled by raw ISR in stm32f1xx_it.c �?no HAL callback needed */
}

/**
 * @brief  UART 鍙屽悜閫忎紶杞鍑芥暟锛堜富寰幆涓寔缁皟鐢�??
 *
 * @note   銆愯皟鐢ㄦ柟寮忋€憁ain() �???while(1) 涓棤寤舵椂鎸佺画璋冪敤
 *         銆愬姛鑳姐€戞鏌ヤ袱涓?UART 鐨勬帴鏀舵爣蹇楋紝灏嗘敹鍒扮殑�?�楄妭杞�??彂鍒板�???
 *
 *  杞彂瑙勫垯:
 *    UART1 RX (閬ユ帶鍣? �???UART2 TX (钃濈墮妯″潡)
 *    UART2 RX (钃濈墮妯″潡) �???UART1 TX (閬ユ帶鍣?
 *
 * 鈺斺晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晽
 * �??? 銆愰噸瑕?Bug 淇璁板綍 �???绔炴€佹潯浠?(Race Condition)�???           �???
 * 鈺犫晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨暎
 * �???                                                             �???
 * �??? 銆愰敊璇啓娉曪紙宸插簾寮冿級銆?                                      �???
 * �??? if (uart1_rx_flag) {                                       �???
 * �???     HAL_UART_Transmit(&huart2, &uart1_rx_data, 1, 100);    �???
 * �???     uart1_rx_flag = 0;  // �???�???TX 涔嬪悗娓呴浂                �???
 * �??? }                                                           �???
 * �???                                                             �???
 * �??? 銆愰棶棰樺満鏅€戦仴鎺у櫒鍙戦€?"AT" 涓や釜�?�楄妭锛岃摑鐗欏彧鏀跺埌 "A"          �???
 * �???                                                             �???
 * �??? 鏃堕棿绾?                                                     �???
 * �??? 鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€鈹€      �???
 * �??? t1: 'A' 鍒拌�?? UART1 �???ISR �???uart1_rx_flag = 1              �???
 * �??? t2: 涓诲惊鐜�???flag=1锛岃繘鍏?HAL_UART_Transmit()              �???
 * �???     �???寮€濮嬪�?? UART2 鍙戦�???'A'锛堢害闇�?? 1.04ms @9600�???            �???
 * �??? t3: �???TX 鏈熼棿锛?T' 鍒拌�?? UART1                              �???
 * �???     �???ISR �???uart1_rx_flag = 1锛寀art1_rx_data = 'T'       �???
 * �??? t4: HAL_UART_Transmit 杩斿�??                                  �???
 * �??? t5: 鎵ц uart1_rx_flag = 0  �???�???'T' 鐨勬爣蹇楁姽鎺変�??!!!       �???
 * �??? t6: 涓嬩竴杞惊�???flag=0 �???'T' 琚涪寮冿紝姘歌繙涓嶈浆�???             �???
 * �???                                                             �???
 * �??? 銆愭牴鍥犮€慣X 鏄樆濉炴搷浣滐紙~1ms锛夛紝鍦?TX 鏈熼�?? ISR 鍙兘宸茬粡       �???
 * �??? 涓烘柊鏀跺埌鐨勫瓧鑺傝缃簡鏍囧織浣嶃€俆X 缁撴潫鍚庢棤鏉�?�欢娓呴浂锛?            �???
 * �??? 瑕嗙洊浜?ISR 璁剧疆鐨勬爣�???= 鏁版嵁涓㈠け�???                          �???
 * �???                                                             �???
 * �??? 銆愭纭啓娉曪紙褰撳墠浠ｇ爜锛夈€?                                  �???
 * �??? if (uart1_rx_flag) {                                       �???
 * �???     uint8_t data = uart1_rx_data;  // �???鍏堟妸鏁版嵁鎷疯礉鍑烘潵    �???
 * �???     uart1_rx_flag = 0;             // �???绔嬪嵆娓呴櫎鏍囧�??        �???
 * �???     HAL_UART_Transmit(&huart2, &data, 1, 100);  // �???鍙戦�??? �???
 * �??? }                                                           �???
 * �???                                                             �???
 * �??? 銆愪负浠€涔堣繖鏍疯兘瑙ｅ喅�???                                        �???
 * �??? - 鏁版嵁鍜?flag �???TX 涔嬪墠灏卞凡淇濆�??/娓呴�??                        �???
 * �??? - 鍗充�?? TX 鏈熼棿鏂板瓧鑺傚埌杈撅紝ISR 浼氭�?? flag 閲嶆柊璁句负 1          �???
 * �??? - TX 缁撴潫鍚庯紝涓嬩竴杞惊鐜兘姝ｇ�?�妫€娴嬪埌 flag=1锛屼笉浼氫涪�???        �???
 * �??? - data 鏄眬閮ㄥ彉閲忥紝涓嶅彈 ISR 淇�?? uart1_rx_data 鐨勫奖鍝?      �???
 * �???                                                             �???
 * 鈺氣晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨晲鈺愨暆
 */
void UART_Forward_Poll(void)
{
    /* ---- 鏂瑰悜涓�??: UART1(閬ユ帶鍣? �???UART2(钃濈�??) ---- */
    if (uart1_rx_flag)
    {
        uint8_t data = uart1_rx_data;   /* �???绔嬪嵆鎷疯礉锛岄槻姝?ISR �???TX 鏈熼棿瑕嗙洊鏁版�?? */
        uart1_rx_flag = 0;              /* �???�???TX 涔嬪墠娓呴浂锛岄槻姝㈢珵鎬佹潯浠跺鑷翠涪�?�楄�??  */
        HAL_UART_Transmit(&huart2, &data, 1, 100);   /* �???闃诲鍙戦€侊紝�???1ms @9600 */
    }

    /* ---- 鏂瑰悜浜? UART2(钃濈�??) �???UART1(閬ユ帶鍣? ---- */
    if (uart2_rx_flag)
    {
        uint8_t data = uart2_rx_data;
        uart2_rx_flag = 0;
        HAL_UART_Transmit(&huart1, &data, 1, 100);
    }
}

/* USER CODE END 1 */
