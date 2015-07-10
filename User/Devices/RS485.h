#define SetRS485_Receive    GPIO_ResetBits(GPIOA, GPIO_Pin_8); // ����� RS485 
#define SetRS485_Transmit   GPIO_SetBits(GPIOA, GPIO_Pin_8); // �������� RS485 


extern unsigned char RxBufModbus[256];     //����� �������� GSM ������ �� �����.
extern unsigned char TxBufModbus[256];     //����� �������� GSM ������ �� ��������.
void SetTimeoutRS485(unsigned int timeout);
void StopRS485timer(void);
