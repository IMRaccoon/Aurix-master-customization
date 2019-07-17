#include "LineFiltering.h"

uint32 filterResult[2][128];
boolean isSchool = FALSE;//TRUE;					// have to change to FALSE, It's just for test
boolean beenObstacle = FALSE;//TRUE;				// have to change to FALSE, It's just for test
boolean isCorrected = FALSE;
boolean isObstacle = FALSE;					// have to change to FALSE, It's just for test
boolean turnleft_init = FALSE;
boolean canWeStart = FALSE;
sint32 edgeRight,edgeLeft;
float32 LineScanAvg_left, LineScanAvg_right;
uint8 edgeNum = 0;
sint8 isleftEmpty = -1;			// -1 is not detected, 0 means go to left lane, 1 means go to right lane

struct Sigma {
   float64 sig;
   uint8 index;
}sigma[127];

struct cumlHist{
	float64 sum;
	uint32 cnt;
}cumlhist[129];

boolean isValidData[2] = {FALSE, FALSE};
boolean isThresholdInit[2] = {FALSE,FALSE};
uint32 hist[128];
uint32 threshold[2];
float32 prev_threshold[2];
float32 leftCam_Correction[49];
float32 my_atanTable[15] = {0.0, 1.263697, 2.522487, 3.771539, 5.006178, 6.221951,7.414689, 8.580558, 9.716103, 10.818272, 11.884434, 12.912389, 13.900357, 14.846971, 15.0};
uint8 emptyLane_left_counter = 0;
uint8 emptyLane_right_counter = 0;

uint32 histogram_otsu(uint32 * data)
{

   uint16 i, k, t;
   float64 var_bg;
   float64 var_fg;
   /* make histogram */
   for (k = 0; k < 128; k++) {
   	   cumlhist[k].cnt = 0;
   	   cumlhist[k].sum = 0;
   	   hist[k] = 0;
   }
   cumlhist[128].cnt = 0;
   cumlhist[128].sum = 0;

   for (k = 20; k < 108; k++) {
	   uint32 temp = (data[k] >> COMPRESSION);
	   cumlhist[temp+1].cnt++;
	   cumlhist[temp+1].sum += temp;
	   hist[temp]++;
   }
   for (k = 0; k < 128; k++) {
   	   cumlhist[k+1].cnt += cumlhist[k].cnt;
   	   cumlhist[k+1].sum += cumlhist[k].sum;
   }
   //calling OTSU
   for (t = 0; t < 128; t++)
   {
	  var_bg = otsu(0, t) ;
	  var_fg = otsu(t + 1, 128 - 1);

	  sigma[t-1].sig = (var_bg * 0.6 + var_fg * 0.4);
	  sigma[t-1].index = t;
   }
   struct Sigma min = { sigma[0].sig, sigma[0].index };
   for (i = 0; i < 128 - 1; i++)
   {
	   if (min.sig > sigma[i].sig) {
		   min.sig = sigma[i].sig;
		   min.index = sigma[i].index;
	   }else if ((min.sig == sigma[i].sig) && (min.index > sigma[i].index)){
		   min.sig = sigma[i].sig;
		   min.index = sigma[i].index;
	   }
   }

   return (min.index << COMPRESSION);
}


float64 otsu(uint16 min, uint16 max)
{
	uint16 j;
	float64 mean = 0.0, wt_bg, sum = 0.0, mean_bg, var = 0.0, var_bg;
	float64 varience, sq, diff;

	mean = cumlhist[max].sum - cumlhist[min].sum;
	sum = cumlhist[max].cnt - cumlhist[min].cnt;
	if (sum != 0.0)
	{
		wt_bg = sum / (SIGMA_SIZE >> COMPRESSION);
		mean_bg = mean / sum;
		for (j = min; j <= max; j++)
		{
		 diff = j - mean_bg;
		 sq = diff * diff;
		 var = var + (sq*(hist[j]));
		}
		var_bg = var / sum;
		varience = wt_bg * var_bg*var_bg;
		return (varience);
	}
	else
		return(0.0);
}
//edgeleft = 0 44 --

