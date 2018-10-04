/**
 * @file   	UFD_uart_for_debug.c
 * @author 	m.isaev
 * @version
 * @date 	18 сент. 2018 г.
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "UFD_uart_for_debug.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
UFD_Init_IO_Ports(
	void);

static void
UFD_Init_DMA1_Channel7_For_USART2_Tx(
	void);

static void
UFD_Init_USART2_TxRx(
	uint32_t baudrate);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/

void
UFD_Init_All_USART2_TxRx_DMA1_Channel7_IO_Ports(
	uint32_t baudrate)
{
	/* Инициализация портов ввода/вывода */
	UFD_Init_IO_Ports();

	/* Инициализация канала DMA1_Channel7 для UART2_Tx */
	UFD_Init_DMA1_Channel7_For_USART2_Tx();

	/* Инициализация модуля UART2 */
	UFD_Init_USART2_TxRx(
		baudrate);
}

void
UFD_StartForceDMATransmit(
	uint32_t *pMemSource,
	uint16_t cnt)
{
	/* TODO - UFD написать форсированный запуск передачи по каналу DMA */

	// Включить в UART запрос на передачу DMA
	LL_USART_EnableDMAReq_TX(USART2);

	// Установить адрес, откуда начнется передача
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_7, (uint32_t) pMemSource);

	// Восстановить кол-во байт, которое необходимо передать по каналу DMA
	 LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, cnt);

	// Включить канал DMA
	 LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

	// Включить UART
	 LL_USART_Enable(USART2);

}

void
UFD_StartDMATransmit(
	uint32_t *pMemSource,
	uint16_t cnt)
{
	/* TODO - UFD написать запуск передачи по каналу DMA если канал DMA неактивен */


	if(LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_7) != 1)
	{
	UFD_StartForceDMATransmit(
		*pMemSource,
		cnt);
	}
}

static void
UFD_Init_IO_Ports(
	void)
{
	/* TODO - UFD написать инициализацию портов ввода/вывода */

	LL_GPIO_InitTypeDef GPIO_init_s;
	GPIO_init_s.Mode 		= LL_GPIO_MODE_ALTERNATE;
	GPIO_init_s.OutputType 	= LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_init_s.Pin 		= LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
	GPIO_init_s.Pull 		= LL_GPIO_PULL_UP;
	GPIO_init_s.Speed 		= LL_GPIO_MODE_OUTPUT_50MHz;

	LL_GPIO_Init(
		GPIOA,
		&GPIO_init_s);
}

static void
UFD_Init_DMA1_Channel7_For_USART2_Tx(
	void)
{
	/* TODO - UFD написать инициализацию для DMA1_Channel7 */

	LL_DMA_InitTypeDef DMA_init_s;
	DMA_init_s.Direction 				= LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_init_s.MemoryOrM2MDstAddress 	= 0;
	DMA_init_s.MemoryOrM2MDstDataSize 	= LL_DMA_MDATAALIGN_BYTE;
	DMA_init_s.MemoryOrM2MDstIncMode 	= LL_DMA_MEMORY_INCREMENT;
	DMA_init_s.Mode 					= LL_DMA_MODE_NORMAL;
	DMA_init_s.NbData 					= 0;
	DMA_init_s.PeriphOrM2MSrcAddress 	= (uint32_t) &USART2->DR;
	DMA_init_s.PeriphOrM2MSrcDataSize 	= LL_DMA_PDATAALIGN_BYTE;
	DMA_init_s.PeriphOrM2MSrcIncMode 	= LL_DMA_PERIPH_NOINCREMENT;
	DMA_init_s.Priority 				= LL_DMA_PRIORITY_LOW;

	LL_DMA_Init(
		DMA1,
		LL_DMA_CHANNEL_7,
		&DMA_init_s);

	// ВКЛЮЧЕНИЕ ПРЕРЫВАНИЯ ПО ОКОНЧАНИИ ПЕРЕДАЧИ
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);

	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_7);

	// ПРИОРИТЕТ ПРЕРЫВАНИЯ ДЛЯ DMA
	NVIC_SetPriority(DMA1_Channel7_IRQn, 5);

	// ВЫКЛЮЧЕНИЕ ВЕКТОРА ПРЕРЫВАНИЙ
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

static void
UFD_Init_USART2_TxRx(
	uint32_t baudrate)
{
	/* TODO - UFD написать инициализацию модуля USART2_TxRx */

	LL_USART_InitTypeDef USART_init_s;
	USART_init_s.BaudRate 				= baudrate;
	USART_init_s.DataWidth 				= LL_USART_DATAWIDTH_8B;
	USART_init_s.HardwareFlowControl 	= LL_USART_HWCONTROL_NONE;
	USART_init_s.Parity 				= LL_USART_PARITY_NONE;
	USART_init_s.StopBits 				= LL_USART_STOPBITS_1;
	USART_init_s.TransferDirection 		= LL_USART_DIRECTION_TX_RX;

	LL_USART_Init(
		USART2,
		&USART_init_s);

	LL_USART_Enable(USART2);

	NVIC_SetPriority(USART2_IRQn, 5);
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(
	void)
{
	/* TODO - UFD написать обработчик прерывания модуля USART2 */
	LL_USART_ClearFlag_PE(USART2);
	LL_USART_ClearFlag_FE(USART2);
	LL_USART_ClearFlag_NE(USART2);
	LL_USART_ClearFlag_ORE(USART2);
	LL_USART_ClearFlag_IDLE(USART2);

}

void DMA1_Channel7_IRQHandler(
	void)
{
	/* TODO - UFD написать обработчик прерывания DMA1_Channel7 */

	if (LL_DMA_IsActiveFlag_GI7(DMA1) == 1)
	{
		LL_DMA_ClearFlag_GI7(DMA1);
	}

	if (LL_DMA_IsActiveFlag_TC7(DMA1) == 1)
	{
		LL_DMA_ClearFlag_TC7(DMA1);
		LL_DMA_DisableChannel(
			DMA1,
			LL_DMA_CHANNEL_7);
	}

	if(LL_DMA_IsActiveFlag_TE7(DMA1) == 1)
	{
		LL_DMA_ClearFlag_TE7(DMA1);

		LL_DMA_DisableChannel(
			DMA1,
			LL_DMA_CHANNEL_7);
	}

}

/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
