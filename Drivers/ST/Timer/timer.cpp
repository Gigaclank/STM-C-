/**
 * @file timer.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-08
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include "timer.h"

TIMER *timer[16];
/**
 * @brief Construct a new TIMER::TIMER object
 * 
 * @param TIMx 
 * @param ms 
 */
TIMER::TIMER(TIM_TypeDef *TIMx, float ms)
{
  RCC_ClocksTypeDef RCC_Clocks;
  NVIC_InitTypeDef NVIC_InitStructure;
 
  uint32_t timerPeriph;
  uint16_t division = 0;
  uint8_t IrqChannel = 0;
  _TIMx = TIMx;
  RCC_GetClocksFreq(&RCC_Clocks);

  this->cnt = 0;
  this->runTime = 0;

  if (_TIMx == TIM1)
  {
    timerPeriph = RCC_APB2Periph_TIM1;
    IrqChannel = TIM1_UP_TIM10_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (_TIMx == TIM2)
  {
    timerPeriph = RCC_APB1Periph_TIM2;
    IrqChannel = TIM2_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM3)
  {
    timerPeriph = RCC_APB1Periph_TIM3;
    IrqChannel = TIM3_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM4)
  {
    timerPeriph = RCC_APB1Periph_TIM4;
    IrqChannel = TIM4_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM5)
  {
    timerPeriph = RCC_APB1Periph_TIM5;
    IrqChannel = TIM5_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM6)
  {
    timerPeriph = RCC_APB1Periph_TIM6;
    IrqChannel = TIM6_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM7)
  {
    timerPeriph = RCC_APB1Periph_TIM7;
    IrqChannel = TIM7_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM8)
  {
    timerPeriph = RCC_APB2Periph_TIM8;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (_TIMx == TIM9)
  {
    timerPeriph = RCC_APB2Periph_TIM9;
    IrqChannel = TIM1_BRK_TIM9_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (_TIMx == TIM10)
  {
    timerPeriph = RCC_APB2Periph_TIM10;
    IrqChannel = TIM1_UP_TIM10_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (_TIMx == TIM11)
  {
    timerPeriph = RCC_APB2Periph_TIM11;
    IrqChannel = TIM1_TRG_COM_TIM11_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (_TIMx == TIM12)
  {
    timerPeriph = RCC_APB1Periph_TIM12;
    IrqChannel = TIM8_BRK_TIM12_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM13)
  {
    timerPeriph = RCC_APB1Periph_TIM13;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (_TIMx == TIM14)
  {
    timerPeriph = RCC_APB1Periph_TIM14;
    IrqChannel = TIM8_TRG_COM_TIM14_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  

  if (_TIMx == TIM1 || _TIMx == TIM8  || _TIMx == TIM9  || _TIMx == TIM10 || _TIMx == TIM11 || _TIMx == TIM15 || _TIMx == TIM16)
  {
    RCC_APB2PeriphClockCmd(timerPeriph, ENABLE); /* TIM2 Periph clock enable */
  }
  else
  {
    RCC_APB1PeriphClockCmd(timerPeriph, ENABLE);
  }

  float TimeS = ms;// / 1000.0);
  TimeS = 1/TimeS;
  uint32_t u32_PrescalerValue  = (uint32_t)(TimeS*1000);//100000;
  uint32_t u32_TimerValue  = TimeS;
  uint16_t u16_PeriodValue = (uint16_t)(u32_PrescalerValue/u32_TimerValue);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(freq/u32_PrescalerValue)-1; /* Prescale to 1Mhz */
  TIM_TimeBaseStructure.TIM_Period = (uint16_t)(u16_PeriodValue)-1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;

  TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);

  NVIC_InitStructure.NVIC_IRQChannel = IrqChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ClearFlag(_TIMx, TIM_FLAG_Update);      /* Clear TIM1 update pending flag */
  TIM_ITConfig(_TIMx, TIM_IT_Update, ENABLE); /* Enable the TIM1 Update Interrupt */
  TIM_Cmd(_TIMx, ENABLE);                     /* TIM1 counter enable */

  this->attach_intterupt(0);
}


/**
 * @brief Deconstruct TIMER object
 * 
 * @param TIMx 
 * @param ms 
 */
TIMER::~TIMER()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  uint32_t timerPeriph;
  uint8_t IrqChannel = 0;
  

  this->cnt = 0;
  this->runTime = 0;

  if (_TIMx == TIM1)
  {
    timerPeriph = RCC_APB2Periph_TIM1;
    IrqChannel = TIM1_UP_TIM10_IRQn;
    timer_callback[0] = 0;
    timer[0] = 0;
  }
  else if (_TIMx == TIM2)
  {
    timerPeriph = RCC_APB1Periph_TIM2;
    IrqChannel = TIM2_IRQn;
    timer_callback[1] = 0;
    timer[1] = 0;
  }
  else if (_TIMx == TIM3)
  {
    timerPeriph = RCC_APB1Periph_TIM3;
    IrqChannel = TIM3_IRQn;
    timer_callback[2] = 0;
    timer[2] = 0;
  }
  else if (_TIMx == TIM4)
  {
    timerPeriph = RCC_APB1Periph_TIM4;
    IrqChannel = TIM4_IRQn;
    timer_callback[3] = 0;
    timer[3] = 0;
  }
  else if (_TIMx == TIM5)
  {
    timerPeriph = RCC_APB1Periph_TIM5;
    IrqChannel = TIM5_IRQn;
    timer_callback[4] = 0;
    timer[4] = 0;
  }
  else if (_TIMx == TIM6)
  {
    timerPeriph = RCC_APB1Periph_TIM6;
    IrqChannel = TIM6_IRQn;
    timer_callback[5] = 0;
    timer[5] = 0;
  }
  else if (_TIMx == TIM7)
  {
    timerPeriph = RCC_APB1Periph_TIM7;
    IrqChannel = TIM7_IRQn;
    timer_callback[6] = 0;
    timer[6] = 0;
  }
  else if (_TIMx == TIM8)
  {
    timerPeriph = RCC_APB2Periph_TIM8;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    timer_callback[7] = 0;
    timer[7] = 0;
  }
  else if (_TIMx == TIM9)
  {
    timerPeriph = RCC_APB2Periph_TIM9;
    IrqChannel = TIM1_BRK_TIM9_IRQn;
    timer_callback[8] = 0;
    timer[8] = 0;
  }
  else if (_TIMx == TIM10)
  {
    timerPeriph = RCC_APB2Periph_TIM10;
    IrqChannel = TIM1_UP_TIM10_IRQn;
    timer_callback[9] = 0;
    timer[9] = 0;
  }
  else if (_TIMx == TIM11)
  {
    timerPeriph = RCC_APB2Periph_TIM11;
    IrqChannel = TIM1_TRG_COM_TIM11_IRQn;
    timer_callback[10] = 0;
    timer[11] = 0;
  }
  else if (_TIMx == TIM12)
  {
    timerPeriph = RCC_APB1Periph_TIM12;
    IrqChannel = TIM8_BRK_TIM12_IRQn;
    timer_callback[11] = 0;
    timer[11] = 0;
  }
  else if (_TIMx == TIM13)
  {
    timerPeriph = RCC_APB1Periph_TIM13;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    timer_callback[12] = 0;
    timer[12] = 0;
  }
  else if (_TIMx == TIM14)
  {
    timerPeriph = RCC_APB1Periph_TIM14;
    IrqChannel = TIM8_TRG_COM_TIM14_IRQn;
    timer_callback[13] = 0;
    timer[13] = 0;
  }
  

  if (_TIMx == TIM1 || _TIMx == TIM12 || _TIMx == TIM15 || _TIMx == TIM16)
  {
    RCC_APB2PeriphClockCmd(timerPeriph, DISABLE); /* TIM2 Periph clock enable */
  }
  else
  {
    RCC_APB1PeriphClockCmd(timerPeriph, DISABLE);
  }

  NVIC_InitStructure.NVIC_IRQChannel = IrqChannel;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ClearFlag(_TIMx, TIM_FLAG_Update);      /* Clear TIM1 update pending flag */
  TIM_ITConfig(_TIMx, TIM_IT_Update, DISABLE); /* Enable the TIM1 Update Interrupt */
  TIM_Cmd(_TIMx, DISABLE);                     /* TIM1 counter enable */
  
  _TIMx = 0;
}

/**
 * @brief attach a callback handler to the timer
 * 
 * @param callback 
 */
void TIMER::attach_intterupt(void (*callback)(void))
{

  if (_TIMx == TIM1)
  {
    timer_callback[0] = callback;
    timer[0] = this;
  }
  else if (_TIMx == TIM2)
  {
    timer_callback[1] = callback;
    timer[1] = this;
  }
  else if (_TIMx == TIM3)
  {
    timer[2] = this;
    timer_callback[2] = callback;
  }
  else if (_TIMx == TIM4)
  {
    timer[3] = this;
    timer_callback[3] = callback;
  }
  else if (_TIMx == TIM5)
  {
    timer[4] = this;
    timer_callback[4] = callback;
  }
  else if (_TIMx == TIM6)
  {
    timer[5] = this;
    timer_callback[5] = callback;
  }
  else if (_TIMx == TIM7)
  {
    timer[6] = this;
    timer_callback[6] = callback;
  }
  else if (_TIMx == TIM8)
  {
    timer[7] = this;
    timer_callback[7] = callback;
  }
  else if (_TIMx == TIM9)
  {
    timer[8] = this;
    timer_callback[8] = callback;
  }
  else if (_TIMx == TIM10)
  {
    timer[9] = this;
    timer_callback[9] = callback;
  }
  else if (_TIMx == TIM11)
  {
    timer[10] = this;
    timer_callback[10] = callback;
  }
  else if (_TIMx == TIM12)
  {
    timer[11] = this;
    timer_callback[11] = callback;
  }
  else if (_TIMx == TIM13)
  {
    timer[12] = this;
    timer_callback[12] = callback;
  }
  else if (_TIMx == TIM14)
  {
    timer[13] = this;
    timer_callback[13] = callback;
  }
  else if (_TIMx == TIM15)
  {
    timer[14] = this;
    timer_callback[14] = callback;
  }
  else if (_TIMx == TIM16)
  {
    timer[15] = this;
    timer_callback[15] = callback;
  }
}
/**
 * @brief delay based on timer
 * 
 * @param delay_value 
 */
void TIMER::delay(uint16_t delay_value)
{
  this->cnt = delay_value;
  while (this->cnt > 0)
    ;
}
/**
 * @brief Timer called in timer interrupt
 * 
 */
void TIMER::cntr(void)
{
  if (this->cnt > 0)
    this->cnt--;
  this->runTime++;
}