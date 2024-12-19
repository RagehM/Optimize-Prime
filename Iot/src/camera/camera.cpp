#include <camera.h>
#include "pico/stdlib.h"

void setupCamera() {
  gpio_init(CAMERA_PIN);
  gpio_set_dir(CAMERA_PIN, GPIO_IN);
  //gpio_put(CAMERA_PIN, 1);
}

int readCamera() {
  return gpio_get(CAMERA_PIN);
}