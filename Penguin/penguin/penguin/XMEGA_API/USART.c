/*
 * USART.c
 *
 * Created: 09-May-16 9:29:15 AM
 *  Author: emon1
 */ 

#include "USART.h"

void _USART_SetMode(USART_t *_usart, USART_CMODE_t _usartMode){
	_usart->CTRLC = (_usart->CTRLC & (~USART_CMODE_gm)) | _usartMode;
}

void _USART_SetUsartFormat(USART_t *_usart,USART_CHSIZE_t _charSize,USART_PMODE_t _parityMode,bool _twoStopBits){
	_usart->CTRLC = (uint8_t) _charSize | _parityMode | (_twoStopBits ? USART_SBMODE_bm : 0);
}

void _USART_SetBaudRate(USART_t *_usart, unsigned long _baudRate, int _bScaleFactor){
	uint16_t bsel_val = BAUD_VALUE(_baudRate,_bScaleFactor);
	_usart->BAUDCTRLA = bsel_val;
	_usart->BAUDCTRLB = (_bScaleFactor<<USART_BSCALE_gp) | (bsel_val>>8);
}

void _USART_EnableDoubleSpeed(USART_t *_usart,bool _doubleSpeed){
	if(_doubleSpeed) _usart->CTRLB |= USART_CLK2X_bm ;
	else _usart->CTRLB &= ~USART_CLK2X_bm;
}
void _USART_EnableMultiprocessor(USART_t *_usart,bool _MultiprocessorMode){
	if(_MultiprocessorMode) _usart->CTRLB |= USART_MPCM_bp ;
	else _usart->CTRLB &= ~USART_MPCM_bp;
}


void _USART_EnableRxd(USART_t *_usart){
	_usart->CTRLB |= USART_RXEN_bm;
}
void _USART_DisableRxd(USART_t *_usart){
	_usart->CTRLB &= ~USART_RXEN_bm;
}
void _USART_EnableTxd(USART_t *_usart){
	_usart->CTRLB |= USART_TXEN_bm;
}
void _USART_DisableTxd(USART_t *_usart){
	_usart->CTRLB &= ~USART_TXEN_bm;
}
void _USART_ConfigureRxdInterruptLevel(USART_t *_usart, USART_RXCINTLVL_t _rxdIntLevel){
	((_usart)->CTRLA = ((_usart)->CTRLA & ~USART_RXCINTLVL_gm) | _rxdIntLevel);
}
void _USART_ConfigureTxdInterruptLevel(USART_t *_usart, USART_TXCINTLVL_t _txdIntLevel){
	(_usart)->CTRLA = ((_usart)->CTRLA & ~USART_TXCINTLVL_gm) | _txdIntLevel;
}
void _USART_ConfigureDreInterruptLevel(USART_t *_usart, USART_DREINTLVL_t _dreIntLevel){
	(_usart)->CTRLA = ((_usart)->CTRLA & ~USART_DREINTLVL_gm) | _dreIntLevel;
}
bool _USART_GetRxdFlag(USART_t *_usart){
	bool flag;
	if(_usart->STATUS & USART_RXCIF_bm) flag = 1 ;
	else flag = 0;
	return flag;
}
bool _USART_GetTxdFlag(USART_t *_usart){
	bool flag;
	if(_usart->STATUS & USART_TXCIF_bm) flag = 1 ;
	else flag = 0;
	return flag;
}
bool _USART_GetDreFlag(USART_t *_usart ){
	bool flag;
	if(_usart->STATUS & USART_DREIF_bm) flag = 1 ;
	else flag = 0;
	return flag;
}
void _USART_Send(USART_t *_usart, uint8_t _data){
	while(!(_usart->STATUS&USART_DREIF_bm));
	_usart->DATA = _data;
}
uint8_t _USART_Receive(USART_t *_usart){
	while(!(_usart->STATUS&USART_RXCIF_bm));
	//return usart_unit->DATA;
	return _usart->DATA;
}
void _USART_TransmitNineBits(USART_t *_usart, uint16_t _data){
	if(_data & 0x0100) {
		_usart->CTRLB |= USART_TXB8_bm;
		}else {
		_usart->CTRLB &= ~USART_TXB8_bm;
	}

	_usart->DATA = (_data & 0x00FF);
}
uint16_t _USART_ReceiveNineBits(USART_t *_usart){
	if(_usart->CTRLB & USART_RXB8_bm) {return(0x0100 | _usart->DATA);}
	else {return(_usart->DATA);}
}
uint8_t _USART_SPIMasterTranscieve(USART_t *_usart, uint8_t _data){
	while ((_usart->STATUS & USART_DREIF_bm) == 0);
	_usart->DATA = _data;
	while((_usart->STATUS & USART_TXCIF_bm) == 0);
	_usart->STATUS = USART_TXCIF_bm;
	return (_usart)->DATA;
}


