#include "i2c.h"
#include "hardware/irq.h"
#include "ServoCluster.h"
#include <string.h>

// this library is used to enable I2C communication between the Raspberry Pi Pico (Slave) and the Arduino Nano RP2040 (Master) and enable interrupt upon a read request from the master

uint8_t i2c_rx_buffer[8];
uint8_t i2c_rx_index = 0;

bool Transform = false;
bool MicTransform = false;
bool transformBack = false;

void initialize_i2c() {
    // initialize I2C
    i2c_init(I2C_PORT, I2C_BAUDRATE);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // set up the pico as slave
    i2c_set_slave_mode(I2C_PORT, true, SLAVE_ADDRESS);
    printf("I2C Initialized\n");

    // enabling interrupt on a write request from the master
    // Note that the order of these lines is important! otherwise the interrupt will not work
    i2c_hw_t *i2c = i2c_get_hw(I2C_PORT); // get a pointer to the previously initialized I2C pin
    printf("IRQ 1\n");
    i2c->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS; // This will enter the interrupt handler when the RX FIFO is full
    printf("IRQ 2\n");
    irq_set_enabled(I2C0_IRQ, true); // enabling interrupt
    printf("IRQ 3\n");
    irq_set_exclusive_handler(I2C0_IRQ, i2c_irq_handler); // setting the interrupt handler
    printf("IRQ 4\n");
}

void i2c_irq_handler() {
    printf("Handling I2C IRQ\n");
    i2c_hw_t *i2c = i2c_get_hw(I2C_PORT);
    printf("Handling I2C IRQ2\n");
    uint32_t status = i2c->intr_stat; // get the current status of the I2C
    printf("Handling I2C IR3\n");
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
        printf("Handling I2C IRQ4\n");
        // Handle received data
        i2c_rx_buffer[i2c_rx_index++] = (i2c->data_cmd & I2C_IC_DATA_CMD_DAT_BITS); // adding the received data to the buffer
        printf("Received: %.*s\n", sizeof(i2c_rx_buffer), i2c_rx_buffer);
        if (i2c_rx_index >= sizeof(i2c_rx_buffer)) { // when we receive at least one byte (character) we will enable the signal we want based on the data received so far
            printf("Buffer full, resetting index\n");
            if(i2c_rx_buffer[0] == '0' && i2c_rx_buffer[1] == 'R') {
                Transform = true; // this to enable the transformation from wifi
            }
            if(i2c_rx_buffer[0] == '0' && i2c_rx_buffer[1] == 'M') {
                MicTransform = true; // this to enable the transformation from microphone
            }
            if(i2c_rx_buffer[0] == '0' && i2c_rx_buffer[1] == 'O') {
                transformBack = true; // this will enable transformation back to the Hominoid position
            }
            i2c_rx_index = 0; // Reset index
        }
    }
    // Clear all interrupts
    (void)i2c->clr_rx_under;
    (void)i2c->clr_rx_over;
    (void)i2c->clr_tx_over;
    (void)i2c->clr_rd_req;
    (void)i2c->clr_tx_abrt;
    (void)i2c->clr_rx_done;
    (void)i2c->clr_activity;
    (void)i2c->clr_stop_det;
    (void)i2c->clr_start_det;
    (void)i2c->clr_gen_call;
}