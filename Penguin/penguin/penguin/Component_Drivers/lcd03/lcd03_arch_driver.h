/*
 * lcd03_arch_driver.h
 *
 * Created: 11-Jun-16 9:40:24 PM
 *  Author: emon1
 */ 


#ifndef LCD03_ARCH_DRIVER_H_
#define LCD03_ARCH_DRIVER_H_

#include <avr/io.h>
#include <stdbool.h>
#include "twi_master_driver.h"


TWI_Master_t lcd03i2c;


static void lcd03_arch_init(){
	TWI_MasterInit(&lcd03i2c,
	&TWIE,
	TWI_MASTER_INTLVL_OFF_gc,
	TWI_BAUD(F_CPU, 100000));
}



static void lcd03_arch_i2c_send(uint8_t data, uint8_t lcd03_address){

	uint8_t writeData[] = {0,data};
	TWI_MasterWrite(&lcd03i2c,
	lcd03_address,
	writeData,
	sizeof(writeData));
	asm("nop");


	while(lcd03i2c.result != TWIM_RESULT_OK){
		while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
		TWI_MasterWriteHandler(&lcd03i2c);
		
	}
	
	asm("nop");
}



#endif /* LCD03_ARCH_DRIVER_H_ */