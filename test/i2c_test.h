#ifndef I2C_TEST_H
#define I2C_TEST_H

#include "i2c.h"

#define I2C_PORT I2C0_id

void i2c_test_master_setup();
void i2c_test_master_w1r3();
void i2c_test_master_wN(uint8_t n);

#endif
