#include "Joystick.h"
#include "devices.h"
#include "RS485.h"
#include "crc.h"
#include "can.h"


u8 JoystickInf[JoystickBuLen];

// поиск БВК при включении контроллера
void FindJoystick(uint8_t	addr)
{
	Ask2b(addr, 0x41, 0x07, TimeoutWaitAnswer);
}	

// запрос команды 0x41
void Joystick_Ask0x41()
{
	StopRS485timer();
	Device.SendCommand = Joystick_Ask0x41;
	Device.WorkAnswer = Device_WorkDataAfterReading;
	Device.NoAnswer = Device_NoAnswerAfterReading;
	Device.Buf = JoystickInf;
	Device.NextCommand = Joystick_Ask0x41;
	Ask2b(Device.Address, 0x41, 3+4, TimeoutWaitAnswer);
}


u8 Joystick_WorkCAN(u8 command)
{
	TxMessage.Data[0] = JoystickInf[0]; // Данные от джойстика по первому каналу.
	TxMessage.Data[1] = JoystickInf[1]; //
	TxMessage.Data[2] = JoystickInf[2]; //
	TxMessage.Data[3] = Device.Error&0x01; 
	TxMessage.Data[4] = 0; //
	TxMessage.Data[5] = 0; //
	TxMessage.Data[6] = 0; //
	TxMessage.Data[7] = 0; // Резерв.
	return 8;// длина пакета
}
//void JoystickPause()
//{
//	StopRS485timer();
//	Device.RequestCounter = 0;
//	Device.SendCommand = Joystick_Ask0x41;
//	Device.NoAnswer = Device_NoAnswerAfterReading;
//	SetTimeoutRS485(100);
//}
