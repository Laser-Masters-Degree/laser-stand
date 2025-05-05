#ifndef DATA_PACKAGES
#define DATA_PACKAGES
#include <stdint.h>

typedef struct {
  uint32_t ch1Val;
  uint32_t ch2Val;
} ADCData;

typedef struct {
  int8_t startBit;
  int8_t standStatus;
  int8_t endBit;
} inputPackage;

typedef struct {
  // uint32_t firstSensorData;
  // uint32_t secondSensorData;
  int8_t firstSensorData[9];
  int8_t secondSensorData[9];
} outputPackage;

typedef struct {
  int8_t standState;
  int8_t ADCStatus;
  int8_t USARTStatus;
  int8_t DMAStatus;
  int8_t laserStatus;
} execStatus;
#endif