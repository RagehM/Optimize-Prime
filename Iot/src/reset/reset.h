#include <stdio.h>
#include "pico/stdlib.h"

// this library is used to enable the transformation in case Wifi or Mic is not working

#define TRANSFORM_EMERGENCY_PIN 28

void initialize_reset_pin();