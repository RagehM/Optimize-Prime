#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "reset.h"

void initialize_reset_pin() {
  printf("Initializing reset pin\n");
  gpio_init(TRANSFORM_EMERGENCY_PIN);
  gpio_set_dir(TRANSFORM_EMERGENCY_PIN, GPIO_IN);
  gpio_pull_up(TRANSFORM_EMERGENCY_PIN);
}
