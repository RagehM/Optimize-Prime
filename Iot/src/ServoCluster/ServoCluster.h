#ifndef SERVOCLUSTER_H
#define SERVOCLUSTER_H

#include <Servo.h>

class ServoCluster {
public:
    ServoCluster(Servo* servos, int numServos);
    void writeAll(int* angles);
    void moveMultipleServosSmoothly(float endAngles[], int totalDurationMs);
    void moveMultipleServosSmoothlyWithdelays(float endAngles[], int stepDelay);
    ~ServoCluster();


private:
    Servo* servos;
    int numServos;
};

#endif 