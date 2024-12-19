#include <stdio.h>
#include "Servo.h"
#include <i2c.h>
#include "ServoCluster.h"
#include <communication.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/stdio_usb.h>
#include "pico/pdm_microphone.h"
#include "tusb.h"
#include <unistd.h>
#include <string.h>
#include <pico/malloc.h>

const struct pdm_microphone_config config = {
    // GPIO pin for the PDM DAT signal
    .gpio_data = 22,

    // GPIO pin for the PDM CLK signal
    .gpio_clk = 23,

    // PIO instance to use
    .pio = pio0,

    // PIO State Machine instance to use
    .pio_sm = 0,

    // sample rate in Hz
    .sample_rate = 16000,

    // number of samples to buffer
    .sample_buffer_size = 128,
};

#define FEATURE_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE

float features[FEATURE_SIZE];
int16_t buffer[128];
int buffer_length = 0;

volatile int samples_read = 0;
size_t feature_index = 0;

void clear_buffer() {
    memset(buffer, 0, sizeof(buffer));
}
void clear_features() {
    feature_index = 0;
    memset(features, 0, sizeof(features));
}
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
  memcpy(out_ptr, buffer + offset, length * sizeof(float));
  return 0;
}

void print_inference_result(ei_impulse_result_t result) {

    // Print how long it took to perform inference
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
            result.timing.dsp,
            result.timing.classification,
            result.timing.anomaly);

    // Print the prediction results (object detection)
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    ei_printf("Object detection bounding boxes:\r\n");
    for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue;
        }
        ei_printf("  %s (%f) [ x: %u, y: %u, width: %u, height: %u ]\r\n",
                bb.label,
                bb.value,
                bb.x,
                bb.y,
                bb.width,
                bb.height);
    }

    // Print the prediction results (classification)
#else
    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }
#endif

    // Print anomaly result (if it exists)
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
#endif

}
void on_pdm_samples_ready()
    {
        // callback from library when all the samples in the library
        // internal sample buffer are ready for reading 
        samples_read = pdm_microphone_read(buffer, 512);
    }


/////------Marawan Mic------/////


#define DELAY 3000
int i = 0;
uint8_t send_buffer[8] = "0R";
uint8_t send_buffer2[8] = "0O";
uint8_t send_buffer3[8] = "0M";
// uint8_t receive_buffer[8];
ServoCluster* globalservosThigh = nullptr;
ServoCluster* globalservosThighKneesandFeet = nullptr;

bool transform = false;
bool state = 0;

void TransformLower(ServoCluster &servosThighKneesandFeet) {
	printf("Transforming to Lower\n");

	sleep_ms(3000);

	float endAngles9[] = {90.0 + 25, 99.0 - 25, 0.0 + 25.0, 180.0 - 25.0 ,86.0+20, 90.0- 20, 70,110};
	globalservosThighKneesandFeet->moveMultipleServosSmoothly(endAngles9, 1000); // Reduced step delay for smoother movement

	i2c_write_blocking(I2C_PORT, SLAVE_ADDRESS, send_buffer, 8, false);
	printf("Sent: %s\n", send_buffer);
	sleep_ms(500);
	float endAngles6[] = {90.0 - 52, 99.0 + 52, 0.0 + 50, 180.0 - 50 ,90 + 70, 90 - 70, 70, 110};
	globalservosThighKneesandFeet->moveMultipleServosSmoothly(endAngles6, 2500); // Reduced step delay for smoother movement

	float endAngles7[] = {90.0 - 90, 93.0 + 90, 0.0 , 180.0 ,90 + 70, 90 - 70, 180, 0};
	globalservosThighKneesandFeet->moveMultipleServosSmoothly(endAngles7, 2000); // Reduced step delay for smoother movement

	sleep_ms(5000);
}

void transformBack(ServoCluster &servosThighKneesandFeet) {
	printf("Transforming Back\n");

	float endAngles8[] = {90.0 - 32, 99.0 + 24, 0.0 +40, 180.0 - 40 ,90 + 60, 90 - 60, 20, 160};
	globalservosThighKneesandFeet->moveMultipleServosSmoothly(endAngles8, 5000); // Reduced step delay for smoother movement

	i2c_write_blocking(I2C_PORT, SLAVE_ADDRESS, send_buffer2, 8, false);
	printf("Sent: %s\n", send_buffer);

	
	float endAngles9[] = {90.0 + 25, 99.0 - 25, 0.0 + 25.0, 180.0 - 25.0 ,86.0+20, 90.0- 20, 140,40};
	globalservosThighKneesandFeet->moveMultipleServosSmoothly(endAngles9, 3500); // Reduced step delay for smoother movement

	sleep_ms(5000);
}



