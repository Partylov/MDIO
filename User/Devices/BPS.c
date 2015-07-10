#include "BPS.h"
#include "devices.h"
#include "RS485.h"
#include "crc.h"


// НЕТ ОПИСАНИЯ ПРОТОКОЛА ВЗАИМОДЕЙСТВИЯ С ПРИбОРОМ

// поиск БВК при включении контроллера
void FindBPS(uint8_t	addr)
{
	u8 i;
	uint16_t  crc;
	TxBufModbus[0] = addr;// Адрес на шине Modbus.
	TxBufModbus[1] = 0x02;// Код команды.
	crc = CRC16(TxBufModbus, 2); // Подсчет CRC16.
	TxBufModbus[3] = (0xff00&crc)>>8;
	TxBufModbus[2] = 0x00ff&crc;
	for (i=0; i < 8; i++) RxBufModbus[i] = 0; // Чистим буфер приемника.
	// Заранее заряжаем количество байт на передачу в DMA.
	SetRS485_Transmit;
	DMA_SetCurrDataCounter(DMA1_Channel2,BPSfindLen); // Количество передаваемых байт.
	Device.AnswerLength = 8;													// Длина ожидаемого ответа
	SetTimeoutRS485(TimeoutWaitAnswer); // Таймаут ожидания ответа 
	DMA_Cmd(DMA1_Channel2, ENABLE); // Включаем DMA передатчи
}	

