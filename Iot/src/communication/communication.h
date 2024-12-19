#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"

#define OUTPUT_PIN 18

void initialize_output_pin();
void set_output_pin_true(void * pvParameters);