#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// this library is used to enable I2C communication between the Raspberry Pi Pico (Slave) and the Arduino Nano RP2040 (Master)

#define I2C_PORT i2c0
#define I2C_BAUDRATE 100000
#define SDA_PIN 12
#define SCL_PIN 13
#define SLAVE_ADDRESS 0x12

void initialize_i2c();

