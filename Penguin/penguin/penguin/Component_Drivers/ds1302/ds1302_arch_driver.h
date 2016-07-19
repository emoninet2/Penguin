/*
 * ds1302_arch_driver.h
 *
 * Created: 19-Jul-16 3:12:54 PM
 *  Author: Emon
 */ 


#ifndef DS1302_ARCH_DRIVER_H_
#define DS1302_ARCH_DRIVER_H_


#include <time.h>

DigitalPin_t ds1302_clk = {&PORTB, 5};
DigitalPin_t ds1302_io = {&PORTB, 6};
DigitalPin_t ds1302_rst = {&PORTB, 7};


#define wait_us(us)	vTaskDelay(us)

#define ds1302_io_pin_output()	DigitalPin_SetDIr(&ds1302_io , 1)
#define ds1302_io_pin_input()	DigitalPin_SetDIr(&ds1302_io , 0)
#define ds1302_clk_pin_output()	DigitalPin_SetDIr(&ds1302_clk , 1)
#define ds1302_rst_pin_output()	DigitalPin_SetDIr(&ds1302_rst , 1)

#define ds1302_io_pin_val()		DigitalPin_GetValue(&ds1302_io)

#define ds1302_io_pin_high()	DigitalPin_SetValue(&ds1302_io)
#define ds1302_io_pin_low()		DigitalPin_ClearValue(&ds1302_io)
#define ds1302_clk_pin_high()	DigitalPin_SetValue(&ds1302_clk)
#define ds1302_clk_pin_low()	DigitalPin_ClearValue(&ds1302_clk)
#define ds1302_rst_pin_high()	DigitalPin_SetValue(&ds1302_rst)
#define ds1302_rst_pin_low()	DigitalPin_ClearValue(&ds1302_rst)


void ds1302_arch_initialize(){
		DigitalPin_Config(&ds1302_io ,false, false,PORT_OPC_PULLDOWN_gc , PORT_ISC_INPUT_DISABLE_gc);
}



#endif /* DS1302_ARCH_DRIVER_H_ */