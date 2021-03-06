#include "devices.h"
#include "RS485.h"
#include "crc.h"
#include "BVK.h"
#include "Joystick.h"
#include "BVK_SHEEM.h"
#include "BPS.h"

void	NoCommand(){}
uint8_t delayed = 0;// ������� ������������� ����� ����� ��������� �����c��

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
		{	//��� �� ������, ���� ���
			Device.Address = 0x21;
			Device.RequestCounter = 0;
			Device.Type = BVK;
			FindBVK(Device.Address);
		}
		else
			if (Device.Address==0x30)
			{//��� �� ������, ���� ��������
				Device.Address = 0x31;
				Device.RequestCounter = 0;
				Device.Type = Joystick;
				FindBVK(Device.Address);
			}
			else
				if (Device.Address==0x35)
				{//�������� �� ������, ���� ���_���
					Device.Address = 0x41;
					Device.RequestCounter = 0;
					Device.Type = BVK_SHEEM;
					FindBVK(Device.Address);
				}
				if (Device.Address>=0x50)
				{
					// ����������, ������������ � ��������������� �� ������� 
					//(����� ���������� �� ���� �������� � ����� �� ��� ������� ��� ������ ������ ����������)
					Device.Type = NoConnectedDevice;
					// ...
					// ...
					// ...
				}
				else
				{
					// ������ ���� �� ���������� �� ��������� �������
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
							// ������, ������������� ����������
							// ...
							// ...
							// ...
						break;		
					}
				}
	}//if (Device.RequestCounter>2)
	else
	{
		// ����������� ����� � ��������� ������ � ��� �� �������
		SetDelay(TimeoutBeforeRequest);// ����� ������� ����������� 100��
		switch (Device.Type)
		{
			case BPS:
				DMA_SetCurrDataCounter(DMA1_Channel2,BPSfindLen); // ���������� ������������ ����.
			break;
			case BVK:
				DMA_SetCurrDataCounter(DMA1_Channel2,BVKfindLen); // ���������� ������������ ����.
			break;
			case Joystick:
				DMA_SetCurrDataCounter(DMA1_Channel2,JoystickFindLen); // ���������� ������������ ����.
			break;
			case BVK_SHEEM:
				DMA_SetCurrDataCounter(DMA1_Channel2,BVK_SHEEMfindLen); // ���������� ������������ ����.
			break;
			default:
				// ������, ������������� ����������
				// ...
				// ...
				// ...
			break;		
		}
	}
}


// ��������� ������, ����������� �� ����������
void	FindDeviceAnswer()
{
	StopRS485timer();
	// ���������� CRC
	if (!TestCRC1(RxBufModbus, Device.AnswerLength))
	{// ���������� �������
		switch (Device.Type)
		{
			case BPS:
			break;
			case BVK:
				// ��������� ��������� ���
				BVKinf[0] = RxBufModbus[2]; BVKinf[1] = RxBufModbus[3]; 
				BVKinf[2] = RxBufModbus[4]; BVKinf[3] = RxBufModbus[5];
				Device.CAN_ID = BVK_CAN_ID;
				Device.RequestCounter = 0;
				Device.WorkCAN = BVK_WorkCAN;
				BVK_Ask01();
			break;
			case Joystick:
				// ��������� ��������� ���������
				JoystickInf[0] = RxBufModbus[2]; JoystickInf[1] = RxBufModbus[3]; 
				JoystickInf[2] = RxBufModbus[4];
				Device.CAN_ID = Joystick_CAN_ID;
				Device.WorkCAN = Joystick_WorkCAN;
				Device.RequestCounter = 0;
				Joystick_Ask0x41();
			break;
			case BVK_SHEEM:
				// ��������� ��������� ���-���
				BVK_SHEEMinf[0] = RxBufModbus[2]; BVK_SHEEMinf[1] = RxBufModbus[3]; 
				BVK_SHEEMinf[2] = RxBufModbus[4]; BVK_SHEEMinf[3] = RxBufModbus[5];
				Device.CAN_ID = BVK_SHEEM_CAN_ID;
				Device.WorkCAN = BVK_SHEEM_WorkCAN;
				Device.RequestCounter = 0;
				BVK_SHEEM_Ask01();// �������� ����� ���� ������ � ��������
			break;
			default:
				// ������, ������������� ����������
				// ...
				// ...
				// ...
			break;		
		}
	}
	else
	{
		FindDeviceCommand(); // ���������� �����
	}
}


