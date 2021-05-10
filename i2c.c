/*
 * File:   i2c.c
 * Author: Jose Borrayo
 *
 * Created on April 16, 2021, 10:23 AM
 */


#include "xc.h"
#include <p24FJ128GC006.h>
#define FCY 8000000UL
#include <libpic30.h>

#include	"i2c.h"


#define I2C_SCL_TRIS TRISDbits.TRISD10
#define I2C_SCL_PULLUP LATDbits.LATD10
#define I2C_SDA_TRIS TRISDbits.TRISD9
#define I2C_SDA_PULLUP LATDbits.LATD9



void i2c_init(void) {
	I2C_SCL_PULLUP = 1;
	I2C_SDA_PULLUP = 1;
	ODCDbits.ODD9 = 1;
    	ODCDbits.ODD10 = 1;
	I2C_SCL_TRIS = 0;//output
	I2C_SDA_TRIS = 0;//output
	//I2C_SCL_TRIS = 1;//disable output
	//I2C_SDA_TRIS = 1;//disable output
	//SSP1ADD = 0x4F;//Baud rate 100 kHz
    	I2C1CON = 0x1000;//Set all bits to known state.
    	I2C1CONbits.I2CEN = 0;//Disable until everything set up. Pins will be I/0
	I2C1BRG = 9;
    	I2C1CONbits.DISSLW = 0;//Enable slew rate control for 400kHz operation.
    	//IFS1bits.MI2C1IF = 0;//Clear I2C master int flag.
    	I2C1CONbits.I2CEN = 1;
    	//SSP1ADD = 0x13;//Baud rate 400 kHz
	//SSP1STAT = 0x00;//SMP=0 (Slew rate control enabled for high speed mode), CKE=0 (Disable SMbus specific inputs)
	//SSP1CON1 = 0x28;//SSPEN=1 (Enable SSP unit), SSP1M<3:0>=1000 (I2C master)
}

void i2c_start(void) {
	I2C1CONbits.SEN = 1;
	while(I2C1CONbits.SEN);
}

void i2c_restart(void) {
	//PIR1bits.SSP1IF = 0;
	I2C1CONbits.RSEN = 1;
	while(I2C1CONbits.RSEN);
}

void i2c_stop(void) {
	I2C1CONbits.PEN = 1;
	while(I2C1CONbits.PEN);
}

void i2c_write(unsigned char byte) {
	I2C1TRN = byte;
	while(I2C1STATbits.TRSTAT);
}

char i2c_read(char bLastRx) {
	I2C1CONbits.RCEN = 1;
	while(I2C1CONbits.RCEN);
	//ack sequence
	if (bLastRx) I2C1CONbits.ACKDT = 1; else I2C1CONbits.ACKDT = 0;
	I2C1CONbits.ACKEN = 1; while (I2C1CONbits.ACKEN);
	return I2C1RCV;
}

void i2c_send_byte(unsigned char i2cAddr, unsigned char reg, unsigned char data) {
	i2c_start();
	i2c_write(i2cAddr);
	i2c_write(reg);
	i2c_write(data);
	i2c_stop();
}

void i2c_send_burst(unsigned char i2cAddr, unsigned char reg, unsigned char* pcdata, unsigned char ndata){
	i2c_start();

	i2c_write(i2cAddr);
	i2c_write(reg);
	unsigned char i;
	for (i=0; i<ndata; i++) {i2c_write(pcdata[i]);}

	i2c_stop();
}

unsigned char i2c_recv_byte(unsigned char i2cAddr, unsigned char reg) {
	unsigned char ucRtn;
	i2c_start();
	i2c_write(i2cAddr);// I2C slave address
	i2c_write(reg);// Send register address
	i2c_restart();
	i2c_write(i2cAddr | 0x01);// I2C slave address + read flag
	ucRtn = i2c_read(1);// receive data
	i2c_stop();
	return ucRtn;
}

void i2c_recv_burst(unsigned char i2cAddr, unsigned char reg, unsigned char* pcdata, unsigned char ndata){
	i2c_start();
	i2c_write(i2cAddr);//address, write
	i2c_write(reg);
	i2c_restart();

	i2c_write(i2cAddr | 0x01);// I2C slave address + read flag
	unsigned char i;
	for (i=0; i<ndata; i++) {pcdata[i] = i2c_read(i == ndata-1);}
	i2c_stop();
}
