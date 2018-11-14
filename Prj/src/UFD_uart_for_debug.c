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

/**
 * @brief	Функция выполняет инициализацию аппаратных модулей для передачи
 * 			данных по USART2 с помощью DMA
 * @param[in]	baudrate:	Скорость передачи данных
 * @return	None
 */
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

/**
 * @brief	Функция передает данные по модулю USART2 с помощью канала DMA
 * @param[in]	*pMemSource:	Адрес, откуда начнется передача
 * @param[in]	cnt: 	Количество байт, которые необходимо передать
 * @return	None
 */
void
UFD_StartForceDMATransmit(
	uint32_t *pMemSource,
	uint16_t cnt)
{
	/* Отключение модуля UART2 */
	LL_USART_Disable(USART2);

	/* Отключение канала DMA */
	LL_DMA_DisableChannel(
		DMA1,
		LL_DMA_CHANNEL_7);

	/* Установить адрес, откуда начнется передача */
	LL_DMA_SetMemoryAddress(
		DMA1,
		LL_DMA_CHANNEL_7,
		(uint32_t) pMemSource);

	/* Восстановить кол-во байт, которое необходимо передать по каналу DMA */
	LL_DMA_SetDataLength(
		DMA1,
		LL_DMA_CHANNEL_7,
		cnt);

	/* Включить в UART запрос на передачу DMA */
	LL_USART_EnableDMAReq_TX(USART2);

	/* Включить канал DMA */
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);

	/* Включить UART */
	LL_USART_Enable(USART2);
}

/**
 * @brief	Функция принудительно передает данные по модулю USART2 с помощью канала DMA,
 * 			если канал DMA неактивен
 * @param[in]	*pMemSource: Адрес, откуда начнется передача
 * @param[in]	cnt:	Количество байт, которые необходимо передать
 * @return	None
 */
void
UFD_StartDMATransmit(
	uint32_t *pMemSource,
	uint16_t cnt)
{
	if (LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_7) != 1)
	{
		UFD_StartForceDMATransmit(
			pMemSource,
			cnt);
	}
}

static void
UFD_Init_IO_Ports(
	void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

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
	__HAL_RCC_DMA1_CLK_ENABLE();

	LL_DMA_InitTypeDef DMA_init_s;
	DMA_init_s.Direction 				= LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_init_s.MemoryOrM2MDstAddress 	= 0u;
	DMA_init_s.MemoryOrM2MDstDataSize 	= LL_DMA_MDATAALIGN_BYTE;
	DMA_init_s.MemoryOrM2MDstIncMode 	= LL_DMA_MEMORY_INCREMENT;
	DMA_init_s.Mode 					= LL_DMA_MODE_NORMAL;
	DMA_init_s.NbData 					= 0u;
	DMA_init_s.PeriphOrM2MSrcAddress 	= (uint32_t) &USART2->DR;
	DMA_init_s.PeriphOrM2MSrcDataSize 	= LL_DMA_PDATAALIGN_BYTE;
	DMA_init_s.PeriphOrM2MSrcIncMode 	= LL_DMA_PERIPH_NOINCREMENT;
	DMA_init_s.Priority 				= LL_DMA_PRIORITY_LOW;

	LL_DMA_Init(
		DMA1,
		LL_DMA_CHANNEL_7,
		&DMA_init_s);

	/* Конфигурирование источников прерываний */
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_7);

	/* Конфигурирование вектора прерываний */
	NVIC_SetPriority	(DMA1_Channel7_IRQn, 5);
	NVIC_EnableIRQ		(DMA1_Channel7_IRQn);
}

static void
UFD_Init_USART2_TxRx(
	uint32_t baudrate)
{
	__HAL_RCC_USART2_CLK_ENABLE();

	LL_USART_InitTypeDef USART_init_s;
	USART_init_s.BaudRate 				= (uint32_t) baudrate;
	USART_init_s.DataWidth 				= LL_USART_DATAWIDTH_8B;
	USART_init_s.HardwareFlowControl 	= LL_USART_HWCONTROL_NONE;
	USART_init_s.Parity 				= LL_USART_PARITY_NONE;
	USART_init_s.StopBits 				= LL_USART_STOPBITS_1;
	USART_init_s.TransferDirection 		= LL_USART_DIRECTION_TX_RX;

	LL_USART_Init(
		USART2,
		&USART_init_s);

	LL_USART_Enable(USART2);

	/* Конфигурирование источников прерываний */
	LL_USART_EnableIT_RXNE(USART2);

	/* Конфигурирование вектора прерываний */
	NVIC_SetPriority(USART2_IRQn, 5);
	NVIC_EnableIRQ(USART2_IRQn);
}

/**
 * @brief	Функция обработки прерывания USART2
 * @param	None
 * @return	None
 */
void USART2_IRQHandler(
	void)
{
	/* Если пришел байт данных */
	if (LL_USART_IsActiveFlag_RXNE(USART2) == 1)
	{
		uint8_t trash =
			LL_USART_ReceiveData8(
				USART2);
		(void) trash;
	}

	/* Иначе */
	else
	{
		/* Сброс всех флагов */
		LL_USART_ClearFlag_PE	(USART2);
		LL_USART_ClearFlag_FE	(USART2);
		LL_USART_ClearFlag_NE	(USART2);
		LL_USART_ClearFlag_ORE	(USART2);
		LL_USART_ClearFlag_IDLE	(USART2);
		LL_USART_ClearFlag_TC	(USART2);
		LL_USART_ClearFlag_LBD	(USART2);
		LL_USART_ClearFlag_nCTS	(USART2);
	}
}

/**
 * @brief	Функция обработки прерывания DMA
 * @param	None
 * @return	None
 */
void DMA1_Channel7_IRQHandler(
	void)
{
	if (LL_DMA_IsActiveFlag_TC7(DMA1) == 1)
	{
		/* Очистка флагов */
		LL_DMA_ClearFlag_TC7(DMA1);
		LL_DMA_ClearFlag_GI7(DMA1);

		/* Отключение канала DMA */
		LL_DMA_DisableChannel(
			DMA1,
			LL_DMA_CHANNEL_7);
	}
	else
	{
		/* Очистка флагов */
		LL_DMA_ClearFlag_HT7	(DMA1);
		LL_DMA_ClearFlag_TE7	(DMA1);
		LL_DMA_ClearFlag_TC7	(DMA1);
		LL_DMA_ClearFlag_GI7	(DMA1);

		/* Отключение канала DMA */
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
