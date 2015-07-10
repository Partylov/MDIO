void CAN_Config(void); // Инициализация и Запуск CAN1.
void CanWriteData(uint16_t ID, uint8_t len);
extern uint8_t FrameBuffer[16]; // Временный буфер для сохранения первого полукадра.
//extern bool bReceivedFirstHalfFrame; // Признак ранее принятого первого полукадра.
//extern bool bReceivedSecondHalfFrame; // Признак ранее принятого второго полукадра.
extern uint8_t MyCANaddress; // Собственный CAN-адрес этажного контроллера.
extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;
