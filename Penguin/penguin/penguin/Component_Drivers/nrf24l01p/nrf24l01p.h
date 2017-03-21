/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NRF24L01p.h
 * Author: emon1
 *
 * Created on January 29, 2017, 7:10 AM
 */

#ifndef NRF24L01P_H
#define NRF24L01P_H

#include "NRF24L01pDriver.h"
#include "NRF24L01pConfig.h"


    typedef enum {
           MODE_UNKNOWN = 0,/**< NRF24L01+ unknown mode */
           MODE_POWER_DOWN,/**< NRF24L01+ Power Down mode */
           MODE_STANDBY,/**< NRF24L01+ Standby mode */
           MODE_RX,/**< NRF24L01+ RX mode */
           MODE_TX,/**< NRF24L01+ TX mode */
    }RadioState_t;
    
    
    typedef struct {
        bool DataReadyInterruptEnabled;
        bool DataSentInterruptFlagEnabled;
        bool MaxRetryInterruptFlagEnabled;
        crc_t Crc;
        uint8_t AutoReTransmissionCount;
        uint8_t AutoReTransmitDelayX250us;
        uint8_t frequencyOffset;
        datarate_t datarate;
        RFpower_t RfPower;
        bool PllLock;
        bool ContWaveEnabled;
        bool FeatureDynamicPayloadEnabled;
        bool FeaturePayloadWithAckEnabled;
        bool FeatureDynamicPayloadWithNoAckEnabled;
    }RadioConfig_t;
    
    typedef struct {
        bool PipeEnabled;
        bool autoAckEnabled;
        uint64_t address;
        aw_t MaxWidth;
        bool dynamicPayloadEnabled;
    }RxPipeConfig_t;

    typedef struct{
        PipeAddr_t address;
        pipe_t pipe;
        bool UseAck;
        uint8_t *data;
        unsigned int length;
        uint8_t retransmitCount;
    }Payload_t;
    
    typedef enum{
        SUCCESS = 0,
        ERROR = -1,
                
    }ErrorStatus_t;
    
    
    RadioState_t RadioState;
    RadioConfig_t RadioConfig;
    RxPipeConfig_t RxPipeConfig[6];
    PipeAddr_t  TxPipeAddress;
    

    
    void ResetConfigValues(RadioConfig_t *_RadioConfig, RxPipeConfig_t *_RxPipeConfig);
    void RadioMode(RadioState_t mode);
    
    bool readable();
    bool writable();
    bool readableOnPipe(pipe_t pipe);
    
    ErrorStatus_t writePayload(Payload_t *payload);
    ErrorStatus_t writeAckPayload(Payload_t *payload);
    ErrorStatus_t readPayload(Payload_t *payload);
    
    ErrorStatus_t TransmitPayload(Payload_t *payload);
    ErrorStatus_t ReceivePayload(Payload_t *payload);
    


#endif /* NRF24L01P_H */

