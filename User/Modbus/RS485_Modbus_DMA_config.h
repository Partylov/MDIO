// Каналы DMA описаны
// в STM32F0x2 Reference manual
// пункт 11.3.7 (стр.203-204)

// Допустимые значения.
#define USE_USART1_DMA
//#define USE_USART2_DMA
//#define USE_USART3_DMA
//#define USE_USART4_DMA


 /**UART1****************************/

#if defined (USE_USART1_DMA)

  /* Definition for USARTx resources */
  #define USARTx                           USART1
  #define USARTx_CLK                       RCC_APB2Periph_USART1
  #define USARTx_CLK_INIT                  RCC_APB2PeriphClockCmd
  #define USARTx_IRQn                      USART1_IRQn
  #define USARTx_IRQHandler                USART1_IRQHandler

  #define USARTx_TX_PIN                    GPIO_Pin_9
  #define USARTx_TX_GPIO_PORT              GPIOA
  #define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOA
  #define USARTx_TX_SOURCE                 GPIO_PinSource9
  #define USARTx_TX_AF                     GPIO_AF_1

  #define USARTx_RX_PIN                    GPIO_Pin_10
  #define USARTx_RX_GPIO_PORT              GPIOA
  #define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOA
  #define USARTx_RX_SOURCE                 GPIO_PinSource10
  #define USARTx_RX_AF                     GPIO_AF_1

  /* Definition for DMAx resources*/
  #define USARTx_TDR_ADDRESS               (uint32_t) (&(USART1->TDR))
  #define USARTx_RDR_ADDRESS               (uint32_t) (&(USART1->RDR))

  #define USARTx_DMA                       DMA1
  #define USARTx_DMAx_CLK                  RCC_AHBPeriph_DMA1

  #define USARTx_TX_DMA_CHANNEL            DMA1_Channel2
  #define USARTx_TX_DMA_FLAG_GLIF          DMA1_FLAG_GL2
  #define USARTx_TX_DMA_FLAG_TEIF          DMA1_FLAG_TE2
  #define USARTx_TX_DMA_FLAG_HTIF          DMA1_FLAG_HT2
  #define USARTx_TX_DMA_FLAG_TCIF          DMA1_FLAG_TC2
	#define USARTx_TX_DMA_IT_TCIF					   DMA1_IT_TC2

  #define USARTx_RX_DMA_CHANNEL            DMA1_Channel3
  #define USARTx_RX_DMA_FLAG_GLIF          DMA1_FLAG_GL3
  #define USARTx_RX_DMA_FLAG_TEIF          DMA1_FLAG_TE3
  #define USARTx_RX_DMA_FLAG_HTIF          DMA1_FLAG_HT3
  #define USARTx_RX_DMA_FLAG_TCIF          DMA1_FLAG_TC3
	#define USARTx_RX_DMA_IT_TCIF					   DMA1_IT_TC3

  #define USARTx_DMA_TX_IRQn               DMA1_Channel2_3_IRQn
  #define USARTx_DMA_RX_IRQn               DMA1_Channel2_3_IRQn
  #define USARTx_DMA_TX_IRQHandler         DMA1_Channel2_3_IRQHandler
  #define USARTx_DMA_RX_IRQHandler         DMA1_Channel2_3_IRQHandler

#endif /* USE_USART1_DMA */


 /**UART2****************************/

