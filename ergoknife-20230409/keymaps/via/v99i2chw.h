#pragma once

#include <stdint.h>
//#include <stdbool.h>
#include "i2c.h"

// V99 Registers

//------------------------------------------------------------------------------------
// device_address (0x36) + 1bit_write =  (first out) MSB[ 0110110 + 0 ] LSB (last out)
// device_address (0x36) + 1bit_read  =  (first out) MSB[ 0110110 + 1 ] LSB (last out)
//------------------------------------------------------------------------------------
// read register
// start 7bit_device_address + 1bit_write + Ack, 8bit_register_address + Ack,
// start 7bit_device_address + 1bit_read  + Ack, read_8bit_readdata + Nack,
// stop

// => (start dev_write ack, regadd ack, start dev_read ack, readdata nack, stop)
//------------------------------------------------------------------------------------
// write register
// start 7bit_device_address + 1bit_write + Ack, 8bit_register_address + Ack,
// write_8bit_data + Ack,
// stop

// => (start dev_write ack, regadd ack,                     writedata ack stop.)
//------------------------------------------------------------------------------------

#define DEVICE_ADDRESS 0x36  //0x36 (I2C device address)
#define REG_PRODUCT_ID 0x00
#define V99_ID 0x11

//v99 register

#define REG_CTRL0 0x00
#define REG_MOTION 0x02
#define FLAG_MOTION 0b10000000

#define REG_DELTA_X 0x03
#define REG_DELTA_Y 0x04
#define REG_MOUSE_CONTROL 0x0d
#define REG_CHIP_RESET 0x3a  // write 0x5A reset device
#define REG_INV_REV_ID 0x3f  // 0xff


#ifdef CONSOLE_ENABLE
void print_byte(uint8_t byte);
#endif

typedef struct {
    int8_t dx;
    int8_t dy;
} report_v99_t;

//i2c_status_t v99_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t* data, uint16_t timeout);
//i2c_status_t v99_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout);
//i2c_status_t v99_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t timeout);

#if 1
//void v99_write(uint8_t addr,uint8_t data);
uint8_t v99_read(uint8_t addr);

void v99_init(void);
//report_v99_t v99_read_burst(void);
#endif
//int8_t convert_twoscomp(uint8_t data);
//void v99_set_cpi(uint8_t cpi);
//bool v99_check_signature(void);
