#ifndef STAND_LOGIC
#define STAND_LOGIC

#include "dataPackages.h"
#define STAND_OFF 0
#define STAND_ON 1
#define REQUIRE_PACKAGE 2
#define START_BIT 255
#define END_BIT 0

#define CYCLES 25

uint32_t movingEverageFilter(uint32_t *data, uint8_t *counter, uint32_t *sum,
                             uint32_t *value);

void parseInputPackage(volatile inputPackage *inPack,
                       volatile outputPackage *outPkg, uint32_t firstSensorData,
                       uint32_t secondSensorData);
#endif