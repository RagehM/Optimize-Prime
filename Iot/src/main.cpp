#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "lwip/netif.h"
#include "hardware/adc.h"
#include "lwipopts.h"
#include "camera.h"
#include "Servo.h"
#include "DcMotor.h"
#include <stdio.h>
#include <string>
#include <ServoCluster.h>
#include <i2c.h>
#include "communication.h"
#include "reset.h"

#define DELAY 3000

const char WIFI_SSID[] = "xxxxxxx";
const char WIFI_PASSWORD[] = "xxxxxx";

const char * ssi_tags[] = {"camera", "micFlag", "state"};

ServoCluster* globalServosInners = nullptr;
ServoCluster* globalServosElbows = nullptr; 

bool sendTransform = false;
bool forwardFlag = false;
bool backwardFlag = false;
bool rightFlag = false;
bool leftFlag = false;
bool waveFlag = false;
int state = 0;
int micFlag = 0;
int cameraValue = 0;
char operation[10] = "";

const char* Left(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "left") != 0) {
        strcpy(operation, "left");
        leftFlag = true;
    }
    return "/index.shtml";
}

const char* Right(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "right") != 0) {
        strcpy(operation, "right");
        rightFlag = true;
    }
    return "/index.shtml";
}

const char* Backward(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "backward") != 0) {
        strcpy(operation, "backward");
        backwardFlag = true;
    }
    return "/index.shtml";
}

const char* Forward(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "forward") != 0) {
        strcpy(operation, "forward");
        forwardFlag = true;
    }
    return "/index.shtml";
}

const char* transformSignal(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "transform") != 0) {
        strcpy(operation, "transform");
        sendTransform = true;
    }
    return "/index.shtml";
}

const char* toggleMic(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "mic") != 0) {
        strcpy(operation, "mic");
        if(micFlag == 0) {
            micFlag = 1;
        } else {
            micFlag = 0;
        }
        printf("MicFlag %i \n", micFlag);
    }
    return "/index.shtml";
}

const char* wave(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "wave") != 0) {
        strcpy(operation, "wave");
        waveFlag = true;
    }
    return "/index.shtml";
}

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
    size_t printed;
    switch (iIndex) {
    case 0: // camera
    {
        cameraValue = readCamera();
        printed = snprintf(pcInsert, iInsertLen, "%i", cameraValue);
        //printf("SSI: %s\n", pcInsert);
        break;
    }
    case 1: // micFlag
    {
        printed = snprintf(pcInsert, iInsertLen, "%i", micFlag);
        //printf("Mic Flag: %s\n", pcInsert);
        break;
    }
    case 2: // state
    {
        printed = snprintf(pcInsert, iInsertLen, "%i", state);
        //printf("State: %s\n", pcInsert);
        break;
    }
    default: 
        printed = 0;
    }
    return (u16_t)printed;
}

const tCGI cgi_handlers[] = {
    {
    "/transform", transformSignal,
    },
    {
    "/mic", toggleMic,
    },
    {
    "/wave", wave
    },
    {
    "/forward", Forward
    },
    {
    "/backward", Backward
    },
    {
    "/right", Right
    },
    {
    "/left", Left
    },
};



int main() {
    stdio_init_all();
    sleep_ms(8000);
    initialize_i2c();
    initialize_output_pin();
    initialize_reset_pin();
    setupDCMotors(2, 3, 4, 5);
    setupCamera();

    //xTaskCreate(set_output_pin_true, "SET_OUTPUT_PIN", 256, NULL, 1, NULL);
    
    printf("Starting Wifi Connection\n");
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();

    sleep_ms(5000);

    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
    }
    printf("Connected! \n");
    struct netif *netif = netif_list;
    if (netif_is_up(netif)) {
        printf("IP Address: %s\n", ipaddr_ntoa(&netif->ip_addr));
    } else {
        printf("Failed to retrieve IP address\n");
    }
    sleep_ms(5000);
    httpd_init();
    printf("Http server initialised\n");
    http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
    printf("SSI Handler initialised\n");
    http_set_cgi_handlers(cgi_handlers, sizeof(cgi_handlers) / sizeof(tCGI));
    printf("CGI Handler initialised\n");

    sleep_ms(3000);

    printf("Initialising Servos\n");

    Servo servoRWrist(SERVO_R_WRIST, 50);
    Servo servoRElbow(SERVO_R_ELBOW, 90);

    Servo servoRShoulder(SERVO_R_SHOULDER, 90); //false
    Servo servoRInner(SERVO_R_INNER, 90); //false

    Servo servoLWrist(SERVO_L_WRIST, 50, true);
    Servo servoLElbow(SERVO_L_ELBOW, 90);

    Servo servoLShoulder(SERVO_L_SHOULDER, 90); //false
    Servo servoLInner(SERVO_L_INNER, 90); //false

    Servo servoInners[] = {servoRInner, servoLInner};
    ServoCluster servosInners(servoInners, 2);

    Servo servoElbows[] = {servoRElbow, servoLElbow};
    ServoCluster servosElbows(servoElbows, 2);

    globalServosInners = &servosInners;
    globalServosElbows = &servosElbows;
    printf("Servos Initialised\n");

    sleep_ms(1000);
    while(true) {
        if(Transform) {
            printf("Transforming! \n");
            float endAngles3[] = {0,180};
            globalServosInners->moveMultipleServosSmoothly(endAngles3, 2000);
            Transform = false;
            state = 1;
            printf("Transformed! \n");
        }
        else if(transformBack) {
            printf("Transforming Back! \n");
            float endAngles3[] = {90,90};
            globalServosInners->moveMultipleServosSmoothly(endAngles3, 2000);
            transformBack = false;
            state = 0;
            printf("Transformed Back! \n");
        }
        else if(sendTransform | (MicTransform & micFlag) | gpio_get(TRANSFORM_EMERGENCY_PIN) == 0) {
            printf("Sending Transforming Signal\n");
            gpio_put(OUTPUT_PIN, true);
            printf("%i \n", gpio_get(OUTPUT_PIN));
            sleep_ms(DELAY);
            gpio_put(OUTPUT_PIN, false);
            printf("Ended Transforming Signal\n");
            sendTransform = false;
            MicTransform = false;
        }
        if(waveFlag) {
            printf("Waving! \n");
            Servo servoRShoulder(SERVO_R_SHOULDER, 90); //false
            Servo servoShoulders[] = {servoRShoulder};
            ServoCluster servosShoulders(servoShoulders, 2);
            float endAngles3[] = {160};
            servosShoulders.moveMultipleServosSmoothly(endAngles3, 2000);
            sleep_ms(4000);
            float endAngles4[] = {90};
            servosShoulders.moveMultipleServosSmoothly(endAngles4, 2000);
            waveFlag = false;
            printf("Waved! \n");
        }
        else if(forwardFlag) {
            moveForward();
            forwardFlag = false;
        }
        else if(backwardFlag) {
            moveBackward();
            backwardFlag = false;
        }
        else if(rightFlag) {
            moveRight();
            rightFlag = false;
        }
        else if(leftFlag) {
            moveLeft();
            leftFlag = false;
        }
    }
}