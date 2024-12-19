#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "communication.h"

void initialize_input_pin() {
  printf("Initializing input pin\n");
  gpio_init(inputPin);
  gpio_set_dir(inputPin, GPIO_IN);
  // gpio_pull_up(inputPin);
}