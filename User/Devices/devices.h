#define TimeoutWaitAnswer			10	// 10мс
#define TimeoutBeforeRequest	10	// 100мс
#define DeviceErrReading	0xff // байт, которым заполняется буфер при ошибке чтения устройства

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

// Описание структуры устройства, подлкюченного к преобразователю
typedef struct 
{
	DeviceType	Type;								//	Тип устройства
	u8					Address;						//	Адрес устройства	
  void    		(*SendCommand) ();	//	Команды для устройства
	void    		(*WorkAnswer) ();  	// 	Ответы от устройства
	void    		(*NoAnswer) (); 		// 	Устройство не ответило
	u8					CommandNumber;			//  Номер отправляемой/принимаемой команды
	u16					AnswerLength;				//  Длина принимаемого ответа
	u8					RequestCounter; 		//	Счетчик попыток отправки команды
	u8					*Buf;								//  Область памяти для хранения данных, полученных от устройства
	void    		(*NextCommand) ();	// 	Следующая команда, отправляемая устройству
	u8					Error;							//  Ошибка взаимодействия с устройством
	u8					CAN_ID;							//	StdId устройства на CAN
	u8		   		(*WorkCAN) (u8 command);			//  Обработка команды, принятой по CAN. Возвращает длину ответа
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
