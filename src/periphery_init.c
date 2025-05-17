#include "periphery_init.h"
#include "stm32f4xx.h"
#include "utilsConstants.h"

void RCC_frequency_init() {
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY)) {
  }
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP);

  // Set PLLM, PLLN and PLLP, and select HSE as PLL source
  RCC->PLLCFGR |= ((8 << RCC_PLLCFGR_PLLM_Pos) | (192 << RCC_PLLCFGR_PLLN_Pos) |
                   (8 << RCC_PLLCFGR_PLLP_Pos) | (1 << RCC_PLLCFGR_PLLSRC_Pos));

  RCC->CFGR |= RCC_CFGR_HPRE_DIV16;
  // Enable PLL and wait for ready
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;
  // Select PLL output as system clock
  RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
  while (!(RCC->CFGR & RCC_CFGR_SWS_PLL))
    ;
}

void RCC_periphery_activate(void) {
  // Periphery activation
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}

void GPIO_init() {
  // Setting GPIO A pins to analog function mode
  GPIOA->MODER |= GPIO_MODER_MODE1;
  GPIOA->MODER |= GPIO_MODER_MODE2;
  // Setting GPIO B pins to output mode
  // Настроить пин PB3 как выход (Output)
  GPIOB->MODER &= ~GPIO_MODER_MODER3; // Сбросить биты MODER для PB5
  GPIOB->MODER |= GPIO_MODER_MODER3_0; // Установить режим Output (01)
  // Настроить тип выхода как Push-Pull
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT3; // Push-Pull (0)
  // Настроить скорость пина (Low speed)
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR3; // Low speed (00)
  // Отключить подтягивающие резисторы (No pull-up/pull-down)
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR3; // No pull (00)

  // Setting GPIO B pins to alternate function mode
  GPIOB->MODER |= GPIO_MODER_MODE7_1;
  GPIOB->MODER |= GPIO_MODER_MODE6_1;
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL7_2 | GPIO_AFRL_AFRL7_1 | GPIO_AFRL_AFRL7_0;
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_0;
}

void DMA2_USART1init(uint32_t varInpAddr, uint32_t varInpSize,
                     uint32_t varOutAddr, uint32_t varOutSize) {
  DMA2_Stream2->CR |= 0x4 << DMA_SxCR_CHSEL_Pos;
  DMA2_Stream2->PAR = (uint32_t) & (USART1->DR);
  DMA2_Stream2->M0AR = varInpAddr;
  DMA2_Stream2->NDTR = varInpSize;
  DMA2_Stream2->CR &= ~DMA_SxCR_DIR;
  DMA2_Stream2->CR |= DMA_SxCR_MINC;
  DMA2_Stream2->CR |= DMA_SxCR_CIRC;
  DMA2_Stream2->CR |= DMA_SxCR_EN;

  DMA2_Stream7->CR = 0x4 << DMA_SxCR_CHSEL_Pos;
  DMA2_Stream7->PAR = (uint32_t) & (USART1->DR);
  DMA2_Stream7->M0AR = varOutAddr;
  DMA2_Stream7->NDTR = varOutSize;
  DMA2_Stream7->CR |= DMA_SxCR_MINC;
  // DMA2_Stream7->CR |= DMA_SxCR_CIRC;
  DMA2_Stream7->CR |= DMA_SxCR_DIR_0;
  // DMA2_Stream7->CR   |= DMA_SxCR_EN;
}

void Start_DMA2_USART_Transfer(uint32_t varOutSize) {

  DMA2_Stream7->CR &= ~DMA_SxCR_EN;
  DMA2->HIFCR |= DMA_HIFCR_CTCIF7 // Очистка флага завершения передачи (TCIF7)
                 | DMA_HIFCR_CHTIF7 // Очистка флага полупередачи (HTIF7)
                 | DMA_HIFCR_CTEIF7; // Очистка флага ошибки (TEIF7)

  DMA2_Stream7->NDTR = varOutSize; // Обновляем счётчик
  DMA2_Stream7->CR |= DMA_SxCR_EN; // Запускаем DMA
}

void DMA2_ADCinit(volatile uint32_t varAddr, uint32_t varSize) {
  DMA2_Stream0->PAR = (uint32_t) & (ADC1->DR);
  DMA2_Stream0->M0AR = varAddr;
  DMA2_Stream0->NDTR = varSize;
  DMA2_Stream0->CR &= ~DMA_SxCR_CHSEL;
  DMA2_Stream0->CR &= ~DMA_SxCR_DIR;
  DMA2_Stream0->CR |= DMA_SxCR_MSIZE_1;
  DMA2_Stream0->CR |= DMA_SxCR_PSIZE_1;
  DMA2_Stream0->CR |= DMA_SxCR_MINC;
  DMA2_Stream0->CR |= DMA_SxCR_CIRC;
  //   DMA2_Stream0->CR |= DMA_SxCR_EN;
}

void USART1_init() {
  USART1->CR1 |= USART_CR1_UE;
  USART1->CR1 |= USART_CR1_TE;
  USART1->CR1 |= USART_CR1_RE;
  USART1->CR1 |= USART_CR1_IDLEIE;
  USART1->CR3 |= USART_CR3_DMAR;
  USART1->CR3 |= USART_CR3_DMAT;
  USART1->BRR = SYSTEM_CORE_CLOCK / BAUD_RATE;
  NVIC_EnableIRQ(USART1_IRQn);
}

// void TIM3_init() {
//   TIM3->PSC = TIM3_PSC;
//   TIM3->ARR = TIM3_ARR;
//   TIM3->CNT = 0;
//   TIM3->CR2 |= TIM_CR2_MMS_1;
//   TIM3->DIER |= TIM_DIER_UIE;
//   NVIC_EnableIRQ(TIM3_IRQn);
//   //   TIM3->CR1 |= TIM_CR1_CEN;
// }

void ADC_init() {
  ADC1->SQR1 |= ADC_SQR1_L_0;
  ADC1->SQR3 |= ADC_SQR3_SQ2_1;
  ADC1->SQR3 |= ADC_SQR3_SQ1_0;
  ADC1->CR2 |= ADC_CR2_CONT;
  // ADC1->CR2 |= ADC_CR2_EXTSEL_3;
  // ADC1->CR2 |= ADC_CR2_EXTEN_0;
  ADC1->CR2 |= ADC_CR2_DDS;
  ADC1->CR2 |= ADC_CR2_DMA;
  ADC1->CR1 |= ADC_CR1_SCAN;
  ADC1->CR2 |= ADC_CR2_ADON;
  //   ADC1->CR2 |= ADC_CR2_SWSTART;
}