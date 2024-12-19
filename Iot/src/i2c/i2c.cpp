#include "i2c.h"
#include "hardware/irq.h"
#include "ServoCluster.h"
#include <string.h>

uint8_t i2c_rx_buffer[8];
uint8_t i2c_rx_index = 0;

bool Transform = false;
bool MicTransform = false;
bool transformBack = false;

void initialize_i2c() {
    i2c_init(I2C_PORT, I2C_BAUDRATE);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    i2c_set_slave_mode(I2C_PORT, true, SLAVE_ADDRESS);
    printf("I2C Initialized\n");

    i2c_hw_t *i2c = i2c_get_hw(I2C_PORT);
    printf("IRQ 1\n");
    i2c->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS;
    printf("IRQ 2\n");
    irq_set_enabled(I2C0_IRQ, true);
    printf("IRQ 3\n");
    irq_set_exclusive_handler(I2C0_IRQ, i2c_irq_handler);
    printf("IRQ 4\n");
}

void i2c_irq_handler() {
    printf("Handling I2C IRQ\n");
    i2c_hw_t *i2c = i2c_get_hw(I2C_PORT);
    printf("Handling I2C IRQ2\n");
    uint32_t status = i2c->intr_stat;
    printf("Handling I2C IR3\n");
    if (status & I2C_IC_INTR_STAT_R_RX_FULL_BITS) {
        printf("Handling I2C IRQ4\n");
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
        // Handle received data
        i2c_rx_buffer[i2c_rx_index++] = (i2c->data_cmd & I2C_IC_DATA_CMD_DAT_BITS);
        printf("Received: %.*s\n", sizeof(i2c_rx_buffer), i2c_rx_buffer);
        if (i2c_rx_index >= sizeof(i2c_rx_buffer)) {
            printf("Buffer full, resetting index\n");
            if(i2c_rx_buffer[0] == '0' && i2c_rx_buffer[1] == 'R') {
                Transform = true;
            }
            if(i2c_rx_buffer[0] == '0' && i2c_rx_buffer[1] == 'M') {
                MicTransform = true;
            }
            if(i2c_rx_buffer[0] == '0' && i2c_rx_buffer[1] == 'O') {
                transformBack = true;
            }
            i2c_rx_index = 0; // Reset index
        }
    }

    // if (status & I2C_IC_INTR_STAT_R_TX_EMPTY_BITS) {
    //     // Provide data to master when it requests a read
    //     if (i2c_tx_index < sizeof(i2c_tx_buffer)) {
    //         i2c->data_cmd = i2c_tx_buffer[i2c_tx_index++];
    //         printf("Master Should Read: %.*s\n", sizeof(i2c_tx_index), i2c_tx_buffer);
    //     } else {
    //         // If no more data, send 0 or a default value
    //         i2c->data_cmd = 0;
    //     }
    // }

    // Clear all handled interrupts
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