#include "ObstacleDetect.h"


float32 infrared;
float32 ultrasound;


void ObstacleDetect_run() {
	infrared = IR_getUsrAdcChn0();

	if(infrared < DETECT_RANGE) {
		if(infrared < DANGER_RANGE) {
			if(isSchool) {
				isObstacle = TRUE;
			}
		}
	}
}
