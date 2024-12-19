#include <stdio.h>
#include "DcMotor.h"
#include "pico/stdlib.h"

static uint8_t A1;
static uint8_t A2;
static uint8_t B1;
static uint8_t B2;

void setupDCMotors(uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2) {
    A1 = a1;
    A2 = a2;
    B1 = b1;
    B2 = b2;
    gpio_init(A1);
    gpio_init(A2);
    gpio_init(B1);
    gpio_init(B2);

    gpio_set_dir(A1, GPIO_OUT);
    gpio_set_dir(A2, GPIO_OUT);
    gpio_set_dir(B1, GPIO_OUT);
    gpio_set_dir(B2, GPIO_OUT);

    gpio_put(A1, 0);
    gpio_put(A2, 0);
    gpio_put(B1, 0);
    gpio_put(B2, 0);
}

void stopMotors() {
    gpio_put(A1, 0);
    gpio_put(A2, 0);
    gpio_put(B1, 0);
    gpio_put(B2, 0);
}

void moveForward() {
    printf("Entered Moving Forward \n");
    stopMotors();
    sleep_ms(250);
    gpio_put(A1, 1);
    gpio_put(A2, 0);
    gpio_put(B1, 1);
    gpio_put(B2, 0);
    sleep_ms(5000);
    stopMotors();
}

void moveBackward() {
    printf("Entered Moving Backward \n");
    stopMotors();
    sleep_ms(250);
    gpio_put(A1, 0);
    gpio_put(A2, 1);
    gpio_put(B1, 0);
    gpio_put(B2, 1);
    sleep_ms(5000);
    stopMotors();
}

void moveLeft() {
    printf("Entered Moving Left \n");
    stopMotors();
    sleep_ms(250);
    gpio_put(A1, 1);
    gpio_put(A2, 0);
    gpio_put(B1, 0);
    gpio_put(B2, 1);
    sleep_ms(5000);
    stopMotors();
}

void moveRight() {
    printf("Entered Moving Right \n");
    stopMotors();
    sleep_ms(250);
    gpio_put(A1, 0);
    gpio_put(A2, 1);
    gpio_put(B1, 1);
    gpio_put(B2, 0);
    sleep_ms(5000);
    stopMotors();
}