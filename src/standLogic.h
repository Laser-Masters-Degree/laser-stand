#ifndef STAND_LOGIC
#define STAND_LOGIC

#include "dataPackages.h"
#define STAND_OFF 0
#define STAND_ON 1
#define REQUIRE_PACKAGE 2
#define START_BIT 255
#define END_BIT 0

// void parseOutputPackage(volatile outputPackage *outPkg,
//                         uint32_t firstSensorData, uint32_t secondSensorData);

void parseInputPackage(volatile inputPackage *inPack,
                       volatile outputPackage *outPkg, uint32_t firstSensorData,
                       uint32_t secondSensorData);
#endif