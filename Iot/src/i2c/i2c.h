#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string>

// this library is used to enable I2C communication between the Raspberry Pi Pico (Slave) and the Arduino Nano RP2040 (Master) and enable interrupt upon a read request from the master

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