#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define I2C_BAUDRATE 100000
#define SDA_PIN 12
#define SCL_PIN 13
#define SLAVE_ADDRESS 0x12

void initialize_i2c();

