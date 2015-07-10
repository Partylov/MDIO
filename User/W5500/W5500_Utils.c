#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "W5500_Utils.h"
#include "W5500_SPI.h"

#define WIZ_INT_PIN					GPIO_Pin_4	// in
#define WIZ_INT_PORT				GPIOC				// in

#define WIZ_RESET_PIN		    GPIO_Pin_5
#define WIZ_RESET_PORT	    GPIOC

#define WIZ_CS_PIN					GPIO_Pin_4
#define WIZ_CS_PORT			  	GPIOA	
#define HIGH	1
#define LOW		0
#define ON	1
#define OFF	0


// Configuration Network Information of W5200
#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket  */
// SRAM address range is 0x2000 0000 ~ 0x2000 4FFF (20KB)
#define TX_RX_MAX_BUF_SIZE	2048

uint8_t Enable_DHCP = OFF;
uint8_t MAC[6] = {0x00, 0x08, 0xDC, 0x01, 0x02, 0x03};//MAC Address
uint8_t IP[4] = {192, 168, 1, 2};//IP Address
uint8_t GateWay[4] = {192, 168, 1, 10};//Gateway Address
uint8_t SubNet[4] = {255, 255, 255, 0};//SubnetMask Address

//TX MEM SIZE- SOCKET 0:8KB, SOCKET 1:2KB, SOCKET2-7:1KB
//RX MEM SIZE- SOCKET 0:8KB, SOCKET 1:2KB, SOCKET2-7:1KB
uint8_t txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8_t rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};

//FOR TCP Client
//Configuration Network Information of TEST PC
uint8_t Dest_IP[4] = {192, 168, 1, 3}; //DST_IP Address
uint16_t Dest_PORT = 3000; //DST_IP port

uint8_t ch_status[MAX_SOCK_NUM] = { 0, };	/** 0:close, 1:ready, 2:connected */

uint8_t TX_BUF[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
uint8_t RX_BUF[TX_RX_MAX_BUF_SIZE]; // RX Buffer for applications


#define COMMON_BASE 0x0000
/**
 @brief Gateway IP Register address
 */
#define GAR0			  (COMMON_BASE + 0x0001)
/**
 @brief Subnet mask Register address
 */
#define SUBR0			  (COMMON_BASE + 0x0005)
/**
 @brief Source MAC Register address
 */
#define SHAR0				(COMMON_BASE + 0x0009)
/**
 @brief Source IP Register address
 */
#define SIPR0				(COMMON_BASE + 0x000F)



uint8_t W5500_Read(uint16_t reg)
{
	uint8_t data;
	WIZ_CS(LOW);

  SPIx_SendByte((reg & 0xFF00) >> 8);   // Address byte Hi
	SPIx_SendByte(reg & 0x00FF);          // Address byte Lo
	SPIx_SendByte(0x00);                   // Control Phase = Read Nbytes CommonRegs
	data = SPIx_SendByte(0x00);            // Data read (read 1byte data)
	
	WIZ_CS(HIGH);
	return data;
}

void W5500_Write(uint16_t reg, uint8_t data)
{
	WIZ_CS(LOW);

  SPIx_SendByte((reg & 0xFF00) >> 8);   // Address byte Hi
	SPIx_SendByte(reg & 0x00FF);          // Address byte Lo
	SPIx_SendByte(0x04);                  // Control Phase = Write Nbytes CommonRegs
	SPIx_SendByte(data);            			// Data write (write 1byte data)
	
	WIZ_CS(HIGH);
}

// Установка параметров сети Ethernet.
void Set_network(void)
{
	// MAC ADDRESS
//	for (i = 0 ; i < 6; i++) Config_Msg.Mac[i] = MAC[i];
	W5500_Write(SHAR0+0,MAC[0]);
	W5500_Write(SHAR0+1,MAC[1]);
	W5500_Write(SHAR0+2,MAC[2]);
	W5500_Write(SHAR0+3,MAC[3]);
	W5500_Write(SHAR0+4,MAC[4]);
	W5500_Write(SHAR0+5,MAC[5]);

	// Local IP ADDRESS
//	Config_Msg.Lip[0] = IP[0]; Config_Msg.Lip[1] = IP[1]; Config_Msg.Lip[2] = IP[2]; Config_Msg.Lip[3] = IP[3];
	W5500_Write(SIPR0+0,IP[0]);
	W5500_Write(SIPR0+1,IP[1]);
	W5500_Write(SIPR0+2,IP[2]);
	W5500_Write(SIPR0+3,IP[3]);
	
	// GateWay ADDRESS
//	Config_Msg.Gw[0] = GateWay[0]; Config_Msg.Gw[1] = GateWay[1]; Config_Msg.Gw[2] = GateWay[2]; Config_Msg.Gw[3] = GateWay[3];
	W5500_Write(GAR0+0,GateWay[0]);
	W5500_Write(GAR0+1,GateWay[1]);
	W5500_Write(GAR0+2,GateWay[2]);
	W5500_Write(GAR0+3,GateWay[3]);

	// Subnet Mask ADDRESS
//	Config_Msg.Sub[0] = SubNet[0]; Config_Msg.Sub[1] = SubNet[1]; Config_Msg.Sub[2] = SubNet[2]; Config_Msg.Sub[3] = SubNet[3];
	W5500_Write(SUBR0+0,SubNet[0]);
	W5500_Write(SUBR0+1,SubNet[1]);
	W5500_Write(SUBR0+2,SubNet[2]);
	W5500_Write(SUBR0+3,SubNet[3]);

//	W5500_Write(0,0x10);
	W5500_Write(0,0x0);
}

// Управление ногой CS 
void WIZ_CS(uint8_t val)
{
	if (val == LOW) {
   		GPIO_ResetBits(WIZ_CS_PORT, WIZ_CS_PIN); 
	}else if (val == HIGH){
   		GPIO_SetBits(WIZ_CS_PORT, WIZ_CS_PIN); 
	}
}

void Reset_W5200(void)
{
//	GPIO_ResetBits(WIZ_RESET_PORT, WIZ_RESET_PIN);
//	Delay_us(2);  
//	GPIO_SetBits(WIZ_RESET_PORT, WIZ_RESET_PIN);
//	Delay_ms(150);  
   uint32_t nCount;
	 GPIO_SetBits(WIZ_RESET_PORT, WIZ_RESET_PIN);
//	Delay_ms(1500);
   nCount = 10000;
   while(nCount--){}
	 GPIO_ResetBits(WIZ_RESET_PORT, WIZ_RESET_PIN);
// Delay_us(2);
   nCount = 1000;
   while(nCount--){}
	 GPIO_SetBits(WIZ_RESET_PORT, WIZ_RESET_PIN);
// Delay_ms(1500);
   nCount = 1000000;
   while(nCount--){}
}

void W5500_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 /*Enable or disable the AHB peripheral clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);

  /*Configure GPIO pin : PC IN*/
  GPIO_InitStructure.GPIO_Pin = WIZ_INT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(WIZ_INT_PORT, &GPIO_InitStructure);
	
  /*Configure GPIO pin : PC OUT*/
  GPIO_InitStructure.GPIO_Pin = WIZ_RESET_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(WIZ_RESET_PORT, &GPIO_InitStructure);
	
  /*Configure GPIO pin : PA OUT*/
  GPIO_InitStructure.GPIO_Pin = WIZ_CS_PIN;
  GPIO_Init(WIZ_CS_PORT, &GPIO_InitStructure);
}
