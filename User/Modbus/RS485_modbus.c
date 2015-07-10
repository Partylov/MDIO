// Главный блок (RS485 Modbus RTU).

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
//#include "stm32f2xx_dma.h"
//#include "stm32f2xx_tim.h"
#include "RS485_Modbus_DMA_config.h"
//#include "RS485_modbus.h"
#include "crc.h"
#include "devices.h"

/*  ОПРЕДЕЛЕНИЯ------------------------------------------*/
#define NUM_MODULES_MODBUS 1 // Количество модулей на шине Modbus.
#define MAX_ERROR_MODBUS	5 // Максимальное количество ошибок связи с модулем.

// Управление Прием-передача RS485.
//#define SetRS485_Receive    GPIO_WriteBit(GPIOG, GPIO_Pin_6,  (BitAction)(0)); // Прием RS485
//#define SetRS485_Transmit   GPIO_WriteBit(GPIOG, GPIO_Pin_6,  (BitAction)(1)); // Передача RS485
#define SetRS485_Receive    GPIO_ResetBits(GPIOA, GPIO_Pin_8); // Прием RS485 
#define SetRS485_Transmit   GPIO_SetBits(GPIOA, GPIO_Pin_8); // Передача RS485 

typedef unsigned char u8;

/* Локальные Переменные------------------------------------------*/
u8 RxBufModbus[256];     //буфер драйвера GSM модема на прием.
u8 TxBufModbus[256];     //буфер драйвера GSM модема на передачу.
u8 CurrentModuleMODBUS; // Номер опрашиваемого модуля.
u8 ErrorModuleMODBUS [NUM_MODULES_MODBUS]; // Признаки ошибок связи с модулями.
u8 ModbusDelayedStart; // Признак паузы перед стартом передатчика.

TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;

/* Private function prototypes -----------------------------------------------*/
//void CalculateCRC16(u8 *buf, u8 count);
//void SetTimeoutRS485(uint16_t timeout);
void ModbusNewAsk (u8 delayed);
void ModbusNoAnswerError (void);

extern Device_TypeDef Device;// устройство, подключенное к преобразователю

//**************************************************************************************************
// Инициализация обмена по Modbus в main.
//**************************************************************************************************
void Modbus_InitNewAsk (void) // Инициализация обмена по Modbus в main.
{
	u8 i;
	CurrentModuleMODBUS = NUM_MODULES_MODBUS; // Номер опрашиваемого модуля, чтобы опрос начался с первого модуля.
	for (i=0; i < NUM_MODULES_MODBUS; ++i) ErrorModuleMODBUS[i] = MAX_ERROR_MODBUS; // Заполняем признаки ошибок связи ошибками.
	// Организация нового запроса Modbus.
	ModbusNewAsk(0);
}

//**************************************************************************************************
// Организация нового запроса Modbus.
//**************************************************************************************************
void ModbusNewAsk (u8 delayed)
{
	Device.SendCommand();
}

//**************************************************************************************************
// Обработка отсутствия ответа.
//**************************************************************************************************
void ModbusNoAnswerError (void)
{
	ErrorModuleMODBUS[CurrentModuleMODBUS]++; // Признак ошибки связи[Номер опрашиваемого модуля].
	if (ErrorModuleMODBUS[CurrentModuleMODBUS] > MAX_ERROR_MODBUS) ErrorModuleMODBUS[CurrentModuleMODBUS] = MAX_ERROR_MODBUS;
}

