#include "../BasicLineScan.h"
#include "../BasicGtmTom.h"
#include "ConfigurationIsr.h"
#include "LinearRegression.h"
#include "MedianFilter.h"
#include "../MotorTest/MotorControl.h"

#define SIGMA_SIZE 		8192
#define COMPRESSION 	5

uint32 histogram_otsu(uint32 * data);
float64 otsu(uint16 min, uint16 max);
void LineFiltering();
void LineFiltering_init(float32 Left, float32 Right);
void getThreshhold();
void controlSrv();

void avoidObstacle(boolean isLeft);

/* There is a problem with the right value of the line scan camera on the left being crushed.
 Solved this problem through correction*/
void lineCorrection_run();
void lineCorrection_init();

#define RIGHTMAXANGLE (-0.15)
#define LEFTMAXANGLE (-0.95)
extern uint32 filterResult[2][128];
extern uint32 medainResult[2][128];
extern sint32 expect_l, expect_r;
extern sint8 isleftEmpty;
extern boolean isSchool;
extern boolean beenObstacle;
extern boolean isObstacle;
extern boolean canWeStart;
