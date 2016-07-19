/*
 * ds1302.h
 *
 * Created: 19-Jul-16 2:39:41 PM
 *  Author: Emon
 */ 


#ifndef DS1302_H_
#define DS1302_H_

#include <avr/io.h>
#include <stdbool.h>

#include "xmega_api.h"
#include "ds1302_arch_driver.h"

bool writeProtect;

enum {
	Seconds = 0x80,
	Minutes = 0x82,
	Hours = 0x84,
	Dates = 0x86,
	Months = 0x88,
	Days = 0x8A,
	Years = 0x8C,
	WriteProtect = 0x8E,
	Charge = 0x90,
	ClockBurst = 0xBE,
	RAMBase = 0xC0
};
// 
void ds1302_initialize(void){
	ds1302_io_pin_output();
	ds1302_io_pin_input();
	ds1302_clk_pin_output();
	ds1302_rst_pin_output();
	
	ds1302_arch_initialize();
	
}




void ds1302_writeByte(uint8_t data){
	ds1302_io_pin_output();
	for (int i = 0; i<8; i++) {

		if(data&0x01) ds1302_io_pin_high();
		else ds1302_io_pin_low();
		
		wait_us(1);
		ds1302_clk_pin_high();
		wait_us(1);
		ds1302_clk_pin_low();
		data >>= 1;
	}
	ds1302_io_pin_input();
}


uint8_t ds1302_readByte(void){
	char retval = 0;
	
	ds1302_io_pin_input();

	for (int i = 0; i<8; i++) {
		bool bitval = ds1302_io_pin_val();
		retval |= (bitval << i);
		wait_us(1);
		ds1302_clk_pin_high();
		wait_us(1);
		ds1302_clk_pin_low();
	}
	return retval;
}

uint8_t ds1302_readReg(uint8_t reg)
{
	uint8_t retval;
	
	ds1302_rst_pin_high();
 	wait_us(4);
	ds1302_writeByte(reg);
	retval = ds1302_readByte();
 	wait_us(4);
	ds1302_rst_pin_low();
	return retval;
}
// 
void ds1302_writeReg(uint8_t reg, uint8_t val)
{
	if (writeProtect) {
		writeProtect = false;
		ds1302_writeReg(WriteProtect, 0);
	}
	ds1302_rst_pin_high();
	wait_us(4);
	ds1302_writeByte(reg);
	ds1302_writeByte(val);
	wait_us(4);
	ds1302_rst_pin_low();
}


void ds1302_setTimestamp(time_t t)
{

	struct tm *_t = localtime(&t);
	
/*	_t->tm_year -= 30 ;*/
	
	ds1302_writeReg(Seconds, (_t->tm_sec % 10) + ((_t->tm_sec / 10) << 4));
	ds1302_writeReg(Minutes, (_t->tm_min % 10) + ((_t->tm_min / 10) << 4));
	ds1302_writeReg(Hours, (_t->tm_hour % 10) + ((_t->tm_hour / 10) << 4));
	ds1302_writeReg(Dates, (_t->tm_mday % 10) + ((_t->tm_mday / 10) << 4));
	ds1302_writeReg(Months, ((_t->tm_mon + 1) % 10) + (((_t->tm_mon + 1) / 10) << 4));
	ds1302_writeReg(Days, _t->tm_wday + 1);
	ds1302_writeReg(Years, ((_t->tm_year - 100) % 10) + (((_t->tm_year - 100) / 10) << 4));

}

time_t ds1302_getTimestamp()
{
	char regs[7];
	ds1302_rst_pin_high();
	wait_us(4);
	ds1302_writeByte(ClockBurst | 1);
	for (int i = 0; i<7; i++)
	regs[i] = ds1302_readByte();
	ds1302_rst_pin_low();
	
	asm("nop");
	
	struct tm _t;
	_t.tm_sec = (regs[0] & 0xF) + (regs[0] >> 4) * 10;
	_t.tm_min = (regs[1] & 0xF) + (regs[1] >> 4) * 10;
	_t.tm_hour = (regs[2] & 0xF) + (regs[2] >> 4) * 10;
	_t.tm_mday = (regs[3] & 0xF) + (regs[3] >> 4) * 10;
	_t.tm_mon = (regs[4] & 0xF) + (regs[4] >> 4) * 10 - 1;
	_t.tm_year = (regs[6] & 0xF) + (regs[6] >> 4) * 10 + 100;


/*	_t.tm_year += 30;*/
		
	asm("nop");
	time_t t;
	t = mktime(&_t);
	
	return t;

}


#endif /* DS1302_H_ */