/*
 * nrf24l01p_arch_driver.h
 *
 * Created: 29-Mar-16 11:17:14 PM
 *  Author: emon1
 */ 


#ifndef NRF24L01P_ARCH_RPI3_H_
#define NRF24L01P_ARCH_RPI3_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>





#if defined (__cplusplus)
extern "C"{
#endif


#include <wiringPi.h>
#include <wiringPiSPI.h>

#define _nrf24l01p_delay_us(us) delayMicroseconds(us)
#define _nrf24l01p_delay_ms(ms) delay(ms)


#define nrf24l01p_csn_pin 6//28
#define nrf24l01p_ce_pin 10//29

#define nrf24l01p_SPI 0

#define set_bit(reg,bit) reg|= (1<<bit);
#define clr_bit(reg,bit) reg&= ~(1<<bit);
#define tgl_bit(reg,bit) reg^= (1<<bit);


static void arch_nrf24l01p_initialize(){
	wiringPiSetup () ;
	pinMode(nrf24l01p_csn_pin ,OUTPUT);//CSN pin as OUTPUT
	pinMode(nrf24l01p_ce_pin ,OUTPUT);//CE pin as OUTPUT
	wiringPiSPISetup(nrf24l01p_SPI,500000);

	digitalWrite(6,LOW);
	digitalWrite(10,LOW);

}

static void arch_nrf24l01p_ce_pin(bool state){
	digitalWrite(nrf24l01p_ce_pin,state);
}

static void arch_nrf24l01p_csn_pin(bool state){
	digitalWrite(nrf24l01p_csn_pin,state);
}

static int arch_spi_master_transcieve(uint8_t *data,  int len)
{
	int temp =  wiringPiSPIDataRW(nrf24l01p_SPI, (unsigned char*)data,len);
	return temp;
}



#if defined (__cplusplus)
}
#endif








#endif /* NRF24L01P_ARCH_RPI3_H_ */