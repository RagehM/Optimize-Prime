/*
 * Servo.h
 *
 *  Created on: 1 Jun 2023
 *      Author: jondurrant
 */

#ifndef SERVO_SRC_SERVO_H_
#define SERVO_SRC_SERVO_H_

#define SERVO_R_WRIST 15 //Slice 7A
#define SERVO_R_ELBOW 14 //Slice 6B
#define SERVO_R_SHOULDER 13 //Slice 6A
#define SERVO_R_INNER 12 //Slice 5B
#define SERVO_L_WRIST 11 //Slice 5A
#define SERVO_L_ELBOW 10 //Slice 4B
#define SERVO_L_SHOULDER 9 //Slice  4A
#define SERVO_L_INNER 8 //Slice  3B

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
