#include <camera.h>
#include "pico/stdlib.h"

void setupCamera() {
  gpio_init(CAMERA_PIN); // initializing the camera pin
  gpio_set_dir(CAMERA_PIN, GPIO_IN); // setting the camera pin as input
}

// function to read the camera pin value
int readCamera() {
  return gpio_get(CAMERA_PIN);
}