/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NRF24L01p.cpp
 * Author: emon1
 * 
 * Created on January 29, 2017, 7:10 AM
 */

#include "NRF24L01p.h"




void ResetConfigValues(RadioConfig_t *_RadioConfig, RxPipeConfig_t *_RxPipeConfig){
    port_Initialize();
    //ResetConfigValues(_RadioConfig, _RxPipeConfig);
    
    
    port_Pin_CE(0);
    port_Pin_CSN(0);

    port_DelayMs(_NRF24L01P_TIMING_PowerOnReset_ms);

    RadioMode(MODE_POWER_DOWN);
    RadioMode(MODE_RX);

    clear_data_ready_flag();
    flush_rx();
    flush_tx();

    uint8_t status_rst_val = 0x70;//reset status
    write_register_buffer(_NRF24L01P_REG_STATUS, &status_rst_val,1);
    uint8_t config_rst_val = 0x0b;//reset config
    write_register_buffer(_NRF24L01P_REG_CONFIG, &config_rst_val,1);

    
    RadioConfig.DataReadyInterruptEnabled = _RadioConfig->DataReadyInterruptEnabled;
    RadioConfig.DataSentInterruptFlagEnabled = _RadioConfig->DataSentInterruptFlagEnabled;
    RadioConfig.MaxRetryInterruptFlagEnabled = _RadioConfig->MaxRetryInterruptFlagEnabled;
    RadioConfig.Crc = _RadioConfig->Crc;
    RadioConfig.AutoReTransmissionCount = _RadioConfig->AutoReTransmissionCount;
    RadioConfig.AutoReTransmitDelayX250us = _RadioConfig->AutoReTransmitDelayX250us;
    RadioConfig.frequencyOffset = _RadioConfig->frequencyOffset;
    RadioConfig.datarate = _RadioConfig->datarate;
    RadioConfig.RfPower = _RadioConfig->RfPower;
    RadioConfig.PllLock = _RadioConfig->PllLock;
    RadioConfig.ContWaveEnabled = _RadioConfig->ContWaveEnabled;
    RadioConfig.FeatureDynamicPayloadEnabled = _RadioConfig->FeatureDynamicPayloadEnabled;
    RadioConfig.FeaturePayloadWithAckEnabled = _RadioConfig->FeaturePayloadWithAckEnabled;
    RadioConfig.FeatureDynamicPayloadWithNoAckEnabled = _RadioConfig->FeatureDynamicPayloadWithNoAckEnabled;

	int i;
    for(i=0;i<6;i++){
        RxPipeConfig[i] = _RxPipeConfig[i];
    }

    
    enable_dynamic_payload(RadioConfig.FeatureDynamicPayloadEnabled);
    enable_payload_with_ack(RadioConfig.FeaturePayloadWithAckEnabled);
    enable_dynamic_payload_with_no_ack(RadioConfig.FeatureDynamicPayloadWithNoAckEnabled);
    set_auto_retransmission_count(RadioConfig.AutoReTransmissionCount);
    set_auto_retransmission_delay(RadioConfig.AutoReTransmitDelayX250us);        
    set_DataRate(RadioConfig.datarate);
    
    for(i=0;i<6;i++){
        enable_rx_on_pipe((pipe_t)i,RxPipeConfig[i].PipeEnabled );
        enable_auto_ack((pipe_t)i,RxPipeConfig[i].autoAckEnabled );
        enable_dynamic_payload_pipe((pipe_t)i,RxPipeConfig[i].dynamicPayloadEnabled);
        set_RX_pipe_address((pipe_t)i,RxPipeConfig[i].address);
    }
}

void RadioMode(RadioState_t mode){
    switch(mode){
        case MODE_POWER_DOWN: {
            power_down();
            port_Pin_CE(0);
            RadioState = MODE_POWER_DOWN;
            break;
        }
        case MODE_STANDBY: {
            if(RadioState == MODE_POWER_DOWN){
                    power_up();
                    port_DelayUs(_NRF24L01P_TIMING_Tpd2stby_us);
            }
            else{
                    port_Pin_CE(0);
            }
            RadioState = MODE_STANDBY;
            break;
        }
        case MODE_RX: {
            if(RadioState != MODE_RX){
                port_Pin_CE(0);
                rx_mode();
                port_Pin_CE(1);
                port_DelayUs(_NRF24L01P_TIMING_Tstby2a_us);
                RadioState = MODE_RX;
            }
            break;
        }
        case MODE_TX: {
            if(RadioState != MODE_TX){
                port_Pin_CE(0);
                tx_mode();
                port_Pin_CE(1);
                port_DelayUs(_NRF24L01P_TIMING_Tstby2a_us);
                RadioState = MODE_TX;
            }
            break;
        }		
    }
}