// ���������� �� ��������, ���� ������������� ����� ����� ��������� �������
void	FindDeviceNoAnswer()
{
	if (delayed)
	{	// ������ ����� �����
		delayed = 0;
		DMA_Cmd(DMA1_Channel2, ENABLE); // �������� DMA ���������
		SetTimeoutRS485(TimeoutWaitAnswer); // ������� �������� ������ 100��
	}
	else
	{ // ������ ����� ���������� ������
		FindDeviceCommand();
	}
}

void InitDevice(Device_TypeDef *dev)
{
	dev->Type = BPS;	// ������� ����� � ���
	dev->Address	= 0x11; 									//����� ���, � �������� �������� �����
	dev->SendCommand = FindDeviceCommand;
	dev->WorkAnswer = FindDeviceAnswer;
	dev->NoAnswer = FindDeviceNoAnswer;
	dev->CommandNumber	= 0;
	dev->AnswerLength = 0;
	dev->RequestCounter = 0;
	dev->Error = 1;
}

//----------------------------------------------------------------------------------------------------
//				������� ������������ ��� �������� � ��������� ������� �� ������ ���������
//----------------------------------------------------------------------------------------------------
// ������: �����, ���_�������, CRC16
void	Ask2b(u8	addr, u8 command, u8 AnswLen, u16 Timeout)
{
	u8 i;
	uint16_t  crc;
	TxBufModbus[0] = addr;				// ����� �� ���� Modbus.
	TxBufModbus[1] = command;			// ��� �������.
	crc = CRC16(TxBufModbus, 2);	// ������� CRC16.
	TxBufModbus[3] = (0xff00&crc)>>8;
	TxBufModbus[2] = 0x00ff&crc;
	for (i=0; i < AnswLen; i++) RxBufModbus[i] = 0; // ������ ����� ���������.
	// ������� �������� ���������� ���� �� �������� � DMA.
	SetRS485_Transmit;
	DMA_SetCurrDataCounter(DMA1_Channel2,4); 	// ���������� ������������ ����.
	Device.AnswerLength = AnswLen;						// ����� ���������� ������
	SetTimeoutRS485(Timeout); 								// ������� �������� ������ 
	DMA_Cmd(DMA1_Channel2, ENABLE); 					// �������� DMA ���������
}