#if defined (USE_USART2_DMA)

  /* Definition for USARTx resources */
  #define USARTx                           USART2
  #define USARTx_CLK                       RCC_APB1Periph_USART2
  #define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define USARTx_IRQn                      USART2_IRQn
  #define USARTx_IRQHandler                USART2_IRQHandler

  #define USARTx_TX_PIN                    GPIO_Pin_2
  #define USARTx_TX_GPIO_PORT              GPIOA
  #define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOA
  #define USARTx_TX_SOURCE                 GPIO_PinSource2
  #define USARTx_TX_AF                     GPIO_AF_1

  #define USARTx_RX_PIN                    GPIO_Pin_3
  #define USARTx_RX_GPIO_PORT              GPIOA
  #define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOA
  #define USARTx_RX_SOURCE                 GPIO_PinSource3
  #define USARTx_RX_AF                     GPIO_AF_1

  /* Definition for DMAx resources*/
  #define USARTx_TDR_ADDRESS               (uint32_t) (&(USART2->TDR))
  #define USARTx_RDR_ADDRESS               (uint32_t) (&(USART2->RDR))

  #define USARTx_DMA                       DMA1
  #define USARTx_DMAx_CLK                  RCC_AHBPeriph_DMA1

  #define USARTx_TX_DMA_CHANNEL            DMA_Channel_4
  #define USARTx_TX_DMA_FLAG_GLIF          DMA1_FLAG_GL4
  #define USARTx_TX_DMA_FLAG_TEIF          DMA1_FLAG_TE4
  #define USARTx_TX_DMA_FLAG_HTIF          DMA1_FLAG_HT4
  #define USARTx_TX_DMA_FLAG_TCIF          DMA1_FLAG_TC4
	#define USARTx_TX_DMA_IT_TCIF					   DMA1_IT_TC4

  #define USARTx_RX_DMA_CHANNEL            DMA_Channel_5
  #define USARTx_RX_DMA_FLAG_GLIF          DMA1_FLAG_GL5
  #define USARTx_RX_DMA_FLAG_TEIF          DMA1_FLAG_TE5
  #define USARTx_RX_DMA_FLAG_HTIF          DMA1_FLAG_HT5
  #define USARTx_RX_DMA_FLAG_TCIF          DMA1_FLAG_TC5
	#define USARTx_RX_DMA_IT_TCIF					   DMA1_IT_TC5

  #define USARTx_DMA_TX_IRQn               DMA1_Channel4_5_6_7_IRQn
  #define USARTx_DMA_RX_IRQn               DMA1_Channel4_5_6_7_IRQn
  #define USARTx_DMA_TX_IRQHandler         DMA1_Channel4_5_6_7_IRQHandler
  #define USARTx_DMA_RX_IRQHandler         DMA1_Channel4_5_6_7_IRQHandler

#endif /* USE_USART2_DMA */

 /**UART3****************************/

#if defined (USE_USART3_DMA)

  /* Definition for USARTx resources */
  #define USARTx                           USART3
  #define USARTx_CLK                       RCC_APB1Periph_USART3
  #define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define USARTx_IRQn                      USART3_4_IRQn
  #define USARTx_IRQHandler                USART3_4_IRQHandler

  #define USARTx_TX_PIN                    GPIO_Pin_10
  #define USARTx_TX_GPIO_PORT              GPIOB
  #define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOB
  #define USARTx_TX_SOURCE                 GPIO_PinSource10
  #define USARTx_TX_AF                     GPIO_AF_4

  #define USARTx_RX_PIN                    GPIO_Pin_11
  #define USARTx_RX_GPIO_PORT              GPIOB
  #define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOB
  #define USARTx_RX_SOURCE                 GPIO_PinSource11
  #define USARTx_RX_AF                     GPIO_AF_4

  /* Definition for DMAx resources*/
  #define USARTx_TDR_ADDRESS               (uint32_t) (&(USART3->TDR))
  #define USARTx_RDR_ADDRESS               (uint32_t) (&(USART3->RDR))

  #define USARTx_DMA                       DMA1
  #define USARTx_DMAx_CLK                  RCC_AHB1Periph_DMA1

  #define USARTx_TX_DMA_CHANNEL            DMA1_Channel2
  #define USARTx_TX_DMA_FLAG_GLIF          DMA1_FLAG_GL2
  #define USARTx_TX_DMA_FLAG_TEIF          DMA1_FLAG_TE2
  #define USARTx_TX_DMA_FLAG_HTIF          DMA1_FLAG_HT2
  #define USARTx_TX_DMA_FLAG_TCIF          DMA1_FLAG_TC2
	#define USARTx_TX_DMA_IT_TCIF					   DMA1_IT_TC2

  #define USARTx_RX_DMA_CHANNEL            DMA1_Channel3
  #define USARTx_RX_DMA_FLAG_GLIF          DMA1_FLAG_GL3
  #define USARTx_RX_DMA_FLAG_TEIF          DMA1_FLAG_TE3
  #define USARTx_RX_DMA_FLAG_HTIF          DMA1_FLAG_HT3
  #define USARTx_RX_DMA_FLAG_TCIF          DMA1_FLAG_TC3
	#define USARTx_RX_DMA_IT_TCIF					   DMA1_IT_TC3

  #define USARTx_DMA_TX_IRQn               DMA1_Channel2_3_IRQn
  #define USARTx_DMA_RX_IRQn               DMA1_Channel2_3_IRQn
  #define USARTx_DMA_TX_IRQHandler         DMA1_Channel2_3_IRQHandler
  #define USARTx_DMA_RX_IRQHandler         DMA1_Channel2_3_IRQHandler

