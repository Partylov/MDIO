/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.3.1
  * @date    17-January-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "stm32f0xx_exti.h"
#include "main.h"

// Функции обработки прерываний от периферии.
#include "RS485_modbus.h"
//#include "RS485_modbus2.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles External lines 12..15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  { // Нажата кнопка Вверх IN1
//		Keys |= 0x1; // Нажатые кнопки.
//		GPIO_SetBits(GPIOA, GPIO_Pin_6); // Подсветка кнопки1.
    /* Clear the EXTI line 12 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  { // Нажата кнопка Вниз IN2
//		Keys |= 0x2; // Нажатые кнопки.
//		GPIO_SetBits(GPIOA, GPIO_Pin_7); // Подсветка кнопки2.
    /* Clear the EXTI line 13 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line13);
  }
  if(EXTI_GetITStatus(EXTI_Line14) != RESET)
  { // Нажата кнопка IN3
//		Keys |= 0x4; // Нажатые кнопки.
//		GPIO_SetBits(GPIOA, GPIO_Pin_8); // Подсветка кнопки3.
    /* Clear the EXTI line 14 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line14);
  }
  if(EXTI_GetITStatus(EXTI_Line15) != RESET)
  { // Нажата кнопка IN4
//		Keys |= 0x8; // Нажатые кнопки.
//		GPIO_SetBits(GPIOA, GPIO_Pin_15); // Подсветка кнопки4.
    /* Clear the EXTI line 15 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line15);
  }
}

//**************************************************************************************************
// Обработчик прерывания DMA1_Channel4_5_6_7:
// UART2_RX, UART2_TX, UART4_RX, UART4_TX
//**************************************************************************************************
void DMA1_Channel2_3_IRQHandler()//void DMA1_Channel4_5_6_7_IRQHandler(void)
{
	// USART4_RX
	Modbus_RX_IRQHandler(); // Вызов обработчика прерывания Modbus.
	// USART4_TX
	Modbus_TX_IRQHandler(); // Вызов обработчика прерывания Modbus.
	// USART2_RX
//	Modbus_RX_IRQHandler2(); // Вызов обработчика прерывания Modbus.
//	// USART2_TX
//	Modbus_TX_IRQHandler2(); // Вызов обработчика прерывания Modbus.
}

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
