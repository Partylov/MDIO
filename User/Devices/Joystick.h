#define JoystickFindLen	4	// длина запроса для поиска Джойстика 			 	
#define JoystickBuLen		3
#define Joystick_CAN_ID	24	
void FindJoystick(unsigned char	addr);
void Joystick_Ask0x41(void);
void JoystickPause(void);
unsigned char Joystick_WorkCAN(unsigned char command);
extern unsigned char JoystickInf[JoystickBuLen];
