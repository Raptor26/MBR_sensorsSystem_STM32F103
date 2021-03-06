/**
 * @file   	BLEDS_board_leds.c
 * @author 	m.isaev
 * @version
 * @date 	24 сент. 2018 г.
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "BLEDS_board_leds.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
static void
BLEDS_Init_GreenLed(
	void);
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
BLEDS_Init_AllLeds(
	void)
{
	/* Инициализация порта для управления зеленым светодиодом */
	BLEDS_Init_GreenLed();

	/* Включение зеленого светодиода */
	BLEDS_Green_ON();
}

void
BLEDS_Init_GreenLed(
	void)
{
	/* Включение тактирования PORTC */
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	/* Объявление структур для инициализации порта ввода/вывода */
	LL_GPIO_InitTypeDef gpio_init_s;
	LL_GPIO_StructInit(
		&gpio_init_s);

	/* Заполнение полей структуры инициализации порта ввода/вывода */
	gpio_init_s.Mode		= LL_GPIO_MODE_OUTPUT;
	gpio_init_s.OutputType	= LL_GPIO_OUTPUT_PUSHPULL;
	gpio_init_s.Pin 		= LL_GPIO_PIN_13;
	gpio_init_s.Pull		= LL_GPIO_PULL_UP;
	gpio_init_s.Speed		= LL_GPIO_SPEED_FREQ_LOW;

	/* Инициализация порта для светодиода */
	LL_GPIO_Init(
		GPIOC,
		&gpio_init_s);
}

void
BLEDS_Green_ON(
	void)
{
	/* TODO - Включение светодиода */
}

void
BLEDS_Green_OFF(
	void)
{
	/* TODO - Выключение светодиода */
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
