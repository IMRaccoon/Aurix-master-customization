#include "BootUp.h"


float32 LineScanAvg_Left = 0;
float32 LineScanAvg_Right = 0;

uint8 count = 0;

void BootUp_run() {
	if(count < 100) {
		LineScanAvg_Left += (IR_LineScan.sum[SUMLEFT] >> 7);
		LineScanAvg_Right += (IR_LineScan.sum[SUMRIGHT] >> 7);
		count += 1;
	}
	else if(count == 100) {
		LineScanAvg_Left /= 100;
		LineScanAvg_Right /= 100;

		LineFiltering_init(LineScanAvg_Left, LineScanAvg_Right);
		count += 1;
	}
}
