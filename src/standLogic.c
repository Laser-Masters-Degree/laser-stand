#include "periphery_init.h"
#include "standLogic.h"
#include "stm32f4xx.h"
#include <stdio.h>

static void switchOffPeriphery(void) {
  ADC1->CR2 &= ~ADC_CR2_SWSTART;
  DMA2_Stream0->CR &= ~DMA_SxCR_EN;
  DMA2_Stream7->CR &= ~DMA_SxCR_EN;
  GPIOB->BSRR = GPIO_BSRR_BR3;
  // TIM3->CR1 &= ~TIM_CR1_CEN;
}

static void switchOnPeriphery(void) {
  ADC1->CR2 |= ADC_CR2_SWSTART;
  DMA2_Stream0->CR |= DMA_SxCR_EN;
  GPIOB->BSRR = GPIO_BSRR_BS3;
  // DMA2_Stream7->CR |= DMA_SxCR_EN;
  // TIM3->CR1 |= TIM_CR1_CEN;
}

static void parseOutputPackage(volatile outputPackage *outPkg,
                               uint32_t firstSensorData,
                               uint32_t secondSensorData) {
  // outPkg->firstSensorData = firstSensorData;
  // outPkg->secondSensorData = secondSensorData;
  sprintf((char *)(outPkg->firstSensorData), "1:%ld\n", firstSensorData);
  sprintf((char *)(outPkg->secondSensorData), "2:%ld\n", secondSensorData);
  Start_DMA2_USART_Transfer(sizeof(*outPkg));
}

void parseInputPackage(volatile inputPackage *inPack,
                       volatile outputPackage *outPkg, uint32_t firstSensorData,
                       uint32_t secondSensorData) {
  switch (inPack->standStatus) {
  case STAND_OFF:
    switchOffPeriphery();
    break;
  case STAND_ON:
    switchOnPeriphery();
    break;
  case REQUIRE_PACKAGE:
    parseOutputPackage(outPkg, firstSensorData, secondSensorData);
    break;
  default:
    break;
  }
}