bool readable(){
    return (get_data_ready_flag() || !get_fifo_flag_rx_empty())&& (get_rx_payload_pipe() != 7)  ; 
}
bool writable(){
    return !get_fifo_flag_tx_full();
    
}
bool readableOnPipe(pipe_t pipe){
    
}


ErrorStatus_t writePayload(Payload_t *payload){
    set_TX_pipe_address(payload->address);
    if(payload->UseAck == 1){
        write_tx_payload(payload->data,payload->length);
    }else{
        if(RadioConfig.FeatureDynamicPayloadWithNoAckEnabled == 1){
            write_tx_payload_noack(payload->data,payload->length); 
        }
    }
}

ErrorStatus_t writeAckPayload(Payload_t *payload){
    write_ack_payload(payload->pipe, payload->data, payload->length);
}
ErrorStatus_t readPayload(Payload_t *payload){
    ErrorStatus_t error;
    payload->pipe = get_rx_payload_pipe();
    
    if(payload->pipe>=0 && payload->pipe<=5){
        if(RadioConfig.FeatureDynamicPayloadEnabled == 1){
            payload->length = read_rx_payload_width();
        }else{
            payload->length = get_RX_pipe_width(payload->pipe);
        }
        read_rx_payload(payload->data,payload->length);
        error = SUCCESS;
    }
    return error;
}


ErrorStatus_t TransmitPayload(Payload_t *payload){
    ErrorStatus_t error;
    if(TxPipeAddress != payload->address){
        set_TX_pipe_address(payload->address);
        TxPipeAddress = payload->address;
    }

    if(payload->UseAck){
        
        if(RxPipeConfig[PIPE_P0].autoAckEnabled == 0){
            enable_auto_ack(PIPE_P0, 1);
        }
        if(RxPipeConfig[PIPE_P0].PipeEnabled == 0){
            enable_rx_on_pipe(PIPE_P0, 1);
        }
        set_RX_pipe_address(PIPE_P0, payload->address);
        
        
        writePayload(payload);
        
        RadioState_t originalState = RadioState;
        RadioMode(MODE_STANDBY);
        

        if(writable()){
            clear_data_sent_flag();
            while(1){
                RadioMode(MODE_TX);   
                RadioMode(MODE_STANDBY);
                
                if(get_data_sent_flag()){
                    error = SUCCESS;
                    break;
                }
                if(get_max_retry_flag()){
                    clear_max_retry_flag();
                    if(get_plos_count()>=payload->retransmitCount){
                        set_frequency_offset(RadioConfig.frequencyOffset);
                        error = ERROR;
                        break;
                    }
                }
            }

        }
        
        RadioMode(originalState);
        
        set_RX_pipe_address(PIPE_P0, RxPipeConfig[PIPE_P0].address);
        if(RxPipeConfig[PIPE_P0].autoAckEnabled == 0){
            enable_auto_ack(PIPE_P0, 0);
        }
        if(RxPipeConfig[PIPE_P0].PipeEnabled == 0){
            enable_rx_on_pipe(PIPE_P0, 0);
        }
    }else{
        set_TX_pipe_address(payload->address);
        writePayload(payload);
        RadioState_t originalState = RadioState;
        if(writable()){
            clear_data_sent_flag();
            while(1){
                RadioMode(MODE_TX);   
                RadioMode(MODE_STANDBY);
                if(get_data_sent_flag()){
                    error = SUCCESS;
                break;
                }
            }
        }
        RadioMode(originalState);
    }
    return error;
}
ErrorStatus_t ReceivePayload(Payload_t *payload){
    ErrorStatus_t error;
    clear_data_ready_flag();
    error = readPayload(payload);
    return error;
}