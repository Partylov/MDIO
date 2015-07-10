#include "devices.h"
#include "RS485.h"
#include "crc.h"
#include "BVK.h"
#include "Joystick.h"
#include "BVK_SHEEM.h"
#include "BPS.h"

void	NoCommand(){}
uint8_t delayed = 0;// признак необходимости паузы перед следующим запроcом

void SetDelay(uint16_t del)	
{
	delayed = 1;
	SetTimeoutRS485(del);
}

void	FindDeviceCommand()
{
	Device.RequestCounter++;
	SetRS485_Transmit
	if (Device.RequestCounter>2)
	{
		Device.Address++;
		if (Device.Address==0x20)
		{	//БПС не найден, ищем БВК
			Device.Address = 0x21;
			Device.RequestCounter = 0;
			Device.Type = BVK;
			FindBVK(Device.Address);
		}
		else
			if (Device.Address==0x30)
			{//БВК не найден, ищем Джойстик
				Device.Address = 0x31;
				Device.RequestCounter = 0;
				Device.Type = Joystick;
				FindBVK(Device.Address);
			}
			else
				if (Device.Address==0x35)
				{//Джойстик не найден, ищем БВК_ШИМ
					Device.Address = 0x41;
					Device.RequestCounter = 0;
					Device.Type = BVK_SHEEM;
					FindBVK(Device.Address);
				}
				if (Device.Address>=0x50)
				{
					// Устройства, подключенные к преобразователю не найдены 
					//(можно оповестить об этом верхушку и ждать от нее команды для нового поиска устройства)
					Device.Type = NoConnectedDevice;
					// ...
					// ...
					// ...
				}
				else
				{
					// запрос того же устройства со следующим адресом
					Device.RequestCounter = 0;
					switch (Device.Type)
					{
						case BPS:
							FindBPS(Device.Address);
						break;
						case BVK:
							FindBVK(Device.Address);
						break;
						case Joystick:
							FindJoystick(Device.Address);
						break;
						case BVK_SHEEM:
							FindBVK_SHEEM(Device.Address);
						break;
						default:
							// ошибка, перезагрузить контроллер
							// ...
							// ...
							// ...
						break;		
					}
				}
	}//if (Device.RequestCounter>2)
	else
	{
		// Выдерживаем паузу и повторяем запрос с тем же адресом
		SetDelay(TimeoutBeforeRequest);// Пауза запуска передатчика 100мс
		switch (Device.Type)
		{
			case BPS:
				DMA_SetCurrDataCounter(DMA1_Channel2,BPSfindLen); // Количество передаваемых байт.
			break;
			case BVK:
				DMA_SetCurrDataCounter(DMA1_Channel2,BVKfindLen); // Количество передаваемых байт.
			break;
			case Joystick:
				DMA_SetCurrDataCounter(DMA1_Channel2,JoystickFindLen); // Количество передаваемых байт.
			break;
			case BVK_SHEEM:
				DMA_SetCurrDataCounter(DMA1_Channel2,BVK_SHEEMfindLen); // Количество передаваемых байт.
			break;
			default:
				// ошибка, перезагрузить контроллер
				// ...
				// ...
				// ...
			break;		
		}
	}
}


// Обработка ответа, полученного от устройства
void	FindDeviceAnswer()
{
	StopRS485timer();
	// поправляем CRC
	if (!TestCRC1(RxBufModbus, Device.AnswerLength))
	{// устройство найдено
		switch (Device.Type)
		{
			case BPS:
			break;
			case BVK:
				// сохраняем состояние БВК
				BVKinf[0] = RxBufModbus[2]; BVKinf[1] = RxBufModbus[3]; 
				BVKinf[2] = RxBufModbus[4]; BVKinf[3] = RxBufModbus[5];
				Device.CAN_ID = BVK_CAN_ID;
				Device.RequestCounter = 0;
				Device.WorkCAN = BVK_WorkCAN;
				BVK_Ask01();
			break;
			case Joystick:
				// сохраняем состояние Джойстика
				JoystickInf[0] = RxBufModbus[2]; JoystickInf[1] = RxBufModbus[3]; 
				JoystickInf[2] = RxBufModbus[4];
				Device.CAN_ID = Joystick_CAN_ID;
				Device.WorkCAN = Joystick_WorkCAN;
				Device.RequestCounter = 0;
				Joystick_Ask0x41();
			break;
			case BVK_SHEEM:
				// сохраняем состояние БВК-ШИМ
				BVK_SHEEMinf[0] = RxBufModbus[2]; BVK_SHEEMinf[1] = RxBufModbus[3]; 
				BVK_SHEEMinf[2] = RxBufModbus[4]; BVK_SHEEMinf[3] = RxBufModbus[5];
				Device.CAN_ID = BVK_SHEEM_CAN_ID;
				Device.WorkCAN = BVK_SHEEM_WorkCAN;
				Device.RequestCounter = 0;
				BVK_SHEEM_Ask01();// Начинаем новый цикл обмена с прибором
			break;
			default:
				// ошибка, перезагрузить контроллер
				// ...
				// ...
				// ...
			break;		
		}
	}
	else
	{
		FindDeviceCommand(); // продолжаем поиск
	}
}


