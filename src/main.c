#include "dataPackages.h"
#include "periphery_init.h"
#include "standLogic.h"
#include "stm32f4xx.h"
#include "utilsConstants.h"

volatile ADCData adcData;
volatile execStatus exStat;
volatile outputPackage outPkg;
volatile inputPackage inPkg;

static uint32_t filtered_sensor_data = 0;

int main() {
  RCC_periphery_activate();
  // TIM3_init();
  DMA2_USART1init((uint32_t)&inPkg, sizeof(inPkg), (uint32_t)&outPkg,
                  sizeof(outPkg));

  DMA2_ADCinit((uint32_t)&adcData, sizeof(adcData));
  USART1_init();
  GPIO_init();
  ADC_init();

  uint32_t diodeValues[CYCLES] = {0};
  uint8_t diodeFilterCounter = 0;
  uint32_t diodeFilterSum = 0;
  while (1) {
    filtered_sensor_data = movingEverageFilter(
        diodeValues, &diodeFilterCounter, &diodeFilterSum, &(adcData.ch1Val));
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
    parseInputPackage(&inPkg, &outPkg, filtered_sensor_data, adcData.ch2Val);
  }
}