#define TimeoutWaitAnswer			10	// 10��
#define TimeoutBeforeRequest	10	// 100��
#define DeviceErrReading	0xff // ����, ������� ����������� ����� ��� ������ ������ ����������

typedef unsigned char u8;
typedef	unsigned int u16;
typedef enum
{
	NoConnectedDevice = 0,
	BPS,
	BVK,
	Joystick,
	BVK_SHEEM	
}DeviceType;

// �������� ��������� ����������, ������������� � ���������������
typedef struct 
{
	DeviceType	Type;								//	��� ����������
	u8					Address;						//	����� ����������	
  void    		(*SendCommand) ();	//	������� ��� ����������
	void    		(*WorkAnswer) ();  	// 	������ �� ����������
	void    		(*NoAnswer) (); 		// 	���������� �� ��������
	u8					CommandNumber;			//  ����� ������������/����������� �������
	u16					AnswerLength;				//  ����� ������������ ������
	u8					RequestCounter; 		//	������� ������� �������� �������
	u8					*Buf;								//  ������� ������ ��� �������� ������, ���������� �� ����������
	void    		(*NextCommand) ();	// 	��������� �������, ������������ ����������
	u8					Error;							//  ������ �������������� � �����������
	u8					CAN_ID;							//	StdId ���������� �� CAN
	u8		   		(*WorkCAN) (u8 command);			//  ��������� �������, �������� �� CAN. ���������� ����� ������
}Device_TypeDef;

extern Device_TypeDef Device;


void InitDevice(Device_TypeDef *dev);
void FindBPS(u8	addr);
void	Ask2b(u8	addr, u8 command, u8 AnswLen, u16 Timeout);
u8	TestCRC1(volatile u8 *bytes, int len);
void Device_WorkDataAfterReading(void);
void  Device_NoAnswerAfterReading(void);
void  Device_NoAnswerAfterWriting(void);
void Device_WorkDataAfterWriting(void);
void	CommandNb(u8	addr, u8 command, u8 AnswLen, u16 Timeout, u8 *data, u8 data_len);
