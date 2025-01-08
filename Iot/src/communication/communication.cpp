// this library is used instead to send a signal to the Arduino Nano RP2040 (Master) instead of I2C since there are some issues with enabling I2C Interrupt upon a read request from the master.
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "communication.h"

void initialize_output_pin() {
  printf("Initializing output pin\n");
  gpio_init(OUTPUT_PIN); // initialize output pin
  gpio_set_dir(OUTPUT_PIN, GPIO_OUT); // // setting the output pin as output
}