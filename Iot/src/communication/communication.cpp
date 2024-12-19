#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "communication.h"

void initialize_output_pin() {
  printf("Initializing output pin\n");
  gpio_init(OUTPUT_PIN);
  gpio_set_dir(OUTPUT_PIN, GPIO_OUT);
}

void set_output_pin_true(void * pvParameters) {
  gpio_put(OUTPUT_PIN, true);
}