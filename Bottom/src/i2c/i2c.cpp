#include <i2c.h>

void initialize_i2c() {
	// initialize I2C
	i2c_init(I2C_PORT, I2C_BAUDRATE);
	gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
	gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(SDA_PIN);
	gpio_pull_up(SCL_PIN);
}