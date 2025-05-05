#ifndef PERIPHERY_INIT
#define PERIPHERY_INIT
#include <stdint.h>

void RCC_periphery_activate(void);
void RCC_frequency_init(void);
void ADC_init(void);
void USART1_init(void);
void DMA2_ADCinit(uint32_t, uint32_t);
void DMA2_USART1init(uint32_t varInpAddr, uint32_t varInpSize,
                     uint32_t varOutAddr, uint32_t varOutSize);
void Start_DMA2_USART_Transfer(uint32_t varOutSize);
// void TIM3_init(void);
void GPIO_init(void);
#endif