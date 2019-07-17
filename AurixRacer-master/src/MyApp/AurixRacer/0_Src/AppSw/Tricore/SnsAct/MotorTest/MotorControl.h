
#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "../BasicGtmTom.h"
#include "ConfigurationIsr.h"
#include "../BasicPort.h"
#include "../LineScanTest/LineFiltering.h"
#include "../BasicGpt12Enc.h"
#include "../ObstacleDetect/ObstacleDetect.h"

#define RADIAN_TO_CYCLE	 	0.159		//1 rad/s == 0.159 cycle/s
#define CYCLE_TO_SPEED		2.5635		// 1 cycle/s == 2.5635 cm/s‬
#define RADIAN_TO_SPEED		RADIAN_TO_CYCLE * CYCLE_TO_SPEED

#define SCHOOLZONE_SPEED	85
#define FIELD_SPEED			100

void MotorControl_init();
void MotorControl_run();
void MotorControl_Field();
void MotorControl_School();
float32 MotorControl_calculate(float32 setpoint, float32 pv);

extern uint8 weight;
extern float32 MotorControl_motorSpeed;
extern float32 MotorControl_encSpeed;
#endif