/************Filtering Start Function**************************/
void getThreshhold(){
	float32 rate_threshold;
	if(isValidData[0]) {
		threshold[0] = histogram_otsu(medainResult[0]);
		if(!isThresholdInit[0]){
			prev_threshold[0] = threshold[0];
			isThresholdInit[0] = TRUE;
		}else{
			// 30%
			rate_threshold = threshold[0]/prev_threshold[0];
			if( 1.10 > rate_threshold &&  0.90 < rate_threshold) {
				prev_threshold[0] = threshold[0];
			}else {
				threshold[0] = prev_threshold[0];
			}
		}
	}
	if(isValidData[1]) {
		threshold[1] = histogram_otsu(medainResult[1]);
		 if(!isThresholdInit[1]){
			prev_threshold[1] = threshold[1];
			isThresholdInit[1] = TRUE;
		}else{
			rate_threshold = threshold[1]/prev_threshold[1];
			if( 1.10 > rate_threshold &&  0.90 < rate_threshold) {
				prev_threshold[1] = threshold[1];
			}else {
				threshold[1] = prev_threshold[1];
			}
		}
	}
	return;
}
void LineFiltering()
{
	uint8 i, j;
	if(!canWeStart) {
		return;
	}
	float32 sumLeft = (IR_LineScan.sum[SUMLEFT]>>7) / LineScanAvg_left;
	float32 sumRight = (IR_LineScan.sum[SUMRIGHT]>>7) / LineScanAvg_right;
	// 20 %
	if( 1.2 > sumLeft && 0.8 < sumLeft) {
		isValidData[LEFTINDEX] = TRUE;
		if(!isCorrected) {
			lineCorrection_init();
			isCorrected = TRUE;
		}
		lineCorrection_run();
		MedianFilter_run(IR_LineScan.adcResult[LEFTINDEX], TRUE); //isLEFT = TRUE
	}
	else {
		isValidData[LEFTINDEX] = FALSE;
	}
	if(1.2 > sumRight && 0.8 < sumRight) {
		isValidData[RIGHTINDEX] = TRUE;
		MedianFilter_run(IR_LineScan.adcResult[RIGHTINDEX], FALSE); //isLEFT = FALSE
	}
	else {
		isValidData[RIGHTINDEX] = FALSE;
	}
	if(isThresholdInit[RIGHTINDEX] && isThresholdInit[LEFTINDEX] != TRUE)
		return;
	// Draw Depend on threshold
	for(i =0;i<2;i++) {
		for(j =20;j<108;j++){
			if(medainResult[i][j] > threshold[i]) {
				filterResult[i][j] = 4095;
			}
			else {
				filterResult[i][j] = 0;
			}
		}
	}

	// Search Edge
	edgeRight = 0;//11111110000 72
	edgeLeft = 0; //00011111111 42
	for(sint32 i=(expect_l+expect_r)/2 + ERROR_RANGE - 1;i>=20;i--){
		if(filterResult[LEFTINDEX][i]==0 && filterResult[LEFTINDEX][i+1] == 4095){
			edgeLeft = i;
			break;
		}
	}
	for(sint32 i=(expect_l+expect_r)/2 - ERROR_RANGE + 1;i<108;i++){
		if(filterResult[RIGHTINDEX][i]==0 && filterResult[RIGHTINDEX][i-1] == 4095){
			edgeRight = i;
			break;
		}
	}

	// Search Empty Lane when there are two lanes
	if(isSchool && isleftEmpty == -1) {
		if(edgeLeft > expect_l + ERROR_RANGE || edgeLeft < expect_l - ERROR_RANGE){
			if(emptyLane_left_counter++ >= 5) {
				isleftEmpty = TRUE;
				emptyLane_left_counter = 0;
				emptyLane_right_counter = 0;
			}
		}
		if(edgeRight > expect_r + ERROR_RANGE || edgeRight < expect_r - ERROR_RANGE) {
			if(emptyLane_right_counter++ >= 5) {
				if(isleftEmpty == TRUE) {
					isleftEmpty = -1;
				}
				else {
					isleftEmpty = FALSE;
				}
				emptyLane_left_counter = 0;
				emptyLane_right_counter = 0;
			}
		}
	}



	// Linear Regression Start
	LinearRegression_run(&edgeLeft, &edgeRight);

	edgeNum = 0;

	for(uint8 i = 0; i < 35; i++) {
		if(filterResult[LEFTINDEX][edgeLeft + i + 1]== 0 && filterResult[LEFTINDEX][edgeLeft + i] == 4095) { //47
			edgeNum += 1;
		}
		if(filterResult[RIGHTINDEX][edgeRight - i -1] == 0 && filterResult[RIGHTINDEX][edgeRight - i] == 4095) { //77
			edgeNum += 1;
		}
	}

	// Finding School zone
	if(edgeNum >= 4) {
		if(beenObstacle) {
			isSchool = FALSE;
		}
		else {
			isSchool = TRUE;
		}
	}

	controlSrv();
	return;
}

