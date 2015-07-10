#define SetRS485_Receive    GPIO_ResetBits(GPIOA, GPIO_Pin_8); // Прием RS485 
#define SetRS485_Transmit   GPIO_SetBits(GPIOA, GPIO_Pin_8); // Передача RS485 


extern unsigned char RxBufModbus[256];     //буфер драйвера GSM модема на прием.
extern unsigned char TxBufModbus[256];     //буфер драйвера GSM модема на передачу.
void SetTimeoutRS485(unsigned int timeout);
void StopRS485timer(void);
