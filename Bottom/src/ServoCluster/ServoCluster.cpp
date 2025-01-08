#include <Servo.h>
#include "ServoCluster.h"

ServoCluster::ServoCluster(Servo* servos, int numServos) {
    this->servos = servos;
    this->numServos = numServos;
}

ServoCluster::~ServoCluster() {
    delete[] servos;
}
// The function takes an array of angles (Same Size of Servos Cluster Servo and in same order), All servos will reach their angle in same time
void ServoCluster::moveMultipleServosSmoothly(float endAngles[], int totalDurationMs){
    int stepDelay = 20; // Servo updates every 20ms To give the PID time to adjust
    int steps = totalDurationMs / stepDelay; // Calculate the number of steps(iterations)

    // This Part create an array with the increment needed for each servo in every step
    float increments[numServos]; 
    for (int i = 0; i < numServos; i++) {
        float currentAngle = servos[i].getCurrentAngle();
        increments[i] = (endAngles[i] - currentAngle) / steps;
    }

    // Move servos in steps that will reach their angle in totalDurationMs
    for (int step = 0; step < steps; step++) {
        for (int i = 0; i < numServos; i++) {
            float currentAngle = servos[i].getCurrentAngle();
            servos[i].goDegree(currentAngle + increments[i]);
        }
        sleep_ms(stepDelay); // Wait for the next update
    }

    // Ensure all servos reach their exact end angles 
    for (int i = 0; i < numServos; i++) {
        servos[i].goDegree(endAngles[i]);
    }
}


// This function is similar to the above but the difference that you adjust the step delay needed for between each increment 
void ServoCluster::moveMultipleServosSmoothlyWithdelays(float endAngles[], int stepDelay) {
    bool allServosAtTarget = false;

    while (!allServosAtTarget) {
        allServosAtTarget = true;

        for (int i = 0; i < numServos; i++) {
            float currentAngle = servos[i].getCurrentAngle();
            float endAngle = endAngles[i];

            if (currentAngle < endAngle) {
                servos[i].goDegree(currentAngle + 1); // Smaller increment for smoother movement
                allServosAtTarget = false;
            } else if (currentAngle > endAngle) {
                servos[i].goDegree(currentAngle - 1); // Smaller increment for smoother movement
                allServosAtTarget = false;
            }
        }

        sleep_ms(stepDelay);
    }
}
void ServoCluster::writeAll(int* angles) {
    for (int i = 0; i < numServos; i++) {
        servos[i].goDegree(angles[i]);
    }
}