float32 convertAngle(float32 raw_angle){
	if(raw_angle >=0){
		if(raw_angle > 15)
			return RIGHTMAXANGLE;
	}else{
		if(raw_angle < -15)
			return LEFTMAXANGLE;
	}
	return (((RIGHTMAXANGLE - LEFTMAXANGLE) / 30.0f)*(raw_angle-15.0) + RIGHTMAXANGLE);
}

void lineCorrection_init() {
	float32 biggest = 0;
	for(uint8 i = 60; i <= 108; i++) {
		biggest = biggest > IR_LineScan.adcResult[LEFTINDEX][i] ? biggest : IR_LineScan.adcResult[LEFTINDEX][i];
	}
	for(uint8 i = 60; i <= 108; i++) {
		leftCam_Correction[i-60] = (biggest / (float32)IR_LineScan.adcResult[LEFTINDEX][i]);
	}
}


void avoidObstacle(boolean isLeft) {
	beenObstacle = TRUE;
	if(isLeft) {
		IR_setSrvAngle(LEFTMAXANGLE);
	}
	else {
		IR_setSrvAngle(RIGHTMAXANGLE);
	}
	BasicGtmTom_run();
	waitTime(6*TimeConst_100ms + 7*TimeConst_1ms);
	if(isLeft) {
		IR_setSrvAngle(RIGHTMAXANGLE);
	}
	else {
		IR_setSrvAngle(LEFTMAXANGLE);
	}
	BasicGtmTom_run();
	waitTime(7*TimeConst_100ms + 7*TimeConst_1ms);
}

void lineCorrection_run() {
	for(uint8 i = 60; i<= 108; i++) {
		IR_LineScan.adcResult[LEFTINDEX][i] = (IR_LineScan.adcResult[LEFTINDEX][i] * leftCam_Correction[i-60]) > 4095 ? 4095 : (uint32)(IR_LineScan.adcResult[1][i] * leftCam_Correction[i-60]);
	}
}

void controlSrv(){
	sint16 index = (edgeRight+edgeLeft)/2 - CENTER_INDEX;
	float32 angle;
	if(index > 0 ){
		if(index>15.0)
			angle = 15.0;
		else
			angle = my_atanTable[index];
		IR_setSrvAngle(convertAngle(angle));
	}else{
		if(index<-15.0)
			angle = 15.0;
		else
			angle = my_atanTable[-1*index];
		IR_setSrvAngle(convertAngle(-1*angle));
	}
	MotorControl_run();
}

void LineFiltering_init(float32 Left, float32 Right) {
	LineScanAvg_left = Left;
	LineScanAvg_right = Right;
	canWeStart = TRUE;
}
