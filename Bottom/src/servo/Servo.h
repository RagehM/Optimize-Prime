/*
 * Servo.h
 *
 *  Created on: 1 Jun 2023
 *      Author: jondurrant
 */

#ifndef SERVO_SRC_SERVO_H_
#define SERVO_SRC_SERVO_H_

#define SERVO_R_FEET 21 //Slice 2B
#define SERVO_R_KNEE 19 //Slice 2A
#define SERVO_R_THIGH 20 //Slice 1B
#define SERVO_R_STAND 18 //Slice 1A
#define SERVO_L_FEET 28 //Slice 0B
#define SERVO_L_KNEE 29 //Slice 0A
#define SERVO_L_THIGH 15 //Slice 7B
#define SERVO_L_STAND 25 //Slice 4b

#include "pico/stdlib.h"

class Servo {
public:
	Servo(uint8_t gp, float start, bool secondChannel=false, int ROTATE_0 = 480, int ROTATE_180 = 2430);
	virtual ~Servo();

	/**
	 * move to angle: 0 to 180.
	 * @param degree
	 */
	void goDegree(float degree);
	   /**
     * Move to angle slowly.
     * @param end
     * @param stepDelay
     */
    void moveSlowly(float end, int stepDelay);
	float getCurrentAngle() const;

private:
	uint8_t xGP = 0;
	float currentAngle = 0.0;
	int ROTATE_0 = 480;
	int ROTATE_180 = 2430;
};

#endif /* SERVO_SRC_SERVO_H_ */
