#include "MedianFilter.h"
uint32 medainResult[2][128];

void MedianFilter_run(uint32 * rawData, boolean isLeft) {
	uint8 i, j;
	uint32 median_tmp[MEDIAN_INTERVAL_SQUARE];
	for(i = 20; i < 108; i++) {
		int count = 0;
		for(j = i - MEDIAN_INTERVAL; j <= i + MEDIAN_INTERVAL; j++) {
			median_tmp[count++] = rawData[j];
		}
		Median_QuickSort(median_tmp, 0, MEDIAN_INTERVAL_SQUARE-1);
		if(isLeft)
			medainResult[LEFTINDEX][i] = median_tmp[MEDIAN_INTERVAL];
		else
			medainResult[RIGHTINDEX][i] = median_tmp[MEDIAN_INTERVAL];
	}
}


void Median_QuickSort(uint32 * data, int left, int right) {
	int L = left, R = right;
	int temp;
	int pivot = data[(left + right) / 2];

	while (L <= R) {
		while (data[L] < pivot) {
	      L++;
	    }
	    while (data[R] > pivot) {
	      R--;
	    }

	    if (L <= R) {
	    	if (L != R) {
	    		temp = data[L];
	    		data[L] = data[R];
	    		data[R] = temp;
	    	}
	    	L++; R--;
	    }
	  }
	if (left < R)
		Median_QuickSort(data, left, R);
	if (L < right)
		Median_QuickSort(data, L, right);
}