// Устройство не ответило, либо выдерживалась пауза перед следующим опросом
void	FindDeviceNoAnswer()
{
	if (delayed)
	{	// Запрос после паузы
		delayed = 0;
		DMA_Cmd(DMA1_Channel2, ENABLE); // Включаем DMA передатчи
		SetTimeoutRS485(TimeoutWaitAnswer); // Таймаут ожидания ответа 100мс
	}
	else
	{ // Запрос после отсутствия ответа
		FindDeviceCommand();
	}
}

void InitDevice(Device_TypeDef *dev)
{
	dev->Type = BPS;	// Начинаю поиск с БПС
	dev->Address	= 0x11; 									//Адрес БПС, с которого начинаем поиск
	dev->SendCommand = FindDeviceCommand;
	dev->WorkAnswer = FindDeviceAnswer;
	dev->NoAnswer = FindDeviceNoAnswer;
	dev->CommandNumber	= 0;
	dev->AnswerLength = 0;
	dev->RequestCounter = 0;
	dev->Error = 1;
}

//----------------------------------------------------------------------------------------------------
//				Функции используемые для запросов и обработки ответов от разных устройств
//----------------------------------------------------------------------------------------------------
// Запрос: адрес, код_команды, CRC16
void	Ask2b(u8	addr, u8 command, u8 AnswLen, u16 Timeout)
{
	u8 i;
	uint16_t  crc;
	TxBufModbus[0] = addr;				// Адрес на шине Modbus.
	TxBufModbus[1] = command;			// Код команды.
	crc = CRC16(TxBufModbus, 2);	// Подсчет CRC16.
	TxBufModbus[3] = (0xff00&crc)>>8;
	TxBufModbus[2] = 0x00ff&crc;
	for (i=0; i < AnswLen; i++) RxBufModbus[i] = 0; // Чистим буфер приемника.
	// Заранее заряжаем количество байт на передачу в DMA.
	SetRS485_Transmit;
	DMA_SetCurrDataCounter(DMA1_Channel2,4); 	// Количество передаваемых байт.
	Device.AnswerLength = AnswLen;						// Длина ожидаемого ответа
	SetTimeoutRS485(Timeout); 								// Таймаут ожидания ответа 
	DMA_Cmd(DMA1_Channel2, ENABLE); 					// Включаем DMA передатчи
}