// ��������  CRC � ������� �������������� �� ��� � Modbus
uint8_t	TestCRC1(volatile uint8_t *bytes, int len)
{
	// ���������� ������� CRC
	bytes[len] = bytes[len-2];
	bytes[len-2] = bytes[len-1];
	bytes[len-1] = bytes[len];
	return CRC16(RxBufModbus, Device.AnswerLength);
}
// ��������� ������ �� ������� Ask2b
void Device_WorkDataAfterReading()
{
	u8 i;
	StopRS485timer(); 
	if (!TestCRC1(RxBufModbus, Device.AnswerLength))
	{
		for (i = 0; i<Device.AnswerLength-4; i++)
		{
			Device.Buf[i] = RxBufModbus[2+i];	// ��������� ���������� ������
		}
		Device.Error = 0;
		Device.RequestCounter = 0;
		Device.NextCommand();// ����������� ��������� �������
	}
	else
	{
		Device.RequestCounter++;
		if (Device.RequestCounter>2)
		{
			for (i = 0; i<Device.AnswerLength-4; i++)
			{
				Device.Buf[i] = DeviceErrReading;	// ��� ������
			}
			Device.Error = 1;
			Device.RequestCounter = 0;
			Device.NextCommand();// ����������� ��������� �������
		}
		else
			Device.SendCommand();// ��������� ������
	}
}
// �������� ��� ���������� ������ �� ������� Ask2b
void  Device_NoAnswerAfterReading()
{
	u8 i;
	DMA_Cmd(DMA1_Channel2, DISABLE); 
	DMA_Cmd(DMA1_Channel3, DISABLE);
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		// DR � SR ��� ����������� stm32F030 ������ �� �����������, �� ��� stm32F207 �������� ORE
		// �������� ������ ������ DR � SR ������ ������ � ���� �������, � �������� USART_IT_ORE ����� �� ����������
		(void)USART1->RDR;
		(void)USART1->ISR;
		USART_ClearITPendingBit(USART1,USART_IT_ORE);
	}
	Device.RequestCounter++;
	if (Device.RequestCounter>2)
	{
		for (i = 0; i<Device.AnswerLength-4; i++)
		{
			Device.Buf[i] = DeviceErrReading;	// ��� ������
		}
		Device.Error = 1;
		Device.RequestCounter = 0;
		Device.NextCommand();// ����������� ��������� �������
	}
	else
			Device.SendCommand();// ��������� ������
	
}

// �������� ��� ���������� ������ �� ������� ������ 
void  Device_NoAnswerAfterWriting()
{
	DMA_Cmd(DMA1_Channel2, DISABLE); 
	DMA_Cmd(DMA1_Channel3, DISABLE);
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		// DR � SR ��� ����������� stm32F030 ������ �� �����������, �� ��� stm32F207 �������� ORE
		// �������� ������ ������ DR � SR ������ ������ � ���� �������, � �������� USART_IT_ORE ����� �� ����������
		(void)USART1->RDR;
		(void)USART1->ISR;
		USART_ClearITPendingBit(USART1,USART_IT_ORE);
	}
	Device.RequestCounter++;
	if (Device.RequestCounter>2)
	{
		Device.Error = 1;		
		Device.RequestCounter = 0;
		Device.NextCommand();// ����������� ��������� �������
	}
	else
			Device.SendCommand();// ��������� ������
	
}

// ��������� ������ �� ������� ������
void Device_WorkDataAfterWriting()
{
	StopRS485timer(); 
	if (!TestCRC1(RxBufModbus, Device.AnswerLength))
	{
		Device.Error = 0;	
		Device.RequestCounter = 0;
		Device.NextCommand();// ����������� ��������� �������
	}
	else
	{
		Device.RequestCounter++;
		if (Device.RequestCounter>2)
		{
			Device.Error = 1;	
			Device.RequestCounter = 0;
			Device.NextCommand();// ����������� ��������� �������
		}
		else
			Device.SendCommand();// ��������� ������
	}
}

void	CommandNb(u8	addr, u8 command, u8 AnswLen, u16 Timeout, u8 *data, u8 data_len)
{
	u8 i;
	uint16_t  crc;
	StopRS485timer();
	TxBufModbus[0] = addr;				// ����� �� ���� Modbus.
	TxBufModbus[1] = command;									// ��� �������.
	for (i = 2; i<data_len+2; i++)
	{
		TxBufModbus[i] = data[i-2];
	}
	crc = CRC16(TxBufModbus, data_len+2);						// ������� CRC16.
	TxBufModbus[2+data_len+1] = (0xff00&crc)>>8;
	TxBufModbus[2+data_len] = 0x00ff&crc;
	SetRS485_Transmit;
	DMA_SetCurrDataCounter(DMA1_Channel2,2+data_len+2); 	// ���������� ������������ ����.
	Device.AnswerLength = AnswLen;										// ����� ���������� ������ 
	SetTimeoutRS485(TimeoutWaitAnswer); 				// ������� �������� ������ 
	DMA_Cmd(DMA1_Channel2, ENABLE); 						// �������� DMA ���������
}
