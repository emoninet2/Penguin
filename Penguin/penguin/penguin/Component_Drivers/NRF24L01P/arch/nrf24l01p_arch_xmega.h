/*
 * nrf24l01p_arch_driver.h
 *
 * Created: 29-Mar-16 11:17:14 PM
 *  Author: emon1
 */ 


#ifndef NRF24L01P_ARCH_XMEGA_H_
#define NRF24L01P_ARCH_XMEGA_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#if defined (__cplusplus)
extern "C"{
#endif


#include <avr/io.h>
#include <util/delay.h>
#include "../XMEGA_API/xmega_api.h"

#define _nrf24l01p_delay_us _delay_us
#define _nrf24l01p_delay_ms _delay_ms


#define set_bit(reg,bit) reg|= (1<<bit);
#define clr_bit(reg,bit) reg&= ~(1<<bit);
#define tgl_bit(reg,bit) reg^= (1<<bit);


static DigitalPin_t nrf24l01p_csn_pin = {&PORTC, 4};
static DigitalPin_t nrf24l01p_ce_pin = {&PORTB, 7};

#define NRF24L01P_CSN_PIN_PORT	PORTC
#define NRF24L01P_CSN_PIN_BIT	4

#define NRF24L01P_CE_PIN_PORT	PORTB
#define NRF24L01P_CE_PIN_BIT	7

#define NRF24L01P_SPI SPIC
#define NRF24L01P_SPI_PORT PORTC

static void arch_nrf24l01p_initialize(){
	NRF24L01P_SPI_PORT.DIRSET = (1<<5) | (1<<4) | (1<<7);
	NRF24L01P_SPI_PORT.DIRCLR = (1<<6);
	NRF24L01P_SPI.CTRL = (1<<SPI_ENABLE_bp) | (1<<SPI_MASTER_bp);
	NRF24L01P_CSN_PIN_PORT.DIRSET = (1<<NRF24L01P_CSN_PIN_BIT);
	NRF24L01P_CE_PIN_PORT.DIRSET = (1<<NRF24L01P_CE_PIN_BIT);
}

static void arch_nrf24l01p_ce_pin(bool state){
	
	if(state) DigitalPin_SetValue(&nrf24l01p_ce_pin) ;
	else DigitalPin_ClearValue(&nrf24l01p_ce_pin);
	
}

static void arch_nrf24l01p_csn_pin(bool state){
	if(state) DigitalPin_SetValue(&nrf24l01p_csn_pin) ;
	else DigitalPin_ClearValue(&nrf24l01p_csn_pin);
}



static int arch_spi_master_transcieve(uint8_t *data,  int len)
{
	int i;
	for(i=0;i<len;i++){
		NRF24L01P_SPI.DATA = data[i];
		while(!(NRF24L01P_SPI.STATUS &(1<<SPI_IF_bp)));
		data[i]=  NRF24L01P_SPI.DATA;
	}
	
}

#if defined (__cplusplus)
}
#endif








#endif /* NRF24L01P_ARCH_XMEGA_H_ */