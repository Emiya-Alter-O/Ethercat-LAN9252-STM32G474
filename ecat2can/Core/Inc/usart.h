/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */


typedef struct 
{
    uint8_t buffer[128][256];
    uint8_t len[128];
    uint8_t read_ptr;
    uint8_t write_ptr;
    uint8_t full_state;
} print_fifo_type;

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern uint8_t shell_rev;

extern print_fifo_type print_fifo;

extern int print_my(const char *fmt, ...);
extern void putchar_my(char ch);

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

