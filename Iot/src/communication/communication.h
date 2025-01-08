// this library is used instead to send a signal to the Arduino Nano RP2040 (Master) instead of I2C since there are some issues with enabling I2C Interrupt upon a read request from the master.
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"

#define OUTPUT_PIN 18

void initialize_output_pin();