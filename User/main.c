/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 22/10/2014 17:58:41
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "stdbool.h"
#include "can.h"
#include "RS485_modbus.h"
//#include "RS485_modbus2.h"
#include "W5500_SPI.h"
#include "W5500_Utils.h"
//#include "main.h"
#include "devices.h"

/* Private variables ---------------------------------------------------------*/
//uint8_t Keys; // Нажатые кнопки (0-ничего не нажато).
uint8_t joystick[3]; // Данные от джойстика по первому каналу.
uint8_t joystick2[3]; // Данные от джойстика по второму каналу.

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

Device_TypeDef 		Device;// устройство, подключенное к преобразователю

int main(void)
{
//	uint8_t addr[20];
	

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Initialize all configured peripherals */
//  MX_GPIO_Init(); // Инициализируем входы и выходы.
//	EXTI_Config(); // Инициализируем прерывания от кнопок.
	
	// Определяем CAN-адрес.
	MyCANaddress = 0x1; // CAN-адрес.
	CAN_Config(); // Настройка связи по CAN.

	// Modbus
	InitDevice(&Device);
	Modbus_Configuration();
	Modbus_DMA_Configuration();
	TIM3_Configuration(); // Таймер обрабатывающий таймаут Modbus.
	//Modbus_InitNewAsk(); // Инициализация обмена по Modbus в main.
	FindBPS(Device.Address);// начинаем поиск с БПС
	
	// Modbus2
//	Modbus_Configuration2();
//	Modbus_DMA_Configuration2();
//	TIM2_Configuration(); // Таймер обрабатывающий таймаут Modbus.
//	Modbus_InitNewAsk2(); // Инициализация обмена по Modbus в main.
	

	/* W5500 configuration ------------------------------------------------------*/
  SPI_Config();
	W5500_Config();
	Reset_W5200();

	Set_network();	
//	addr[0] = W5500_Read(0x18);
//	addr[1] = W5500_Read(0x19);
//	addr[2] = W5500_Read(0x1A);
//	addr[3] = W5500_Read(0x25);
//	addr[4] = W5500_Read(0x26);
//	addr[5] = W5500_Read(0x27);
//	addr[6] = W5500_Read(0x37);
//	addr[7] = W5500_Read(0x38);
//	addr[8] = W5500_Read(0x39);
//	addr[9] = W5500_Read(0x3A);

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN 3 */
  /* Infinite loop */
  while (1)
  {

  }
  /* USER CODE END 3 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
