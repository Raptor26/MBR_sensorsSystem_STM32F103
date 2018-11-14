/** 
 * @file   	%<%NAME%>%.%<%EXTENSION%>%
 * @author 	%<%USER%>%
 * @version	
 * @date 	%<%DATE%>%, %<%TIME%>%
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "VT_virt_tmrs.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void VT_Init_Tim3MasterTim2Slave(void)
{
	 /* Slave TIM 	ITR0 (TS = 000) 	ITR1 (TS = 001) 	ITR2 (TS = 010) 	ITR3 (TS = 011)
		TIM2 		TIM1 				TIM8 				TIM3 				TIM4
		TIM3 		TIM1 				TIM2 				TIM5 				TIM4
		TIM4 		TIM1 				TIM2 				TIM3 				TIM8
		TIM5 		TIM2 				TIM3 				TIM4 				TIM8
	 */

	/*
	 * TIM2 - должен работать в slavemode
	 * TIM3 - master
	 * */
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();



	/* Настройка таймера */

	LL_TIM_InitTypeDef timInit_s;
	LL_TIM_StructInit(&timInit_s);
	timInit_s.Autoreload = 0xFFFF;
	timInit_s.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timInit_s.CounterMode = LL_TIM_COUNTERMODE_UP;
	timInit_s.Prescaler = 72u;

	/* Инициализируем TIM3 */
	LL_TIM_Init(TIM3, &timInit_s);
	/* Структура для master-мода */
	TIM_MasterConfigTypeDef tim3MasterConfig_s;
	tim3MasterConfig_s.MasterOutputTrigger = TIM_TRGO_UPDATE;
	tim3MasterConfig_s.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(TIM3, &tim3MasterConfig_s);

	/* Структура для slave-мода */
	TIM_SlaveConfigTypeDef tim2SlaveConfig_s;
	tim2SlaveConfig_s.SlaveMode = TIM_SLAVEMODE_TRIGGER;
	/* этот триггер связан с tim3 */
	tim2SlaveConfig_s.InputTrigger = TIM_TS_ITR2;
	/* ETR active on rising edge */
	tim2SlaveConfig_s.TriggerPolarity = TIM_TRIGGERPOLARITY_NONINVERTED;
	/* не использовать предделитель для триггера */
	tim2SlaveConfig_s.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
	/* никаких фильтров */
	tim2SlaveConfig_s.TriggerFilter = 0;


	//Настраиваем slave-а
	HAL_TIM_SlaveConfigSynchronization(TIM2, &tim2SlaveConfig_s);

	/* Включение Master И Slave */
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableCounter(TIM2);
}

void VT_Init_Tim3_As_16bit(
		void)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	LL_TIM_InitTypeDef timInit_s;
	LL_TIM_StructInit(&timInit_s);
	timInit_s.Autoreload = 0xFFFF;
	timInit_s.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timInit_s.CounterMode = LL_TIM_COUNTERMODE_UP;
	timInit_s.Prescaler = 72u;

	/* Инициализируем TIM3 */
	LL_TIM_Init(TIM3, &timInit_s);

	LL_TIM_EnableCounter(TIM3);

}

void TIM3_IRQHandler(void)
{
	/* Сбросить флаги */
	LL_TIM_ClearFlag_UPDATE(TIM3);

	/* Ловушка */
	static int a = 0;
	a++;
}

void TIM2_IRQHandler(void)
{
	/* Сбросить флаги */
	LL_TIM_ClearFlag_UPDATE(TIM2);

	/* Ловушка */
	static int a = 0;
	a++;
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
