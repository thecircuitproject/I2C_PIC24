#ifndef _I2C_H_
#define _I2C_H_

extern void i2c_init();
extern void i2c_send_byte(unsigned char i2cAddr, unsigned char reg, unsigned char data);
extern void i2c_send_burst(unsigned char i2cAddr, unsigned char reg, unsigned char* pcdata, unsigned char ndata);
extern unsigned char i2c_recv_byte(unsigned char i2cAddr, unsigned char reg);
extern void i2c_recv_burst(unsigned char i2cAddr, unsigned char reg, unsigned char* pcdata, unsigned char ndata);
extern void i2c_write(unsigned char byte);
extern void i2c_start(void);
extern void i2c_stop(void);

#endif