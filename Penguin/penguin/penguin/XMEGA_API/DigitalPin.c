/*
 * _GPIO.c
 *
 * Created: 08-May-16 10:02:26 PM
 *  Author: emon1
 */ 


#include "DIGITALPIN.h"

void DigitalPin_Config(DigitalPin_t *pin ,bool slewRateEnable, bool invertEnable,PORT_OPC_t opc, PORT_ISC_t isc){
	uint8_t temp = (uint8_t) opc |isc |(slewRateEnable ? PORT_SRLEN_bm : 0) |(invertEnable ? PORT_INVEN_bm : 0);
	
	/* Save status register. */
	uint8_t sreg = SREG;
	cli();
	PORTCFG.MPCMASK = pin->_pin;
	pin->_port->PIN0CTRL = temp;
	/* Restore status register. */
	SREG = sreg;	
}


void DigitalPin_SetDIr(DigitalPin_t *pin , bool dir){
	if(dir) pin->_port->DIRSET = (1<< pin->_pin);
}

void DigitalPin_SetValue(DigitalPin_t *pin){
	pin->_port->OUTSET = (1<< pin->_pin);
}

void DigitalPin_ClearValue(DigitalPin_t *pin){
	pin->_port->OUTCLR = (1<< pin->_pin);
}

void DigitalPin_ToggleValue(DigitalPin_t *pin){
	pin->_port->OUTTGL = (1<< pin->_pin);
}
bool DigitalPin_GetValue(DigitalPin_t *pin){
	bool pinValue;
	if((pin->_port->IN)&(1<<pin->_pin)) pinValue = 0 ;
	else pinValue = 1;
	return pinValue;
}