#endif /* USE_USART3_DMA */


 /**UART4****************************/

#if defined (USE_USART4_DMA)

  /* Definition for USARTx resources */
  #define USARTx                           USART4
  #define USARTx_CLK                       RCC_APB1Periph_USART4
  #define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
  #define USARTx_IRQn                      USART3_4_IRQn
  #define USARTx_IRQHandler                USART3_4_IRQHandler

//  #define USARTx_TX_PIN                    GPIO_Pin_0
//  #define USARTx_TX_GPIO_PORT              GPIOA
//  #define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOA
//  #define USARTx_TX_SOURCE                 GPIO_PinSource0
  // alternate
  #define USARTx_TX_PIN                    GPIO_Pin_10
  #define USARTx_TX_GPIO_PORT              GPIOC
  #define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOC
  #define USARTx_TX_SOURCE                 GPIO_PinSource10
  #define USARTx_TX_AF                     GPIO_AF_4

//  #define USARTx_RX_PIN                    GPIO_Pin_1
//  #define USARTx_RX_GPIO_PORT              GPIOA
//  #define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOA
//  #define USARTx_RX_SOURCE                 GPIO_PinSource1
  // alternate
  #define USARTx_RX_PIN                    GPIO_Pin_11
  #define USARTx_RX_GPIO_PORT              GPIOC
  #define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOC
  #define USARTx_RX_SOURCE                 GPIO_PinSource11
  #define USARTx_RX_AF                     GPIO_AF_4

  /* Definition for DMAx resources*/
  #define USARTx_TDR_ADDRESS               (uint32_t) (&(USART4->TDR))
  #define USARTx_RDR_ADDRESS               (uint32_t) (&(USART4->RDR))

  #define USARTx_DMA                       DMA1
  #define USARTx_DMAx_CLK                  RCC_AHBPeriph_DMA1

  #define USARTx_TX_DMA_CHANNEL            DMA1_Channel7
  #define USARTx_TX_DMA_FLAG_GLIF          DMA1_FLAG_GL7
  #define USARTx_TX_DMA_FLAG_TEIF          DMA1_FLAG_TE7
  #define USARTx_TX_DMA_FLAG_HTIF          DMA1_FLAG_HT7
  #define USARTx_TX_DMA_FLAG_TCIF          DMA1_FLAG_TC7
	#define USARTx_TX_DMA_IT_TCIF					   DMA1_IT_TC7

  #define USARTx_RX_DMA_CHANNEL            DMA1_Channel6
  #define USARTx_RX_DMA_FLAG_GLIF          DMA1_FLAG_GL6
  #define USARTx_RX_DMA_FLAG_TEIF          DMA1_FLAG_TE6
  #define USARTx_RX_DMA_FLAG_HTIF          DMA1_FLAG_HT6
  #define USARTx_RX_DMA_FLAG_TCIF          DMA1_FLAG_TC6
	#define USARTx_RX_DMA_IT_TCIF					   DMA1_IT_TC6

  #define USARTx_DMA_TX_IRQn               DMA1_Channel4_5_6_7_IRQn
  #define USARTx_DMA_RX_IRQn               DMA1_Channel4_5_6_7_IRQn
  #define USARTx_DMA_TX_IRQHandler         DMA1_Channel4_5_6_7_IRQHandler
  #define USARTx_DMA_RX_IRQHandler         DMA1_Channel4_5_6_7_IRQHandler

#endif /* USE_USART4_DMA */


/*****************************END OF FILE****/


