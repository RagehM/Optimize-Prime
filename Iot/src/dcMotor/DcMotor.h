#include <cstdint>

// this library is used to initialize and control the 4 DC motors of the Transformer

// this function given the pins of the DC motors, initializes them
void setupDCMotors(uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2); 

void moveForward(); //
void moveBackward(); // 
void moveLeft(); // 
void moveRight(); //
void stopMotors(); // stop all motors