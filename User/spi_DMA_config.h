// Каналы DMA описаны
// в STM32F207 Reference manual
// пункт 9.3.3 Channel selection (стр.176-177)

// Допустимые значения.
#define USE_SPI1
//#define USE_SPI2
//#define USE_SPI3


 /*****SPI1****************************/
/* SPIx Communication boards Interface */
// SPI1 MISO (PA6)	MOSI(PA7)  CLK(PA5)
#ifdef USE_SPI1
	#define SPIx                           SPI1

	#define SPIx_CLK                       RCC_APB2Periph_SPI1
	#define SPIx_IRQn                      SPI1_IRQn
	#define SPIx_IRQHANDLER                SPI1_IRQHandler

	#define SPIx_SCK_PIN                   GPIO_Pin_5
	#define SPIx_SCK_GPIO_PORT             GPIOA
	#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
	#define SPIx_SCK_SOURCE                GPIO_PinSource5
	#define SPIx_SCK_AF                    GPIO_AF_SPI1

	#define SPIx_MISO_PIN                  GPIO_Pin_6
	#define SPIx_MISO_GPIO_PORT            GPIOA
	#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
	#define SPIx_MISO_SOURCE               GPIO_PinSource6
	#define SPIx_MISO_AF                   GPIO_AF_SPI1

	#define SPIx_MOSI_PIN                  GPIO_Pin_7
	#define SPIx_MOSI_GPIO_PORT            GPIOA
	#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
	#define SPIx_MOSI_SOURCE               GPIO_PinSource7
	#define SPIx_MOSI_AF                   GPIO_AF_SPI1

  /* Definition for DMAx resources*/
  #define SPIx_DR_ADDRESS                   (uint32_t) (&(SPI1->DR))

  #define SPIx_DMA                          DMA2
  #define SPIx_DMAx_CLK                     RCC_AHB1Periph_DMA2

  #define SPIx_DMA_CHANNEL                  DMA_Channel_3
  #define SPIx_DMA_STREAM_TX                DMA2_Stream3
  #define SPIx_DMA_STREAM_RX                DMA2_Stream0

  #define SPIx_TX_DMA_TCFLAG                DMA_FLAG_TCIF3
  #define SPIx_TX_DMA_FEIFLAG               DMA_FLAG_FEIF3
  #define SPIx_TX_DMA_DMEIFLAG              DMA_FLAG_DMEIF3
  #define SPIx_TX_DMA_TEIFLAG               DMA_FLAG_TEIF3
  #define SPIx_TX_DMA_HTIFLAG               DMA_FLAG_HTIF3
  #define SPIx_TX_DMA_IT_TCIF				DMA_IT_TCIF3
  #define SPIx_RX_DMA_TCFLAG                DMA_FLAG_TCIF0
  #define SPIx_RX_DMA_FEIFLAG               DMA_FLAG_FEIF0
  #define SPIx_RX_DMA_DMEIFLAG              DMA_FLAG_DMEIF0
  #define SPIx_RX_DMA_TEIFLAG               DMA_FLAG_TEIF0
  #define SPIx_RX_DMA_HTIFLAG               DMA_FLAG_HTIF0
  #define SPIx_RX_DMA_IT_TCIF				DMA_IT_TCIF0

  #define SPIx_DMA_TX_IRQn                  DMA2_Stream3_IRQn
  #define SPIx_DMA_RX_IRQn                  DMA2_Stream0_IRQn
  #define SPIx_DMA_TX_IRQHandler            DMA2_Stream3_IRQHandler
  #define SPIx_DMA_RX_IRQHandler            DMA2_Stream0_IRQHandler
#endif


 /*****SPI2****************************/
// SPI2 MISO (PC2)	MOSI(PC3)  CLK(PB10)
#ifdef USE_SPI2
	#define SPIx                           SPI2

	#define SPIx_CLK                       RCC_APB1Periph_SPI2
	#define SPIx_IRQn                      SPI2_IRQn
	#define SPIx_IRQHANDLER                SPI2_IRQHandler

	#define SPIx_SCK_PIN                   GPIO_Pin_10
	#define SPIx_SCK_GPIO_PORT             GPIOB
	#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
	#define SPIx_SCK_SOURCE                GPIO_PinSource10
    // alternate
//	#define SPIx_SCK_PIN                   GPIO_Pin_13
//	#define SPIx_SCK_GPIO_PORT             GPIOB
//	#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
//	#define SPIx_SCK_SOURCE                GPIO_PinSource13
	#define SPIx_SCK_AF                    GPIO_AF_SPI2

	#define SPIx_MISO_PIN                  GPIO_Pin_2
	#define SPIx_MISO_GPIO_PORT            GPIOC
	#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
	#define SPIx_MISO_SOURCE               GPIO_PinSource2
    // alternate
//	#define SPIx_MISO_PIN                  GPIO_Pin_14
//	#define SPIx_MISO_GPIO_PORT            GPIOB
//	#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
//	#define SPIx_MISO_SOURCE               GPIO_PinSource14
	#define SPIx_MISO_AF                   GPIO_AF_SPI2

	#define SPIx_MOSI_PIN                  GPIO_Pin_3
	#define SPIx_MOSI_GPIO_PORT            GPIOC
	#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
	#define SPIx_MOSI_SOURCE               GPIO_PinSource3
    // alternate
