/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nr_micro_shell.h"

uint8_t shell_rev;

print_fifo_type print_fifo = 
{
  .read_ptr = 0,
  .write_ptr = 0
};

static char overlength_info[] = "print length over 256B\r\n";
static char print_fail_info[] = "print fail\r\n";
static char fifo_full_info[] = "fifo is full\r\n";
// static char malloc_fail_info[] = "print malloc is fail\r\n";

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 4000000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel1;
    hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel2;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
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
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4|GPIO_PIN_5);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint8_t fifo_is_full(print_fifo_type *fifo)
{
  if(fifo->write_ptr >= fifo->read_ptr)
  {
    if(fifo->write_ptr - fifo->read_ptr >= 127)
    {
      return 0;
    }
    else if(fifo->write_ptr - fifo->read_ptr >= 126)
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
  else
  {
    if(fifo->read_ptr - fifo->write_ptr <= 1)
    {
      return 0;
    }
    else if(fifo->read_ptr - fifo->write_ptr <= 2)
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
}
uint16_t print_cnt = 0;

int print_my(const char *fmt, ...)
{
  int len;
  va_list args;
  char temp_data[256];

  va_start(args,fmt);
  len = vsprintf(temp_data,fmt,args);
  va_end(args);

  print_cnt++;

  print_fifo.full_state = fifo_is_full(&print_fifo);


  if(print_fifo.full_state == 2)
  {
    if(len > 0 && len < 256)
    {
      memcpy(&print_fifo.buffer[print_fifo.write_ptr][0], temp_data, len);        
      print_fifo.len[print_fifo.write_ptr] = len;
      print_fifo.write_ptr++;
      print_fifo.write_ptr &= 0X7F;        
    }
    else if(len <= 0)
    {
      memcpy(&print_fifo.buffer[print_fifo.write_ptr][0],print_fail_info,sizeof(print_fail_info));
      //print_fifo.buffer[print_fifo.write_ptr] = (uint8_t *)print_fail_info;
      print_fifo.len[print_fifo.write_ptr] = sizeof(print_fail_info);
      print_fifo.write_ptr++;
      print_fifo.write_ptr &= 0X7F;   
    }
    else if(len >=  256)
    {
      memcpy(&print_fifo.buffer[print_fifo.write_ptr][0],overlength_info,sizeof(overlength_info));
      //print_fifo.buffer[print_fifo.write_ptr] = (uint8_t *)overlength_info;
      print_fifo.len[print_fifo.write_ptr] = sizeof(overlength_info);
      print_fifo.write_ptr++;
      print_fifo.write_ptr &= 0X7F;   
    }
  }
  else if(print_fifo.full_state == 1)
  {
    memcpy(&print_fifo.buffer[print_fifo.write_ptr][0],fifo_full_info,sizeof(fifo_full_info));
    //print_fifo.buffer[print_fifo.write_ptr] = (uint8_t *)fifo_full_info;
    print_fifo.len[print_fifo.write_ptr] = sizeof(fifo_full_info);
    print_fifo.write_ptr++;
    print_fifo.write_ptr &= 0X7F;
  }

  return len;
}

void putchar_my(char ch)
{
  // static char temp_ch;

  // temp_ch = ch;
  
  // if(!fifo_is_full(&print_fifo))
  // {
  //   print_fifo.buffer[print_fifo.write_ptr] = (uint8_t *)&temp_ch;     
  //   print_fifo.len[print_fifo.write_ptr] = 1;
  //   print_fifo.write_ptr++;
  // }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {
    shell(shell_rev);//print_my("usart irq\r\n");
    HAL_UART_Receive_IT(&huart1, &shell_rev, 1);
  }
}

/* USER CODE END 1 */
