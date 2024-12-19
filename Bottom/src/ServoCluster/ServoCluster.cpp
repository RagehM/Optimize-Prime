#include <Servo.h>
#include "ServoCluster.h"

ServoCluster::ServoCluster(Servo* servos, int numServos) {
    this->servos = servos;
    this->numServos = numServos;
}

ServoCluster::~ServoCluster() {
    delete[] servos;
}

void ServoCluster::moveMultipleServosSmoothly(float endAngles[], int totalDurationMs){
    int stepDelay = 20; // Servo updates every 20ms To give the PID time to adjust
    int steps = totalDurationMs / stepDelay;

    float increments[numServos];
    for (int i = 0; i < numServos; i++) {
        float currentAngle = servos[i].getCurrentAngle();
        increments[i] = (endAngles[i] - currentAngle) / steps;
    }

    // Move servos in steps
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

