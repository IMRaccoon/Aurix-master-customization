#include "MotorControl.h"

float32 Motor_max, Motor_min, Motor_Kp;
float32 MotorControl_motorSpeed;
float32 MotorControl_encSpeed;
float32 MotorControl_incresement;
uint8 delay = 0;


void MotorControl_init() {
	IR_setMotor0En(TRUE);
	Motor_max = 1;
	Motor_min = -1;
	Motor_Kp = 0.05;
	MotorControl_motorSpeed = 0;
	MotorControl_encSpeed = 0;
	MotorControl_incresement = 0;
}

void MotorControl_run() {
	// give motor delay to find proper edge & threshold
	if(delay > 150) {
		//MotorControl_encSpeed = IR_getEncSpeed() * RADIAN_TO_SPEED;
		if(isSchool) {
			ObstacleDetect_run();
			//MotorControl_School();
		}
		else {
			//MotorControl_Field();
		}
		IR_setMotor0Vol(0.2);
	}
	else {
		delay += 1;
	}

	//float32 controlAngle = IR_getSrvAngle();
	/*
	if(prev_Angle == controlAngle) {
	 */

	//MotorControl_speed = IR_getMotor0Vol();
	/*
	if(isSchool) {
		IR_setMotor0Vol(SCHOOLZONE_SPEED);
	}
	 */
}

void MotorControl_School() {
	MotorControl_motorSpeed = SCHOOLZONE_SPEED;
	if(isObstacle) {
		if(isleftEmpty == 0) {
			avoidObstacle(TRUE);
			isleftEmpty = 1;
			isObstacle = FALSE;
		}
		else {			// problem is that if 'isleftEmpty' variable is -1 (which means not detected), it will go to right lane
			avoidObstacle(FALSE);
			isleftEmpty = 0;
			isObstacle = FALSE;
		}
	}
	if(MotorControl_motorSpeed != MotorControl_encSpeed) {
		MotorControl_incresement = MotorControl_calculate(MotorControl_motorSpeed, MotorControl_encSpeed);
		IR_setMotor0Vol(IR_getMotor0Vol() + MotorControl_incresement);
	}
}

void MotorControl_Field() {
	MotorControl_motorSpeed = FIELD_SPEED;
	if(MotorControl_motorSpeed != MotorControl_encSpeed) {
		MotorControl_incresement = MotorControl_calculate(MotorControl_motorSpeed, MotorControl_encSpeed);
		IR_setMotor0Vol(IR_getMotor0Vol() + MotorControl_incresement);
	}
}

/*********** Proportional-Integral-Differential controller Run *************/
float32 MotorControl_calculate(float32 setpoint, float32 pv) {
	// Calculate error
	float32 error = setpoint - pv;

	// Proportional term
	float32 Pout = Motor_Kp * error;

	if(Pout > Motor_max) {
		Pout = Motor_max;
	}
	else if(Pout < Motor_min) {
		Pout = Motor_min;
	}
	return Pout;
}