void inputPinHandler(uint gpio, uint32_t events) {
	printf("Input Pin Handler\n");
	transform = true;
}

int main() {
	stdio_init_all();
	sleep_ms(DELAY);
	initialize_i2c();
	initialize_input_pin();

	sleep_ms(3000);

	
	printf("hello PDM microphone\n");

     // initialize the PDM microphone
    if (pdm_microphone_init(&config) < 0) {
        printf("PDM microphone initialization failed!\n");
        while (1) { tight_loop_contents(); }
    }


    pdm_microphone_set_samples_ready_handler(on_pdm_samples_ready);
    // start capturing data from the PDM microphone
    if (pdm_microphone_start() < 0) {
        printf("PDM microphone start failed!\n");
        while (1) { tight_loop_contents(); }
    }


	    ei_impulse_result_t result = {nullptr};

    ei_printf("Edge Impulse standalone inferencing (Raspberry Pi Pico)\n");
    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        ei_printf("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
        EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }
            printf("here 0\n");

	printf("Initialising Servos\n");
	Servo servoRThigh(SERVO_R_THIGH, 90.0 + 25);
	Servo servoLThigh(SERVO_L_THIGH, 99.0 - 25);

	Servo servoRKnee(SERVO_R_KNEE, 0.0 + 25.0, false, 500, 2440); //false
	Servo servoLKnee(SERVO_L_KNEE, 180.0 - 25.0, false, 520, 2480); //false

	Servo servoRFeet(SERVO_R_FEET, 86.0+20, true, 520, 2480); //true There is difference 4 degree minus in right feet
	Servo servoLFeet(SERVO_L_FEET, 90.0- 20, true, 520, 2450 ); //true

	Servo servoRStand(SERVO_R_STAND, 140, true); //false
	Servo servoLStand(SERVO_L_STAND,  40 ); //false

	Servo servosThighArray[] = {servoRThigh, servoLThigh};
	ServoCluster servosThigh(servosThighArray, 2);
	globalservosThigh = &servosThigh;

	Servo servosThighKneesandFeetArray[] = {servoRThigh, servoLThigh, servoRKnee, servoLKnee, servoRFeet, servoLFeet, servoRStand, servoLStand};
	ServoCluster servosThighKneesandFeet(servosThighKneesandFeetArray, 8);

	Servo servosStandsArray[] = {servoRStand, servoLStand};
	ServoCluster servosStands(servosStandsArray, 2);
	globalservosThighKneesandFeet = &servosThighKneesandFeet;
	printf("Servos Initialised\n");

	sleep_ms(3000);
	// TransformLower(servosThighKneesandFeet);
	while (true) {
		//sleep_ms(2000);
		printf("%i \n", gpio_get(inputPin));
		// if(gpio_get(inputPin) == 1) {
		// 	if(state == 1) {
		// 		transformBack(servosThighKneesandFeet);
		// 		state = 0;
		// 	}
		// 	else if(state == 0) {
		// 		TransformLower(servosThighKneesandFeet);
		// 		state = 1;
		// 	}
		// }
		while (samples_read == 0) {
				tight_loop_contents();
			}
				
			int sample_count = samples_read;
				samples_read = 0;

				// Add samples from buffer to features
			for (int i = 0; i < sample_count; i++) {
				if (feature_index < FEATURE_SIZE) {
					features[feature_index++] = (float)buffer[i];
						
				}
				clear_buffer();
				// Process features when full
				if (feature_index >= FEATURE_SIZE) {
					printf("Features filled. Running inference...\n");

					signal_t features_signal;
					features_signal.total_length = FEATURE_SIZE;
					features_signal.get_data = &raw_feature_get_data;
					ei_impulse_result_t result = {nullptr};
					EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
					if (res != EI_IMPULSE_OK) {
						ei_printf("ERR: Failed to run classifier (%d)\n", res);
					} else {
						print_inference_result(result);
						printf("%f \n",result.classification[0].value);
						if(result.classification[1].value <= 0.5) {
							printf("Detcected Voice\n");
							i2c_write_blocking(I2C_PORT, SLAVE_ADDRESS, send_buffer3, 8, false);
							printf("Sent: %s\n", send_buffer3);	
						}
					}

						// Clear features for the next round
					clear_features();
				}
			}
		// i2c_write_blocking(I2C_PORT, SLAVE_ADDRESS, send_buffer, 8, false);
		// printf("Sent: %s\n", send_buffer);
		// printf("Will Start Receiving: %s\n", receive_buffer);
		// i2c_read_blocking(I2C_PORT, SLAVE_ADDRESS, receive_buffer, 8, false);
		// printf("Received: %s\n", receive_buffer);
}
}