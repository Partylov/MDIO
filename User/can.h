void CAN_Config(void); // ������������� � ������ CAN1.
void CanWriteData(uint16_t ID, uint8_t len);
extern uint8_t FrameBuffer[16]; // ��������� ����� ��� ���������� ������� ���������.
//extern bool bReceivedFirstHalfFrame; // ������� ����� ��������� ������� ���������.
//extern bool bReceivedSecondHalfFrame; // ������� ����� ��������� ������� ���������.
extern uint8_t MyCANaddress; // ����������� CAN-����� �������� �����������.
extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;
