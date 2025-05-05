#include "dataPackages.h"
#include "periphery_init.h"
#include "standLogic.h"
#include "stm32f4xx.h"
#include "utilsConstants.h"

volatile ADCData adcData;
volatile execStatus exStat;
volatile outputPackage outPkg;
volatile inputPackage inPkg;

int main() {
  RCC_periphery_activate();
  // TIM3_init();
  DMA2_USART1init((uint32_t)&inPkg, sizeof(inPkg), (uint32_t)&outPkg,
                  sizeof(outPkg));

  DMA2_ADCinit((uint32_t)&adcData, sizeof(adcData));
  USART1_init();
  GPIO_init();
  ADC_init();
  while (1) {
  }
  return 0;
}

// void TIM3_IRQHandler() {
//   TIM3->SR &= ~TIM_SR_UIF;
//   parseOutputPackage(&outPkg, adcData.ch1Val, adcData.ch2Val);
// }

void USART1_IRQHandler() {
  if (USART1->SR & USART_SR_IDLE) {
    (void)USART1->DR;
    parseInputPackage(&inPkg, &outPkg, adcData.ch1Val, adcData.ch2Val);
  }
}