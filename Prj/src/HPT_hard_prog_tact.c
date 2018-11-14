/**
 * @file   	HPT_hard_prog_tact.c
 * @author 	m.isaev
 * @version
 * @date 	26 сент. 2018 г.
 * @brief
 */


/*#### |Begin| --> Секция - "Include" ########################################*/
#include "HPT_hard_prog_tact.h"
/*#### |End  | <-- Секция - "Include" ########################################*/


/*#### |Begin| --> Секция - "Глобальные переменные" ##########################*/
hpt_hard_programm_tact_status_s HPT_status_s;
/*#### |End  | <-- Секция - "Глобальные переменные" ##########################*/


/*#### |Begin| --> Секция - "Локальные переменные" ###########################*/
/*#### |End  | <-- Секция - "Локальные переменные" ###########################*/


/*#### |Begin| --> Секция - "Прототипы локальных функций" ####################*/
/*#### |End  | <-- Секция - "Прототипы локальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание глобальных функций" ####################*/
void
HPT_InitTIMForProgTact(uint32_t ovefrlowVal)
{
	__HAL_RCC_TIM4_CLK_ENABLE();
	LL_TIM_InitTypeDef timInit_s;
	LL_TIM_StructInit(&timInit_s);

	timInit_s.Autoreload = ovefrlowVal;
	timInit_s.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timInit_s.CounterMode = LL_TIM_COUNTERMODE_UP;
	timInit_s.Prescaler = 72u;

	LL_TIM_Init(
		TIM4,
		&timInit_s);
	LL_TIM_EnableCounter(TIM4);
	LL_TIM_EnableIT_UPDATE(TIM4);

	NVIC_SetPriority(
		TIM4_IRQn,
		1);
	NVIC_EnableIRQ(
		TIM4_IRQn);

}

void TIM4_IRQHandler(void)
{
	LL_TIM_ClearFlag_UPDATE(TIM4);

	HPT_status_s.newProgTactEn_flag ++;
}

size_t HPT_Min(size_t oldMinVal, size_t curMinVal)
{
	if(oldMinVal < curMinVal)
	{
		return oldMinVal;
	}
	else
	{
		return curMinVal;
	}
}
/*#### |End  | <-- Секция - "Описание глобальных функций" ####################*/


/*#### |Begin| --> Секция - "Описание локальных функций" #####################*/
/*#### |End  | <-- Секция - "Описание локальных функций" #####################*/


/*############################################################################*/
/*############################ END OF FILE  ##################################*/
/*############################################################################*/
