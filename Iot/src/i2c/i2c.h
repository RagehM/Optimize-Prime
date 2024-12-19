#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string>

#define I2C_PORT i2c0
#define I2C_BAUDRATE 100000
#define SDA_PIN 16
#define SCL_PIN 17
#define SLAVE_ADDRESS 0x12

extern bool Transform;
extern bool MicTransform;
extern bool transformBack;

void initialize_i2c();
void i2c_irq_handler();