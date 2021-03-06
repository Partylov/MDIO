/* Define to prevent recursive inclusion -------------------------------------*/
typedef unsigned char u8;

/* Includes ------------------------------------------------------------------*/
//#include "stm32f2xx.h"
//#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Modbus_TX_IRQHandler(void); // ����� ����������� ���������� Modbus.
void Modbus_RX_IRQHandler(void); // ����� ����������� ���������� Modbus.

void TIM3_Configuration(void);
void Modbus_Configuration(void);
void Modbus_DMA_Configuration(void);
void Modbus_InitNewAsk(void); // ������������� ������ �� Modbus � main.

extern u8 ErrorModuleMODBUS[]; // �������� ������ ����� � ��������.