//	#define SPIx_MOSI_PIN                  GPIO_Pin_15
//	#define SPIx_MOSI_GPIO_PORT            GPIOB
//	#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
//	#define SPIx_MOSI_SOURCE               GPIO_PinSource15
	#define SPIx_MOSI_AF                   GPIO_AF_SPI2

  /* Definition for DMAx resources*/
  #define SPIx_DR_ADDRESS                   (uint32_t) (&(SPI2->DR))

  #define SPIx_DMA                          DMA1
  #define SPIx_DMAx_CLK                     RCC_AHB1Periph_DMA1

  #define SPIx_DMA_CHANNEL                  DMA_Channel_0
  #define SPIx_DMA_STREAM_TX                DMA1_Stream4
  #define SPIx_DMA_STREAM_RX                DMA1_Stream3

  #define SPIx_TX_DMA_TCFLAG                DMA_FLAG_TCIF4
  #define SPIx_TX_DMA_FEIFLAG               DMA_FLAG_FEIF4
  #define SPIx_TX_DMA_DMEIFLAG              DMA_FLAG_DMEIF4
  #define SPIx_TX_DMA_TEIFLAG               DMA_FLAG_TEIF4
  #define SPIx_TX_DMA_HTIFLAG               DMA_FLAG_HTIF4
  #define SPIx_TX_DMA_IT_TCIF				DMA_IT_TCIF4
  #define SPIx_RX_DMA_TCFLAG                DMA_FLAG_TCIF3
  #define SPIx_RX_DMA_FEIFLAG               DMA_FLAG_FEIF3
  #define SPIx_RX_DMA_DMEIFLAG              DMA_FLAG_DMEIF3
  #define SPIx_RX_DMA_TEIFLAG               DMA_FLAG_TEIF3
  #define SPIx_RX_DMA_HTIFLAG               DMA_FLAG_HTIF3
  #define SPIx_RX_DMA_IT_TCIF				DMA_IT_TCIF3

  #define SPIx_DMA_TX_IRQn                  DMA1_Stream4_IRQn
  #define SPIx_DMA_RX_IRQn                  DMA1_Stream3_IRQn
  #define SPIx_DMA_TX_IRQHandler            DMA1_Stream4_IRQHandler
  #define SPIx_DMA_RX_IRQHandler            DMA1_Stream3_IRQHandler
#endif


 /*****SPI3****************************/
#ifdef USE_SPI3
	#define SPIx                           SPI3

	#define SPIx_CLK                       RCC_APB1Periph_SPI3
	#define SPIx_IRQn                      SPI3_IRQn
	#define SPIx_IRQHANDLER                SPI3_IRQHandler

	#define SPIx_SCK_PIN                   GPIO_Pin_10
	#define SPIx_SCK_GPIO_PORT             GPIOC
	#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOC
	#define SPIx_SCK_SOURCE                GPIO_PinSource10
	#define SPIx_SCK_AF                    GPIO_AF_SPI3

	#define SPIx_MISO_PIN                  GPIO_Pin_11
	#define SPIx_MISO_GPIO_PORT            GPIOC
	#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
	#define SPIx_MISO_SOURCE               GPIO_PinSource11
	#define SPIx_MISO_AF                   GPIO_AF_SPI3

	#define SPIx_MOSI_PIN                  GPIO_Pin_12
	#define SPIx_MOSI_GPIO_PORT            GPIOC
	#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
	#define SPIx_MOSI_SOURCE               GPIO_PinSource12
	#define SPIx_MOSI_AF                   GPIO_AF_SPI3

  /* Definition for DMAx resources*/
  #define SPIx_DR_ADDRESS                   (uint32_t) (&(SPI3->DR))

  #define SPIx_DMA                          DMA1
  #define SPIx_DMAx_CLK                     RCC_AHB1Periph_DMA1

  #define SPIx_DMA_CHANNEL                  DMA_Channel_0
  #define SPIx_DMA_STREAM_TX                DMA1_Stream5
  #define SPIx_DMA_STREAM_RX                DMA1_Stream0

  #define SPIx_TX_DMA_TCFLAG                DMA_FLAG_TCIF5
  #define SPIx_TX_DMA_FEIFLAG               DMA_FLAG_FEIF5
  #define SPIx_TX_DMA_DMEIFLAG              DMA_FLAG_DMEIF5
  #define SPIx_TX_DMA_TEIFLAG               DMA_FLAG_TEIF5
  #define SPIx_TX_DMA_HTIFLAG               DMA_FLAG_HTIF5
  #define SPIx_TX_DMA_IT_TCIF				DMA_IT_TCIF5
  #define SPIx_RX_DMA_TCFLAG                DMA_FLAG_TCIF0
  #define SPIx_RX_DMA_FEIFLAG               DMA_FLAG_FEIF0
  #define SPIx_RX_DMA_DMEIFLAG              DMA_FLAG_DMEIF0
  #define SPIx_RX_DMA_TEIFLAG               DMA_FLAG_TEIF0
  #define SPIx_RX_DMA_HTIFLAG               DMA_FLAG_HTIF0
  #define SPIx_RX_DMA_IT_TCIF				DMA_IT_TCIF0

  #define SPIx_DMA_TX_IRQn                  DMA1_Stream5_IRQn
  #define SPIx_DMA_RX_IRQn                  DMA1_Stream0_IRQn
  #define SPIx_DMA_TX_IRQHandler            DMA1_Stream5_IRQHandler
  #define SPIx_DMA_RX_IRQHandler            DMA1_Stream0_IRQHandler
#endif