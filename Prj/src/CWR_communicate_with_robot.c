/**
 * @file   	CWR_communicate_with_robot.c
 * @author 	m.isaev
 * @version
 * @date 	16 окт. 2018 г.
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "CWR_communicate_with_robot.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
CWR_Init_TxRx_IOPorts(
	void);

static void
CWR_Init_USART1_TxRx(
	uint32_t baudrate);

static void
CWR_Init_DMA1_Channel4_For_USART1_Tx(
	void);

static void
CWR_Init_DMA1_Channel5_For_USART1_Rx(
	void);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
CWR_Init_USART1_TxWithDMA_RxWithDMA(
	uint32_t baudrate)
{
	/* Инициализация портов ввода/вывода */
	CWR_Init_TxRx_IOPorts();

	/* Инициализация модулей USART */
	CWR_Init_USART1_TxRx(
		baudrate);

	/* Инициализация канала DMA на передачу */
	CWR_Init_DMA1_Channel4_For_USART1_Tx();

	/* Инициализация канала DMA на прием данных */
	CWR_Init_DMA1_Channel5_For_USART1_Rx();
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
void
CWR_Init_TxRx_IOPorts(
	void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	LL_GPIO_InitTypeDef GPIO_init_s;
	GPIO_init_s.Mode 		= LL_GPIO_MODE_ALTERNATE;
	GPIO_init_s.OutputType 	= LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_init_s.Pin 		= LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
	GPIO_init_s.Pull 		= LL_GPIO_PULL_UP;
	GPIO_init_s.Speed 		= LL_GPIO_MODE_OUTPUT_50MHz;

	LL_GPIO_Init(
		GPIOA,
		&GPIO_init_s);
}

void
CWR_Init_USART1_TxRx(
	uint32_t baudrate)
{
	__HAL_RCC_USART1_CLK_ENABLE();

	LL_USART_InitTypeDef USART_init_s;
	USART_init_s.BaudRate 				= (uint32_t) baudrate;
	USART_init_s.DataWidth 				= LL_USART_DATAWIDTH_8B;
	USART_init_s.HardwareFlowControl 	= LL_USART_HWCONTROL_NONE;
	USART_init_s.Parity 				= LL_USART_PARITY_NONE;
	USART_init_s.StopBits 				= LL_USART_STOPBITS_1;
	USART_init_s.TransferDirection 		= LL_USART_DIRECTION_TX_RX;

	LL_USART_Init(
		USART1,
		&USART_init_s);

	LL_USART_Enable(USART1);

	/* Конфигурирование источников прерываний */
	LL_USART_EnableIT_RXNE(USART1);

	/* Конфигурирование вектора прерываний */
	NVIC_SetPriority(USART1_IRQn, 5);
	NVIC_EnableIRQ(USART1_IRQn);
}

void
CWR_Init_DMA1_Channel4_For_USART1_Tx(
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
	DMA_init_s.PeriphOrM2MSrcAddress 	= (uint32_t) &USART1->DR;
	DMA_init_s.PeriphOrM2MSrcDataSize 	= LL_DMA_PDATAALIGN_BYTE;
	DMA_init_s.PeriphOrM2MSrcIncMode 	= LL_DMA_PERIPH_NOINCREMENT;
	DMA_init_s.Priority 				= LL_DMA_PRIORITY_LOW;

	LL_DMA_Init(
		DMA1,
		LL_DMA_CHANNEL_4,
		&DMA_init_s);

	/* Конфигурирование источников прерываний */
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);

	/* Конфигурирование вектора прерываний */
	NVIC_SetPriority	(DMA1_Channel4_IRQn, 5);
	NVIC_EnableIRQ		(DMA1_Channel4_IRQn);
}

void
CWR_Init_DMA1_Channel5_For_USART1_Rx(
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
	DMA_init_s.PeriphOrM2MSrcAddress 	= (uint32_t) &USART1->DR;
	DMA_init_s.PeriphOrM2MSrcDataSize 	= LL_DMA_PDATAALIGN_BYTE;
	DMA_init_s.PeriphOrM2MSrcIncMode 	= LL_DMA_PERIPH_NOINCREMENT;
	DMA_init_s.Priority 				= LL_DMA_PRIORITY_LOW;

	LL_DMA_Init(
		DMA1,
		LL_DMA_CHANNEL_5,
		&DMA_init_s);

	/* Конфигурирование источников прерываний */
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_5);

	/* Конфигурирование вектора прерываний */
	NVIC_SetPriority	(DMA1_Channel5_IRQn, 5);
	NVIC_EnableIRQ		(DMA1_Channel5_IRQn);
}
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
