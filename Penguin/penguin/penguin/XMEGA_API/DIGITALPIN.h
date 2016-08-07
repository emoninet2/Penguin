/*
 * _GPIO.h
 *
 * Created: 08-May-16 9:22:49 PM
 *  Author: emon1
 */ 


#ifndef DIGITALPIN_H_
#define DIGITALPIN_H_

#include "api_common.h"

typedef struct DigitalPin_enum{
	PORT_t *_port;
	uint8_t _pin;	
}DigitalPin_t;



void DigitalPin_Config(DigitalPin_t *pin ,bool slewRateEnable, bool invertEnable,PORT_OPC_t opc, PORT_ISC_t isc);
void DigitalPin_SetDir(DigitalPin_t *pin , bool dir);
void DigitalPin_SetValue(DigitalPin_t *pin);
void DigitalPin_ClearValue(DigitalPin_t *pin);
void DigitalPin_ToggleValue(DigitalPin_t *pin);
bool DigitalPin_GetValue(DigitalPin_t *pin);


#endif /* DIGITALPIN_H_ */