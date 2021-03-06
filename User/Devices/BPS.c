#include "BPS.h"
#include "devices.h"
#include "RS485.h"
#include "crc.h"


// ��� �������� ��������� �������������� � ��������

// ����� ��� ��� ��������� �����������
void FindBPS(uint8_t	addr)
{
	u8 i;
	uint16_t  crc;
	TxBufModbus[0] = addr;// ����� �� ���� Modbus.
	TxBufModbus[1] = 0x02;// ��� �������.
	crc = CRC16(TxBufModbus, 2); // ������� CRC16.
	TxBufModbus[3] = (0xff00&crc)>>8;
	TxBufModbus[2] = 0x00ff&crc;
	for (i=0; i < 8; i++) RxBufModbus[i] = 0; // ������ ����� ���������.
	// ������� �������� ���������� ���� �� �������� � DMA.
	SetRS485_Transmit;
	DMA_SetCurrDataCounter(DMA1_Channel2,BPSfindLen); // ���������� ������������ ����.
	Device.AnswerLength = 8;													// ����� ���������� ������
	SetTimeoutRS485(TimeoutWaitAnswer); // ������� �������� ������ 
	DMA_Cmd(DMA1_Channel2, ENABLE); // �������� DMA ���������
}	

