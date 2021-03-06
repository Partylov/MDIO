#include "BVK.h"
#include "devices.h"
#include "RS485.h"
#include "crc.h"
#include "can.h"

u8 BVKinf[BVKbuLen];
u8 BVKkey = cDefBVKkey;	
u8 BVKust[15];	

// ����� ��� ��� ��������� �����������
void FindBVK(uint8_t	addr)
{
	Ask2b(addr, 0x02, 0x08, TimeoutWaitAnswer);
}	

// ������ ������� 01
void BVK_Ask01()
{
	StopRS485timer();
	Device.SendCommand = BVK_Ask01;
	Device.WorkAnswer = Device_WorkDataAfterReading;
	Device.NoAnswer = Device_NoAnswerAfterReading;
	Device.Buf = BVKinf+4;
	Device.NextCommand = BVK_Ask02;
	Ask2b(Device.Address, 0x01, 12, TimeoutWaitAnswer);
}
// ������ ������� 02
void BVK_Ask02()
{
	StopRS485timer();
	Device.SendCommand = BVK_Ask02;
	Device.WorkAnswer = Device_WorkDataAfterReading;
	Device.NoAnswer = Device_NoAnswerAfterReading;
	Device.Buf = BVKinf;
	Device.NextCommand = BVK_Send04;
	Ask2b(Device.Address, 0x02, 8, TimeoutWaitAnswer);
}
// ������� 03 �� ���������, ������ ��� ��� ������������ �������� 04
// ������� 04 - ������ ��������� ������ ��� 15 ������
void BVK_Send04()
{
	u8 i;
	StopRS485timer();
	Device.SendCommand = BVK_Send04;
	Device.WorkAnswer = BVK_Ask01;//BVK_Send06;
	Device.NoAnswer = BVK_Ask01;//BVK_Send06;
	Device.NextCommand = BVK_Ask01;//BVK_Send06;
	for (i = 0; i<15; i++) 
	{
		BVKust[i] = BVKkey;
	}
	CommandNb(0x20/*Device.Address*/, 0x04, 0, TimeoutWaitAnswer, BVKust, 15);
}

// ������� 06 - ������ ������� ����� ������
void BVK_Send06()
{
	//u8 i;
	StopRS485timer();
	Device.SendCommand = BVK_Send06;
	Device.WorkAnswer = Device_WorkDataAfterWriting;
	Device.NoAnswer = Device_NoAnswerAfterWriting;
	Device.NextCommand = BVK_Ask01;
//	//test-------------------------{
//	for (i = 0; i<8; i++) // ���-��� �� ������� �� ��� �������
//	{
//		BVKust[i] = 2;
//	}
//	//-------------------------test}
	CommandNb(Device.Address, 0x06, 4, TimeoutWaitAnswer, BVKust, 8);
}

u8 BVK_WorkCAN(u8 command)
{
	switch (command)
	{
		case 0: // ������ ��������� ������ - ������� 4 �� RS485
			BVKkey = RxMessage.Data[0];
			TxMessage.Data[0] = BVKinf[0]; // � ������ ���������� � ������� 2 ��� ���
			TxMessage.Data[1] = BVKinf[1]; 
			TxMessage.Data[2] = BVKinf[2]; 
			TxMessage.Data[3] = BVKinf[3]; 
			TxMessage.Data[4] = Device.Error&0x01; 
			TxMessage.Data[5] = 0; 
			TxMessage.Data[6] = 0; 
			TxMessage.Data[7] = 0; 
		break;
		case 1:
		break;		
		case 2:
		break;
		case 3:
		break;
		default:
		break;		
	}
	return 8;
}

