/*
 * nrf24l01p_arch_driver.h
 *
 * Created: 29-Mar-16 11:17:14 PM
 *  Author: emon1
 */ 


#ifndef NRF24L01P_ARCH_DRIVER_H_
#define NRF24L01P_ARCH_DRIVER_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#include "../XMEGA_API/xmega_api.h"


#if defined (__cplusplus)
extern "C"{
#endif






#define _nrf24l01p_delay_us _delay_us
#define _nrf24l01p_delay_ms _delay_ms


#define set_bit(reg,bit) reg|= (1<<bit);
#define clr_bit(reg,bit) reg&= ~(1<<bit);
#define tgl_bit(reg,bit) reg^= (1<<bit);


static DigitalPin_t nrf24l01p_csn_pin = {&PORTC, 4};
static DigitalPin_t nrf24l01p_ce_pin = {&PORTC, 0};

#define NRF24L01P_CSN_PIN_PORT	PORTC
#define NRF24L01P_CSN_PIN_BIT	4

#define NRF24L01P_CE_PIN_PORT	PORTC
#define NRF24L01P_CE_PIN_BIT	0

#define NRF24L01P_SPI SPIC
#define NRF24L01P_SPI_PORT PORTC


static void arch_nrf24l01p_ce_pin(bool state){
	//if(state) NRF24L01P_CE_PIN_PORT.OUTSET = (1<<NRF24L01P_CE_PIN_BIT) ;
	//else NRF24L01P_CE_PIN_PORT.OUTCLR = (1<<NRF24L01P_CE_PIN_BIT);
	
	if(state) DigitalPin_SetValue(&nrf24l01p_ce_pin) ;
	else DigitalPin_ClearValue(&nrf24l01p_ce_pin);
	
}

static void arch_nrf24l01p_csn_pin(bool state){
// 	if(state) NRF24L01P_CSN_PIN_PORT.OUTSET = (1<<NRF24L01P_CSN_PIN_BIT) ;
// 	else NRF24L01P_CSN_PIN_PORT.OUTCLR = (1<<NRF24L01P_CSN_PIN_BIT);
	if(state) DigitalPin_SetValue(&nrf24l01p_csn_pin) ;
	else DigitalPin_ClearValue(&nrf24l01p_csn_pin);
}

static void arch_nrf24l01p_initialize(){
	NRF24L01P_SPI_PORT.DIRSET = (1<<5) | (1<<4) | (1<<7);
	NRF24L01P_SPI.CTRL = (1<<SPI_ENABLE_bp) | (1<<SPI_MASTER_bp);
	NRF24L01P_CSN_PIN_PORT.DIRSET = (1<<NRF24L01P_CSN_PIN_BIT);
	NRF24L01P_CE_PIN_PORT.DIRSET = (1<<NRF24L01P_CE_PIN_BIT);
}


static uint8_t arch_spi_master_transmit_byte_val(uint8_t data)
{
	NRF24L01P_SPI.DATA = data;
	while(!(SPIC.STATUS &(1<<SPI_IF_bp)));
	return SPIC.DATA;
}

static uint8_t arch_spi_master_transmit_byte_ref(uint8_t *data)
{
	NRF24L01P_SPI.DATA = *data;
	while(!(NRF24L01P_SPI.STATUS &(1<<SPI_IF_bp)));
	return NRF24L01P_SPI.DATA;
}

static void arch_spi_master_receive_byte_ref(uint8_t *data){
	//transmit dummy byte
	arch_spi_master_transmit_byte_val(0xFF);
	*data = NRF24L01P_SPI.DATA;
}


#if defined (__cplusplus)
}
#endif

#endif /* NRF24L01P_ARCH_DRIVER_H_ */