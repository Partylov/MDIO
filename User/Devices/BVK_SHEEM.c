#include "BVK_SHEEM.h"
#include "devices.h"
#include "RS485.h"
#include "crc.h"
#include "can.h"

u8 BVK_SHEEMinf[BVK_SHEEM_BuLen];
u8 BVK_SHEEMust[8] = {defPWMkey1,defPWMkey2,defPWMkey3,defPWMkey4,defPWMkey5,defPWMkey6,defPWMkey7,defPWMkey8};	

// поиск БВК при включении контроллера
void FindBVK_SHEEM(uint8_t	addr)
{
	Ask2b(addr, 0x02, 0x08, TimeoutWaitAnswer);
}

// запрос команды 01
void BVK_SHEEM_Ask01()
{
	StopRS485timer();
	Device.SendCommand = BVK_SHEEM_Ask01;
	Device.WorkAnswer = Device_WorkDataAfterReading;
	Device.NoAnswer = Device_NoAnswerAfterReading;
	Device.Buf = BVK_SHEEMinf+4;
	Device.NextCommand = BVK_SHEEM_Ask02;
	Ask2b(Device.Address, 0x01, 12, TimeoutWaitAnswer);
}
// запрос команды 02
void BVK_SHEEM_Ask02()
{
	StopRS485timer();
	Device.SendCommand = BVK_SHEEM_Ask02;
	Device.WorkAnswer = Device_WorkDataAfterReading;
	Device.NoAnswer = Device_NoAnswerAfterReading;
	Device.Buf = BVK_SHEEMinf;
	Device.NextCommand = BVK_SHEEM_Send05;
	Ask2b(Device.Address, 0x02, 8, TimeoutWaitAnswer);
}

// команда 05 - запись скважности ШИМ для ключей
void BVK_SHEEM_Send05()
{
	StopRS485timer();
	Device.SendCommand = BVK_SHEEM_Send05;
	Device.WorkAnswer = Device_WorkDataAfterWriting;
	Device.NoAnswer = Device_NoAnswerAfterWriting;
	Device.NextCommand = BVK_SHEEM_Send06;
	CommandNb(Device.Address, 0x05, 4, TimeoutWaitAnswer, BVK_SHEEMust, 8);
}

// команда 06 - запись уставок токов ключей
void BVK_SHEEM_Send06()
{
	u8 i;
	StopRS485timer();
	Device.SendCommand = BVK_SHEEM_Send06;
	Device.WorkAnswer = Device_WorkDataAfterWriting;
	Device.NoAnswer = Device_NoAnswerAfterWriting;
	Device.NextCommand = BVK_SHEEM_Ask01;
	//test-------------------------{
	for (i = 0; i<8; i++) // БВК-ШИМ не ответил на эту команду
	{
		BVK_SHEEMust[i] = 2;
	}
	//-------------------------test}
	CommandNb(Device.Address, 0x06, 4, TimeoutWaitAnswer, BVK_SHEEMust, 8);
}

//void BVK_SHEEMPause()
//{
//	StopRS485timer();
//	DMA_Cmd(DMA1_Channel2, DISABLE); 
//	DMA_Cmd(DMA1_Channel3, DISABLE);
//	Device.NoAnswer = BVK_SHEEM_Ask01;
//	SetTimeoutRS485(100);
//}
u8 BVK_SHEEM_WorkCAN(u8 command)
{
	u8 i;
	switch (command)
	{
		case 0: // задание скважности ШИМ  - команда 5 по RS485
			for (i = 0; i<4; i++)
			 BVK_SHEEMust[i] = RxMessage.Data[i]; // ключи 1-4
			TxMessage.Data[0] = BVK_SHEEMinf[4]; 	// в ответе информация с команды 1 для БВК по RS485
			TxMessage.Data[1] = BVK_SHEEMinf[5]; 
			TxMessage.Data[2] = BVK_SHEEMinf[6]; 
			TxMessage.Data[3] = BVK_SHEEMinf[7]; 
			TxMessage.Data[4] = BVK_SHEEMinf[0];  // в ответе информация с команды 2 для БВК по RS485 
			TxMessage.Data[5] = BVK_SHEEMinf[1]; 
			TxMessage.Data[6] = BVK_SHEEMinf[2]; 
			TxMessage.Data[7] = Device.Error&0x01;  
		break;
		case 1:
			for (i = 4; i<8; i++)// задание скважности ШИМ  - команда 5 по RS485
			 BVK_SHEEMust[i] = RxMessage.Data[i-4];	// ключи 5-8
			TxMessage.Data[0] = BVK_SHEEMinf[8]; 		// в ответе информация с команды 1 для БВК по RS485
			TxMessage.Data[1] = BVK_SHEEMinf[9]; 
			TxMessage.Data[2] = BVK_SHEEMinf[10]; 
			TxMessage.Data[3] = BVK_SHEEMinf[11];
			TxMessage.Data[3] = BVK_SHEEMinf[3]; 		// в ответе информация с команды 2 для БВК по RS485 
			TxMessage.Data[5] = Device.Error&0x01;  
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

