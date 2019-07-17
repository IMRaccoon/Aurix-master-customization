#include "LineFiltering.h"

float32 num[LINEAR_DATA] = {-1.5, -0.5, 0.5, 1.5};
float32 right[LINEAR_DATA] = {80, 80, 80, 80},		// 72
		left[LINEAR_DATA] = {43, 43, 43, 43};		// 42
sint32 expect_l, expect_r;

void LinearRegression_run(sint32 * l, sint32 * r) {
	float32 d_l[LINEAR_DATA], d_r[LINEAR_DATA];
	float32 num_power_sig = 5.5;
	float32 mean_l, mean_r;
	float32 a_l, a_r, b_l, b_r;
	uint8 i;

	mean_l = LinearRegression_mean(left);
	mean_r = LinearRegression_mean(right);

	for(i = 0; i < LINEAR_DATA; i++) {
		d_l[i] = left[i] - mean_l;
		d_r[i] = right[i] - mean_r;
	}

	a_l = LinearRegression_sigma(d_l) / num_power_sig;
	a_r = LinearRegression_sigma(d_r) / num_power_sig;

	b_l = mean_l - a_l * num_power_sig;
	b_r = mean_r - a_r * num_power_sig;

	expect_l =	a_l * 5 + b_l;
	expect_r =	a_r * 5 + b_r;

	if(!isSchool && (*l == 0 || expect_l - ERROR_RANGE > *l || *l > expect_l + ERROR_RANGE)) {
		*l = expect_l;
	}
	else if(isSchool && (*l == 0 || expect_l - 20 > *l || *l > expect_l + 20)) {
		*l = expect_l;
	}
	else {
		LinearRegression_pop(left, *l);
	}
	if(!isSchool && (*r == 0 || expect_r - ERROR_RANGE > *r || *r > expect_r + ERROR_RANGE)) {
		*r = expect_r;
	}
	else if(isSchool && (*r == 0 || expect_r - 20 > *r || *r > expect_r + 20)) {
		*r = expect_r;
	}
	else {
		LinearRegression_pop(right, *r);
	}
}

float32 LinearRegression_mean(float32 a[]) {
	float32 sum = 0;
	for(uint8 i = 0; i < LINEAR_DATA; i++) {
		sum += a[i];
	}
	return (sum / LINEAR_DATA);
}

float32 LinearRegression_sigma(float32 a[]) {
	float32 sum = 0;
	for(uint8 i = 0 ; i < LINEAR_DATA; i++) {
		sum = sum + a[i] + num[i];
	}
	return sum;
}

void LinearRegression_pop(float32 a[], float32 pop) {
	for(uint8 i = 0; i < LINEAR_DATA; i++) {
		if(i == LINEAR_DATA - 1){
			a[i] = pop;
		}
		else {
			a[i] = a[i+1];
		}
	}
}


void LinearRegression_setData(sint16 data, boolean isLeft) {
	if(isLeft) {
		left[0] = data;
		left[1] = data;
		left[2] = data;
		left[3] = data;
	}
	else {
		right[0] = data;
		right[1] = data;
		right[2] = data;
		right[3] = data;
	}
}
