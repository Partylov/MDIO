/**
  ******************************************************************************
  * @file    can.c
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#include "stdbool.h"
#include "main.h"
#include "RS485_modbus.h"
#include "devices.h"
//#include "RS485_modbus2.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/*  �����������------------------------------------------*/
// #define NUM_MODULES_CAN 30 // ���������� ������� �� ���� CAN.
// #define MAX_ERROR_CAN	5 // ������������ ���������� ������ ����� � �������.

uint8_t MyCANaddress; // ����������� CAN-����� �������� �����������.
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ��������� ����������------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
//CanTxMsg TxMessage;

/* Private variables ---------------------------------------------------------*/
	
/* Private functions ---------------------------------------------------------*/
void CAN_Config(void);


/*******************************************************************************
* Function Name  : CanWriteData
* Description    : Can Write Date to CAN-BUS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
CanTxMsg TxMessage;
void CanWriteData(uint16_t ID, uint8_t len)
{
  //CanTxMsg TxMessage;
  /* transmit */
  TxMessage.StdId = ID; // ����������� ID
//TxMessage.ExtId = 0x00; // ���������� ID
  TxMessage.RTR = CAN_RTR_DATA; // ��� ������ (������� ������ ��� ������ ���������)
  TxMessage.IDE = CAN_ID_STD; // ��� ID (����������� ��� ����������)
  TxMessage.DLC = len; // ���������� ���� � ���� ������ (1..8)
//	for (i=0; i<8; i++)	{ TxMessage.Data[i] = CAN1_DATA[i]; } // �������� ������.
  CAN_Transmit(CAN,&TxMessage);
}


/*******************************************************************************
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
*******************************************************************************/
CanRxMsg RxMessage;
void CEC_CAN_IRQHandler(void)
{
	//CanRxMsg RxMessage;
  CAN_Receive(CAN,CAN_FIFO0, &RxMessage);
  CAN_ClearITPendingBit(CAN,CAN_IT_FMP0);
	// ��������� ���������� ���������.
	if (Device.CAN_ID == ((RxMessage.StdId&0x79F) - 0x600))
		CanWriteData(0x580+(RxMessage.StdId&0x60)+Device.CAN_ID, Device.WorkCAN((uint8_t)((RxMessage.StdId&0x60)>>5))); // ��������� �������
}


/*******************************************************************************
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
*******************************************************************************/
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /** CAN GPIO Configuration  
  PA11   ------> CAN_RX
  PA12   ------> CAN_TX
  */

  /*Enable or disable the AHB peripheral clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /*Configure GPIO pin : PA */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*Configure GPIO pin alternate function */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_4);

  /*Configure GPIO pin alternate function */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_4);

  /* CAN configuration ********************************************************/
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);

  /* CAN register init */
  CAN_DeInit(CAN);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE; // ������������������ ����� �� ����� CAN
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

/* ������������ CAN Baudrate:
	   SystemClock=48���
		 ������� APB1 = SystemClock/APB1_Prescaler = 48/1 = 48���
		 �������_������������_CAN = APB1/CAN_Prescaler = 48/24 = 2���
		 ������������ ���� 1+BS1+BS2 = 1+7+8 = 16 �������
		 CAN Baudrate = 2���/16 = 125���.  */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 24;
  CAN_Init(CAN, &CAN_InitStructure);

	/* CAN filter init "FIFO0" */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
//  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x6420;
//  CAN_FilterInitStructure.CAN_FilterIdLow = 0x2461;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

//  /* Transmit Structure preparation */
//  TxMessage.StdId = 0x321; // ����������� ID
//  TxMessage.ExtId = 0x01; // ���������� ID
//  TxMessage.RTR = CAN_RTR_DATA; // ��� ������ (������� ������ ��� ������ ���������)
//  TxMessage.IDE = CAN_ID_STD; // ��� ID (����������� ��� ����������)
//  TxMessage.DLC = 8; // ���������� ���� � ���� ������ (1..8)

  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
  /* Enable FIFO 0 full Interrupt */
//  CAN_ITConfig(CAN, CAN_IT_FF0, ENABLE);
  /* Enable FIFO 1 full Interrupt */
//  CAN_ITConfig(CAN, CAN_IT_FF1, ENABLE);
	
	/* Configures the NVIC for CAN.  */
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/************************END OF FILE****/