// проверяю  CRC с байтами расположенными не как в Modbus
uint8_t	TestCRC1(volatile uint8_t *bytes, int len)
{
	// поправляем корявую CRC
	bytes[len] = bytes[len-2];
	bytes[len-2] = bytes[len-1];
	bytes[len-1] = bytes[len];
	return CRC16(RxBufModbus, Device.AnswerLength);
}
// обработка ответа на команду Ask2b
void Device_WorkDataAfterReading()
{
	u8 i;
	StopRS485timer(); 
	if (!TestCRC1(RxBufModbus, Device.AnswerLength))
	{
		for (i = 0; i<Device.AnswerLength-4; i++)
		{
			Device.Buf[i] = RxBufModbus[2+i];	// сохраняем полученные данные
		}
		Device.Error = 0;
		Device.RequestCounter = 0;
		Device.NextCommand();// запрашиваем следующую команду
	}
	else
	{
		Device.RequestCounter++;
		if (Device.RequestCounter>2)
		{
			for (i = 0; i<Device.AnswerLength-4; i++)
			{
				Device.Buf[i] = DeviceErrReading;	// код ошибки
			}
			Device.Error = 1;
			Device.RequestCounter = 0;
			Device.NextCommand();// запрашиваем следующую команду
		}
		else
			Device.SendCommand();// повторяем запрос
	}
}
// действия при отсутствии ответа на команду Ask2b
void  Device_NoAnswerAfterReading()
{
	u8 i;
	DMA_Cmd(DMA1_Channel2, DISABLE); 
	DMA_Cmd(DMA1_Channel3, DISABLE);
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		// DR и SR для контроллера stm32F030 читать не обязательно, но для stm32F207 сбросить ORE
		// помогает только чтение DR и SR причем именно в этом порядке, а наоборот USART_IT_ORE можно не сбрасывать
		(void)USART1->RDR;
		(void)USART1->ISR;
		USART_ClearITPendingBit(USART1,USART_IT_ORE);
	}
	Device.RequestCounter++;
	if (Device.RequestCounter>2)
	{
		for (i = 0; i<Device.AnswerLength-4; i++)
		{
			Device.Buf[i] = DeviceErrReading;	// код ошибки
		}
		Device.Error = 1;
		Device.RequestCounter = 0;
		Device.NextCommand();// запрашиваем следующую команду
	}
	else
			Device.SendCommand();// повторяем запрос
	
}

// действия при отсутствии ответа на команду записи 
void  Device_NoAnswerAfterWriting()
{
	DMA_Cmd(DMA1_Channel2, DISABLE); 
	DMA_Cmd(DMA1_Channel3, DISABLE);
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		// DR и SR для контроллера stm32F030 читать не обязательно, но для stm32F207 сбросить ORE
		// помогает только чтение DR и SR причем именно в этом порядке, а наоборот USART_IT_ORE можно не сбрасывать
		(void)USART1->RDR;
		(void)USART1->ISR;
		USART_ClearITPendingBit(USART1,USART_IT_ORE);
	}
	Device.RequestCounter++;
	if (Device.RequestCounter>2)
	{
		Device.Error = 1;		
		Device.RequestCounter = 0;
		Device.NextCommand();// запрашиваем следующую команду
	}
	else
			Device.SendCommand();// повторяем запрос
	
}

// обработка ответа на команду записи
void Device_WorkDataAfterWriting()
{
	StopRS485timer(); 
	if (!TestCRC1(RxBufModbus, Device.AnswerLength))
	{
		Device.Error = 0;	
		Device.RequestCounter = 0;
		Device.NextCommand();// запрашиваем следующую команду
	}
	else
	{
		Device.RequestCounter++;
		if (Device.RequestCounter>2)
		{
			Device.Error = 1;	
			Device.RequestCounter = 0;
			Device.NextCommand();// запрашиваем следующую команду
		}
		else
			Device.SendCommand();// повторяем запрос
	}
}

void	CommandNb(u8	addr, u8 command, u8 AnswLen, u16 Timeout, u8 *data, u8 data_len)
{
	u8 i;
	uint16_t  crc;
	StopRS485timer();
	TxBufModbus[0] = Device.Address;				// Адрес на шине Modbus.
	TxBufModbus[1] = command;									// Код команды.
	for (i = 2; i<data_len+2; i++)
	{
		TxBufModbus[i] = data[i-2];
	}
	crc = CRC16(TxBufModbus, data_len+2);						// Подсчет CRC16.
	TxBufModbus[2+data_len+1] = (0xff00&crc)>>8;
	TxBufModbus[2+data_len] = 0x00ff&crc;
	SetRS485_Transmit;
	DMA_SetCurrDataCounter(DMA1_Channel2,2+data_len+2); 	// Количество передаваемых байт.
	Device.AnswerLength = AnswLen;										// Длина ожидаемого ответа 
	SetTimeoutRS485(TimeoutWaitAnswer); 				// Таймаут ожидания ответа 
	DMA_Cmd(DMA1_Channel2, ENABLE); 						// Включаем DMA передатчи
}