//**************************************************************************************************
// Установка Таймаута и запуск таймера.
//**************************************************************************************************
void SetTimeoutRS485(uint16_t timeout)
{
		TIM_SetAutoreload(TIM3,timeout); // Перезаряжаем таймаут
/*		TIM3->EGR = TIM_PSCReloadMode_Immediate; // Перезарядить немедленно */
		TIM_GenerateEvent(TIM3,TIM_EventSource_Update); // Перезарядить немедленно
  	TIM_Cmd(TIM3, ENABLE); // Start TIM3
}
//**************************************************************************************************
// Обработчик прерывания Таймера (таймаут).
//**************************************************************************************************
void TIM3_IRQHandler(void)
{
  TIM_Cmd(TIM3, DISABLE); // Disable TIM3
	TIM3->SR = 0; // Сбросить флаг прерывания.
	Device.NoAnswer();
}
void StopRS485timer(void)
{
	TIM_Cmd(TIM3, DISABLE); 
}
//**************************************************************************************************
// Обработчик прерывания Передатчика (DMA).
//**************************************************************************************************
//void USARTx_DMA_TX_IRQHandler(void)
void Modbus_TX_IRQHandler(void)
{
	if(DMA_GetITStatus(USARTx_TX_DMA_IT_TCIF)) // бит transfer complete
	{
		DMA_ClearITPendingBit(USARTx_TX_DMA_IT_TCIF); // Сброс флага прерывания
		DMA_ClearITPendingBit(USARTx_TX_DMA_FLAG_HTIF); // Сброс флага прерывания
		DMA_ClearITPendingBit(USARTx_TX_DMA_FLAG_GLIF); // Сброс флага прерывания
//		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE); //Разрешаем побайтную передачу для завершения передачи.
		USART_ITConfig(USARTx, USART_IT_TC, ENABLE); //Разрешаем побайтную передачу для завершения передачи.
		if (Device.AnswerLength>0)
		{
			DMA_SetCurrDataCounter(USARTx_RX_DMA_CHANNEL,Device.AnswerLength); // Количество принимаемых байт.
			DMA_Cmd(USARTx_RX_DMA_CHANNEL, ENABLE); //Разрешаем прием
		}
		DMA_Cmd(USARTx_TX_DMA_CHANNEL, DISABLE); //Запрет передачи
	}
}
/*****************************************************************************
 * 
 ******************************************************************************/
void USARTx_IRQHandler(void)
{
  //Transmission complete interrupt
  if(USART_GetITStatus(USARTx, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USARTx, USART_IT_TC);
		SetRS485_Receive	// Прием RS485 
//		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE); //Запрещаем побайтную передачу
		USART_ITConfig(USARTx, USART_IT_TC, DISABLE); //Запрещаем побайтную передачу
  }
}

//**************************************************************************************************
// Обработчик прерывания Приемника (DMA).
//**************************************************************************************************
//void USARTx_DMA_RX_IRQHandler(void)
void Modbus_RX_IRQHandler(void)
{
//	uint8_t tmp;
	if(DMA_GetITStatus(USARTx_RX_DMA_IT_TCIF)) // бит stream2 transfer complete
	{
		DMA_ClearITPendingBit(USARTx_RX_DMA_IT_TCIF); // Сброс флага прерывания
		DMA_Cmd(USARTx_RX_DMA_CHANNEL, DISABLE); //Запрет приема
		DMA_Cmd(USARTx_TX_DMA_CHANNEL, DISABLE); //Запрет передачи
		// Обработка ответа.
		Device.WorkAnswer();
	}
}

/*******************************************************************************
* Function Name  : TIM3_Configuration
* Description    : Configure Timer3
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM3_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
 	/* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM3_TimeBaseStructure); // Начальное заполнение всех полей структуры
	TIM_UpdateRequestConfig(TIM3,TIM_UpdateSource_Regular); // Прерывание только при истечении времени счета
  TIM3_TimeBaseStructure.TIM_Prescaler = 48000-1;
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM3_TimeBaseStructure.TIM_Period = 10000;
// 	TIM3_TimeBaseStructure.TIM_ClockDivision = 0;
// 	TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);
 /* Enable the TIM3_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Modbus_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

	// Конфигурация управления Приемопередатчиком.
	// RS485 переключение прием/передача.
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	SetRS485_Receive	// Прием RS485 
//	SetRS485_Transmit // Передача RS485 

	// Конфигурация USART.
  RCC_AHBPeriphClockCmd(USARTx_TX_GPIO_CLK,ENABLE); // Тактирование Порта TX. Все GPIO сидят на шине AHB1.
  RCC_AHBPeriphClockCmd(USARTx_RX_GPIO_CLK,ENABLE); // Тактирование Порта RX. Все GPIO сидят на шине AHB1.
	USARTx_CLK_INIT(USARTx_CLK,ENABLE);// Тактирование USART.

	GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

//  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
// 	USART_InitStructure.USART_StopBits = USART_StopBits_2;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USARTx, &USART_InitStructure);
//  USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE); // Разрешить прерывание при обнаружении тишины в линии.
//  USART_ITConfig(USARTx, USART_IT_TC, ENABLE); // Разрешить прерывание при завершении передачи.
////  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
////  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
////  USART_ClearFlag(USARTx,USART_FLAG_TC);
////  USART_Cmd(USARTx, ENABLE);
}

/*******************************************************************************
* Function Name  : USART_DMA_Configuration
* Description    : Configure DMA for USART
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Modbus_DMA_Configuration(void)
{
//  DMA_InitTypeDef  DMA_InitStructure;
	DMA_InitTypeDef  DMA_InitStructureRX, DMA_InitStructureTX;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHBPeriphClockCmd(USARTx_DMAx_CLK,ENABLE); // Оба DMA сидят на шине AHB.

  // ПРИЕМНИК
  /* USART1_RX_DMA_Channel Config */
  DMA_DeInit(USARTx_RX_DMA_CHANNEL);
  DMA_InitStructureRX.DMA_PeripheralBaseAddr = USARTx_RDR_ADDRESS; //(uint32_t) (&(USARTx->RDR))
  DMA_InitStructureRX.DMA_MemoryBaseAddr = (uint32_t)RxBufModbus;
  DMA_InitStructureRX.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructureRX.DMA_BufferSize = 10;
  DMA_InitStructureRX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructureRX.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructureRX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructureRX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructureRX.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructureRX.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructureRX.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructureRX.DMA_FIFOMode = DMA_FIFOMode_Disable; // F2
