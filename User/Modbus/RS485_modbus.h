/* Define to prevent recursive inclusion -------------------------------------*/
typedef unsigned char u8;

/* Includes ------------------------------------------------------------------*/
//#include "stm32f2xx.h"
//#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Modbus_TX_IRQHandler(void); // Вызов обработчика прерывания Modbus.
void Modbus_RX_IRQHandler(void); // Вызов обработчика прерывания Modbus.

void TIM3_Configuration(void);
void Modbus_Configuration(void);
void Modbus_DMA_Configuration(void);
void Modbus_InitNewAsk(void); // Инициализация обмена по Modbus в main.

extern u8 ErrorModuleMODBUS[]; // Признаки ошибок связи с модулями.
