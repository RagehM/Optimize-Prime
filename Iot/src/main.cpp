// This part is related to the Pico W (Slave)
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

const char WIFI_SSID[] = "xxxxxxx"; // Wifi SSID (Name)
const char WIFI_PASSWORD[] = "xxxxxx"; // WIFI Password

const char * ssi_tags[] = {"camera", "micFlag", "state"}; // this is the tags or values that we want to show on the UI of the web server

ServoCluster* globalServosInners = nullptr;
ServoCluster* globalServosElbows = nullptr; 

bool sendTransform = false;

// DC Motors Flags
bool forwardFlag = false;
bool backwardFlag = false;
bool rightFlag = false;
bool leftFlag = false;

bool waveFlag = false;

int state = 0; // the state is initially 0 which is the state of the Hominoid
int micFlag = 0; // the mic is initially disabled and it should be enabled first from the web server
int cameraValue = 0; // the camera initially does not detect anything

char operation[10] = ""; // we will store here the operation we wanted to do from the web server. Why that ? because the webserver detects the operation from the url so when the url becomes for example /transform then the server will keep detecting the same operation until we change it. So we need to prevent that

// Left Right Backward Forward is the functions used to handle the req to move left right backward and forward
const char* Left(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "left") != 0) {
        strcpy(operation, "left"); // operation = "left"
        leftFlag = true;
    }
    return "/index.shtml";
}

const char* Right(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "right") != 0) {
        strcpy(operation, "right"); // operation = "right"
        rightFlag = true;
    }
    return "/index.shtml";
}

const char* Backward(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "backward") != 0) {
        strcpy(operation, "backward"); // operation = "backward"
        backwardFlag = true;
    }
    return "/index.shtml";
}

const char* Forward(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "forward") != 0) {
        strcpy(operation, "forward"); // operation = "forward"
        forwardFlag = true;
    }
    return "/index.shtml";
}

const char* transformSignal(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "transform") != 0) {
        strcpy(operation, "transform"); // operation = "transform"
        sendTransform = true;
    }
    return "/index.shtml";
}

const char* toggleMic(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    if(strcmp(operation, "mic") != 0) {
        strcpy(operation, "mic"); // operation = "mic"
        if(micFlag == 0) { // toggle the mic flag
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
        strcpy(operation, "wave"); // operation = "wave"
        waveFlag = true;
    }
    return "/index.shtml";
}

// this function will loop on all the tags and update each tag with the value we want to show on the web server
u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
    size_t printed;
    switch (iIndex) {
    case 0: // camera
    {
        cameraValue = readCamera(); // get the camera value
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

// this will handle the Common Gateway Interface (CGI) requests by checking the url of the request and then calling the function that handles the request
const tCGI cgi_handlers[] = {
    {
    "/transform", transformSignal,
    },
    {
    "/mic", toggleMic,
    },
    {
    "/wave", wave // NOTE: that this will will cause RUNTIME ERROR and the Pico W will need to be restarted
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
    stdio_init_all(); // initialize the stdio

    sleep_ms(8000);

    initialize_i2c(); // initialize the i2c
    initialize_output_pin(); // initialize the output pin
    initialize_reset_pin(); // initialize the reset pin
    setupDCMotors(2, 3, 4, 5); // initialize the DC Motors
    setupCamera(); // initialize the camera
    
    printf("Starting Wifi Connection\n");

    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();

    sleep_ms(5000);

    // connect to the wifi using the SSID and the password
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
    }

    printf("Connected! \n");

    struct netif *netif = netif_list;

    if (netif_is_up(netif)) { // this is used to print the IP address assigned to Pico W
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
        if(Transform) { // if transform flag is true
            printf("Transforming! \n");
            float endAngles3[] = {0,180};
            globalServosInners->moveMultipleServosSmoothly(endAngles3, 2000);
            Transform = false; // disabling the transform flag
            state = 1; // change the state to 1 which is also the state of the Car
            printf("Transformed! \n");
        }
        else if(transformBack) {
            printf("Transforming Back! \n");
            float endAngles3[] = {90,90};
            globalServosInners->moveMultipleServosSmoothly(endAngles3, 2000);
            transformBack = false; // disabling the transform flag
            state = 0; // change the state to 0 which is the state of the Hominoid
            printf("Transformed Back! \n");
        }
        // if the we want to transform from Wifi or we got a signal to transform from the Mic on the Arduino Nano RP2040 and the mic is enabled or we want to perform an emergency transformation
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
        // if we want to wave by hand
        if(waveFlag) {
            printf("Waving!\n");
            Servo servoRShoulder(SERVO_R_SHOULDER, 90); //false
            Servo servoShoulders[] = {servoRShoulder};
            ServoCluster servosShoulders(servoShoulders, 2);
            float endAngles3[] = {160};
            servosShoulders.moveMultipleServosSmoothly(endAngles3, 2000);
            sleep_ms(4000);
            float endAngles4[] = {90};
            servosShoulders.moveMultipleServosSmoothly(endAngles4, 2000);
            waveFlag = false; // disable the wave flag
            printf("Waved! \n");
        }
        // if move forward is enabled
        else if(forwardFlag) {
            moveForward();
            forwardFlag = false;
        }
        // if move backward is enabled
        else if(backwardFlag) {
            moveBackward();
            backwardFlag = false;
        }
        // if move right is enabled
        else if(rightFlag) {
            moveRight();
            rightFlag = false;
        }
        // if move left is enabled
        else if(leftFlag) {
            moveLeft();
            leftFlag = false;
        }
    }
}