//	DMA_InitStructureRX.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; // F2
//	DMA_InitStructureRX.DMA_MemoryBurst = DMA_MemoryBurst_Single; // F2
//	DMA_InitStructureRX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // F2
  DMA_InitStructureRX.DMA_M2M = DMA_M2M_Disable; // F1,F0
	DMA_Init(USARTx_RX_DMA_CHANNEL, &DMA_InitStructureRX);

   /* USART1_RX_DMA_Channel enable */
//	DMA_Cmd(USARTx_RX_DMA_CHANNEL, ENABLE);

   /* Enable USART1_RX_DMA_Channel Transfer complete interrupt */
  DMA_ITConfig(USARTx_RX_DMA_CHANNEL, DMA_IT_TC, ENABLE);

   /* Enable the USART1_RX_DMA_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 15;//0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


   // ПЕРЕДАТЧИК.
   /* USART1_TX_DMA_Channel Config */
	DMA_DeInit(USARTx_TX_DMA_CHANNEL);
  DMA_InitStructureTX.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS; //(uint32_t) (&(USARTx->TDR))
  DMA_InitStructureTX.DMA_MemoryBaseAddr = (uint32_t)TxBufModbus;
  DMA_InitStructureTX.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructureTX.DMA_BufferSize = 10;
  DMA_InitStructureTX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructureTX.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructureTX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructureTX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructureTX.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructureTX.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructureTX.DMA_Priority = DMA_Priority_High;
  DMA_InitStructureTX.DMA_M2M = DMA_M2M_Disable; // F1,F0
//	DMA_InitStructureTX.DMA_FIFOMode = DMA_FIFOMode_Disable; // F2
//	DMA_InitStructureTX.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; // F2
//	DMA_InitStructureTX.DMA_MemoryBurst = DMA_MemoryBurst_Single; // F2
//	DMA_InitStructureTX.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // F2
	DMA_Init(USARTx_TX_DMA_CHANNEL, &DMA_InitStructureTX);

   /* USART1_TX_DMA_Channel enable */
//  DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);

   /* Enable USART1_TX_DMA_Channel Transfer complete interrupt */
  DMA_ITConfig(USARTx_TX_DMA_CHANNEL, DMA_IT_TC, ENABLE);

   /* Enable the USART1_TX_DMA_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 15;//0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	// Стартуем USART и DMA.
  USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
  USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
  USART_Cmd(USARTx, ENABLE);

	// Прерывание USART побайтовой передачи для корректного переключения 
	// приемопередатчика RS485.
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn; //канал
  NVIC_InitStructure.NVIC_IRQChannelPriority = 15;//0; //приоритет
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //включаем канал
  NVIC_Init(&NVIC_InitStructure); //инициализируем
  USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE); // Запрещаем побайтовый прием
  USART_ITConfig(USARTx, USART_IT_TXE, DISABLE); // Запрещаем побайтовую передачу
}