void _USART_ConfigurePortPins(USART_t *_usart){

	PORT_t *usart_port;
	int usart_port_unit;

	#if defined(USARTC0)
	if(_usart == &USARTC0){usart_port = &PORTC;usart_port_unit = 0;}
	#endif

	#if defined(USARTC1)
	if(_usart == &USARTC1){usart_port = &PORTC;usart_port_unit = 1;}
	#endif

	#if defined(USARTD0)
	if(_usart == &USARTD0){usart_port = &PORTD;usart_port_unit = 0;}
	#endif

	#if defined(USARTD1)
	if(_usart == &USARTD1){usart_port = &PORTD;usart_port_unit = 1;}
	#endif

	#if defined(USARTE0)
	if(_usart == &USARTE0){usart_port = &PORTE;usart_port_unit = 0;}
	#endif

	#if defined(USARTE1)
	if(_usart == &USARTE1){usart_port = &PORTE;usart_port_unit = 1;}
	#endif

	#if defined(USARTF0)
	if(_usart == &USARTF0){usart_port = &PORTF;usart_port_unit = 0;}
	#endif

	#if defined(USARTF1)
	if(_usart == &USARTF1){usart_port = &PORTF;usart_port_unit = 1;}
	#endif

	asm("nop");
	if(usart_port_unit==0){
		if(!((usart_port->REMAP)&PORT_USART0_bm)){
			usart_port->DIRSET = (1<<3);//USARTx0 TXD pin output
			usart_port->DIRCLR = (1<<2);//USARTx0 RXD pin input
			if((_usart->CTRLC&USART_CMODE_gm) == USART_CMODE_MSPI_gc){
				usart_port->DIRSET = (1<<1);//USARTx0 TXD pin output
				usart_port->DIRSET = (1<<0);//USARTx0 TXD pin output
			}
		}

	}
	else{
		usart_port->DIRSET = (1<<7);//USARTx0 TXD pin output
		usart_port->DIRCLR = (1<<6);//USARTx0 RXD pin input
	}


}

#if defined(USARTC0)
ISR(USARTC0_RXC_vect){

}

ISR(USARTC0_TXC_vect){
	
}

ISR(USARTC0_DRE_vect){
	
}
#endif


#if defined(USARTC1)
ISR(USARTC1_RXC_vect){
	
}

ISR(USARTC1_TXC_vect){
	
}

ISR(USARTC1_DRE_vect){
	
}
#endif


#if defined(USARTD0)
ISR(USARTD0_RXC_vect){
	
}

ISR(USARTD0_TXC_vect){
	
}

ISR(USARTD0_DRE_vect){
	
}
#endif


#if defined(USARTD1)
ISR(USARTD1_RXC_vect){
	
}

ISR(USARTD1_TXC_vect){
	
}

ISR(USARTD1_DRE_vect){
	
}
#endif

#if defined(USARTE0)
ISR(USARTE0_RXC_vect){
	
}

ISR(USARTE0_TXC_vect){
	
}

ISR(USARTE0_DRE_vect){
	
}
#endif


#if defined(USARTE1)
ISR(USARTE1_RXC_vect){
	
}

ISR(USARTE1_TXC_vect){
	
}

ISR(USARTE1_DRE_vect){
	
}
#endif

