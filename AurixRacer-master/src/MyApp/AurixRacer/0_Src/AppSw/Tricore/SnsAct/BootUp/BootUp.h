#ifndef BOOTUP_H
#define BOOTUP_H

#include "../BasicLineScan.h"
#include "../BasicGtmTom.h"
#include "../LineScanTest/LineFiltering.h"

void BootUp_run();

extern float32 LineScanAvg_Left;
extern float32 LineScanAvg_Right;
#endif
