
#ifndef OBSTACLEDETECT_H
#define OBSTACLEDETECT_H

#include "../LineScanTest/LineFiltering.h"
#include "../BasicVadcBgScan.h"

#define DETECT_RANGE 		80
#define DANGER_RANGE		60

void ObstacleDetect_run();

extern float32 infrared;
extern float32 ultrasound;

#endif
