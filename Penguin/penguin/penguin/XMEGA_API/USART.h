/*
 * _USART.h
 *
 * Created: 08-May-16 8:43:57 PM
 *  Author: emon1
 */ 


#ifndef USART_H_
#define USART_H_


#include "api_common.h"

#define BAUD_VALUE(BAUDRATE,BSCALE) (BSCALE>=0)? (F_CPU/16/BAUDRATE/(2<<(BSCALE-1)) - 1):(F_CPU*(2<<(-BSCALE))/16/BAUDRATE/2 - 1)


void _USART_SetMode(USART_t *_usart, USART_CMODE_t _usartMode);
void _USART_SetUsartFormat(USART_t *_usart,USART_CHSIZE_t _charSize,USART_PMODE_t _parityMode,bool _twoStopBits);
void _USART_SetBaudRate(USART_t *_usart, unsigned long _baudRate, int _bScaleFactor);
void _USART_EnableDoubleSpeed(USART_t *_usart,bool _doubleSpeed);
void _USART_EnableMultiprocessor(USART_t *_usart,bool _MultiprocessorMode);
void _USART_EnableRxd(USART_t *_usart);
void _USART_DisableRxd(USART_t *_usart);
void _USART_EnableTxd(USART_t *_usart);
void _USART_DisableTxd(USART_t *_usart);
void _USART_ConfigureRxdInterruptLevel(USART_t *_usart, USART_RXCINTLVL_t _rxdIntLevel);
void _USART_ConfigureTxdInterruptLevel(USART_t *_usart, USART_TXCINTLVL_t _txdIntLevel);
void _USART_ConfigureDreInterruptLevel(USART_t *_usart, USART_DREINTLVL_t _dreIntLevel);
bool _USART_GetRxdFlag(USART_t *_usart);
bool _USART_GetTxdFlag(USART_t *_usart);
bool _USART_GetDreFlag(USART_t *_usart );
void _USART_Send(USART_t *_usart, uint8_t _data);
uint8_t _USART_Receive(USART_t *_usart);
void _USART_TransmitNineBits(USART_t *_usart, uint16_t _data);
uint16_t _USART_ReceiveNineBits(USART_t *_usart);
uint8_t _USART_SPIMasterTranscieve(USART_t *_usart, uint8_t _data);
void _USART_ConfigurePortPins(USART_t *_usart);

#endif /* USART_H_ */