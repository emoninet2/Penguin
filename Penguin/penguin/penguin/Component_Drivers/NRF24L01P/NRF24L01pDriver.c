/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NRF24L01pDriver.cpp
 * Author: emon1
 * 
 * Created on January 29, 2017, 7:56 AM
 */

#include "NRF24L01pDriver.h"



uint8_t read_register(uint8_t address){
    port_Pin_CSN(0);
    uint8_t temp = address&_NRF24L01P_REG_ADDRESS_MASK;
    port_SPI_Transcieve(&temp, 1);
    port_SPI_Transcieve(&temp, 1);
    port_Pin_CSN(1);
    return temp;
    
}
void read_register_buffer(uint8_t address, uint8_t *dataout, int len){
    port_Pin_CSN(0);
    uint8_t temp = address&_NRF24L01P_REG_ADDRESS_MASK;
    port_SPI_Transcieve(&temp, 1);
    port_SPI_Transcieve(dataout, len);
    port_Pin_CSN(1);
}
void write_register(uint8_t address, uint8_t datain){
    port_Pin_CSN(0);
    uint8_t temp = (( _NRF24L01P_SPI_CMD_WR_REG | (address&(_NRF24L01P_REG_ADDRESS_MASK))));
    port_SPI_Transcieve(&temp, 1);
    port_SPI_Transcieve(&datain, 1);
    port_Pin_CSN(1);
}
void write_register_buffer(uint8_t address, uint8_t *datain, int len){
    port_Pin_CSN(0);
    uint8_t temp = (( _NRF24L01P_SPI_CMD_WR_REG | (address&(_NRF24L01P_REG_ADDRESS_MASK))));
    port_SPI_Transcieve(&temp, 1);
    port_SPI_Transcieve(datain, len);
    port_Pin_CSN(1);
}
void read_rx_payload(uint8_t *dataout, int pay_len){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_RD_RX_PAYLOAD);
    port_SPI_Transcieve(&temp,1);
    port_SPI_Transcieve(dataout,pay_len);
    port_Pin_CSN(1);
}
void write_tx_payload(uint8_t *datain, int pay_len){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_WR_TX_PAYLOAD);
    port_SPI_Transcieve(&temp,1);
    port_SPI_Transcieve(datain,pay_len);
    port_Pin_CSN(1);
}
void flush_tx(){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_FLUSH_TX);
    port_SPI_Transcieve(&temp,1);
    port_Pin_CSN(1);  
}
void flush_rx(){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_FLUSH_RX);
    port_SPI_Transcieve(&temp,1);
    port_Pin_CSN(1);
}
void reuse_tx_payload(){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_REUSE_TX_PL);
    port_SPI_Transcieve(&temp,1);
    port_Pin_CSN(1);
}
int read_rx_payload_width(){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_R_RX_PL_WID);
    port_SPI_Transcieve(&temp,1);
    port_SPI_Transcieve(&temp,1);
    port_Pin_CSN(1);
    return temp;
}
void write_ack_payload(pipe_t pipe, uint8_t *datain, int pay_len){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_W_ACK_PAYLOAD | pipe);
    port_SPI_Transcieve(&temp,1);
    port_SPI_Transcieve(datain,pay_len);
    port_Pin_CSN(1);
}
void write_tx_payload_noack(uint8_t *datain, int pay_len){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_W_TX_PYLD_NO_ACK);
    port_SPI_Transcieve(&temp,1);
    port_SPI_Transcieve(datain,pay_len);
    port_Pin_CSN(1);
}
int get_status(){
    port_Pin_CSN(0);
    uint8_t temp = (_NRF24L01P_SPI_CMD_NOP );
    port_SPI_Transcieve(&temp,1);
    port_Pin_CSN(1);     
    return temp;
}


void power_up(){
    uint8_t temp = read_register(_NRF24L01P_REG_CONFIG);
    temp |= _NRF24L01P_CONFIG_PWR_UP;
    write_register(_NRF24L01P_REG_CONFIG,temp);
}
void power_down(){
    uint8_t temp = read_register(_NRF24L01P_REG_CONFIG);
    temp &= ~_NRF24L01P_CONFIG_PWR_UP;
    write_register(_NRF24L01P_REG_CONFIG,temp);
}
void rx_mode(){
    uint8_t temp = read_register(_NRF24L01P_REG_CONFIG);
    temp |= _NRF24L01P_CONFIG_PRIM_RX;
    write_register(_NRF24L01P_REG_CONFIG,temp);
}
void tx_mode(){
    uint8_t temp = read_register(_NRF24L01P_REG_CONFIG);
    temp &= ~_NRF24L01P_CONFIG_PRIM_RX;
    write_register(_NRF24L01P_REG_CONFIG,temp);
}
void set_CRC(crc_t opt){
    uint8_t temp = read_register(_NRF24L01P_REG_CONFIG);
    temp &= ~(_NRF24L01P_CONFIG_CRC_MASK);
    temp |= opt;
    write_register(_NRF24L01P_REG_CONFIG,temp);
}


void enable_dataReady_interrupt(bool sel){
    
}

void enable_dataSent_interrupt(bool sel){
    
}

void enable_maxRetry_interrupt(bool sel){
    
}


void enable_auto_ack(pipe_t pipe, bool sel){
    if (sel){
        uint8_t temp = read_register(_NRF24L01P_REG_EN_AA);
        _nrf24l01p_set_bit(temp,pipe);
        write_register(_NRF24L01P_REG_EN_AA,temp); 
    }else{
        uint8_t temp = read_register(_NRF24L01P_REG_EN_AA);
        _nrf24l01p_clr_bit(temp,pipe);
        write_register(_NRF24L01P_REG_EN_AA,temp); 
    }
  
}



void enable_rx_on_pipe(pipe_t pipe, bool sel){
    if(sel){
        uint8_t temp = read_register(_NRF24L01P_REG_EN_RXADDR);
        _nrf24l01p_set_bit(temp,pipe);
        write_register(_NRF24L01P_REG_EN_RXADDR,temp);
    }else{
        uint8_t temp = read_register(_NRF24L01P_REG_EN_RXADDR);
        _nrf24l01p_clr_bit(temp,pipe);
        write_register(_NRF24L01P_REG_EN_RXADDR,temp);
    }

}

void set_address_width(aw_t width){
    write_register(_NRF24L01P_REG_SETUP_AW,(uint8_t) width);
}
aw_t get_address_width(){
    return (aw_t) read_register(_NRF24L01P_REG_SETUP_AW);  
}

void set_auto_retransmission_count(uint8_t count){
    uint8_t temp = read_register(_NRF24L01P_REG_SETUP_RETR);
    temp &= ~ 0x0F;
    temp |= (count<<0);
    write_register(_NRF24L01P_REG_SETUP_RETR,temp);
}
uint8_t read_auto_retransmission_count(){
    uint8_t temp = read_register(_NRF24L01P_REG_SETUP_RETR);
    return temp & 0x0F; 
}

void set_auto_retransmission_delay(uint8_t times250us){
    uint8_t temp = read_register(_NRF24L01P_REG_SETUP_RETR);
    temp &= ~(0xF0);
    temp |= (times250us<<4);
    write_register(_NRF24L01P_REG_SETUP_RETR,temp);
}
uint8_t read_auto_retransmission_delay(){
    uint8_t temp = read_register(_NRF24L01P_REG_SETUP_RETR);
    return temp & 0xF0;
}

void set_frequency_offset(uint8_t offset){
    if( (offset >=0)  && ( offset <= 125)){
            write_register(_NRF24L01P_REG_RF_CH,offset);
    }
}
uint8_t get_frequency_offset(){
    return read_register(_NRF24L01P_REG_RF_CH);
}

void set_DataRate(datarate_t DataRate){
    uint8_t temp = read_register(_NRF24L01P_REG_RF_SETUP);
    temp &= ~_NRF24L01P_RF_SETUP_RF_DR_MASK;
    temp |= DataRate;
    write_register(_NRF24L01P_REG_RF_SETUP,temp);
}
datarate_t get_DataRate(){
    uint8_t temp = read_register(_NRF24L01P_REG_RF_SETUP);
    temp &= _NRF24L01P_RF_SETUP_RF_DR_MASK;
    return (datarate_t) temp;
}
void set_RF_Power(RFpower_t RFpower){
    uint8_t temp = read_register(_NRF24L01P_REG_RF_SETUP);
    temp &= ~_NRF24L01P_RF_SETUP_RF_PWR_MASK;
    temp |= RFpower;
    write_register(_NRF24L01P_REG_RF_SETUP,temp);
}
RFpower_t get_RF_Power(){
    uint8_t temp = read_register(_NRF24L01P_REG_RF_SETUP);
    temp &= _NRF24L01P_RF_SETUP_RF_PWR_MASK;
    return (RFpower_t) temp;
}
void enable_pll_lock(bool sel){
    
}

void enable_cont_wave(bool sel){
    if(sel){
        uint8_t temp = read_register(_NRF24L01P_REG_RF_SETUP);
        temp |= _NRF24L01P_RF_CONT_WAVE;
        write_register(_NRF24L01P_REG_RF_SETUP,temp);
    }
    else{
        uint8_t temp = read_register(_NRF24L01P_REG_RF_SETUP);
        temp &= ~_NRF24L01P_RF_CONT_WAVE;
        write_register(_NRF24L01P_REG_RF_SETUP,temp);
    }

}


bool get_tx_fifo_full_flag(){
    if(get_status()&_NRF24L01P_STATUS_TX_FULL) return 1;
    else return 0;
}
bool get_max_retry_flag(){
    if(get_status()&_NRF24L01P_STATUS_MAX_RT) return 1;
    else return 0;
}
void clear_max_retry_flag(){
    uint8_t temp = _NRF24L01P_STATUS_MAX_RT;
    write_register_buffer(_NRF24L01P_REG_STATUS,&temp,sizeof(temp));
}
bool get_data_sent_flag(){
    if(get_status()&_NRF24L01P_STATUS_TX_DS) return 1;
    else return 0;
}
void clear_data_sent_flag(){
    uint8_t temp = _NRF24L01P_STATUS_TX_DS;
    write_register_buffer(_NRF24L01P_REG_STATUS,&temp,sizeof(temp));
}
bool get_data_ready_flag(){
    if(get_status()&_NRF24L01P_STATUS_RX_DR) return 1;
    else return 0;
}
void clear_data_ready_flag(){
    uint8_t temp = _NRF24L01P_STATUS_RX_DR;
    write_register_buffer(_NRF24L01P_REG_STATUS,&temp,sizeof(temp));
}
pipe_t get_rx_payload_pipe(){
    return (pipe_t) ((get_status()&_NRF24L01P_STATUS_RX_P_NO)>>1);
}

uint8_t get_arc_count(){
    uint8_t temp = read_register(_NRF24L01P_REG_OBSERVE_TX);
    return ((temp&_NRF24L01P_OBSERVE_TX_ARC_CNT_MASK)>>_NRF24L01P_OBSERVE_TX_ARC_CNT_BP);
    //return (temp&0x0F)>>0;

}
uint8_t get_plos_count(){
    uint8_t temp;
    read_register_buffer(_NRF24L01P_REG_OBSERVE_TX,&temp,sizeof(temp));
    return ((temp&_NRF24L01P_OBSERVE_TX_PLOS_CNT_MASK)>>_NRF24L01P_OBSERVE_TX_PLOS_CNT_BP);
    //return (temp&0xF0)>>4; 
}
void clear_plos_count(){
    set_frequency_offset(get_frequency_offset());
}

bool get_rpd(){
    if(read_register(_NRF24L01P_REG_RPD)) return 1;
    else return 0;
}

void set_RX_pipe_address(pipe_t pipe,PipeAddr_t address){
    int max_pipe_addr_width = 0;
    if((pipe>=0) && (pipe<=1)   ){max_pipe_addr_width = 5;}
    else if ((pipe>=2) && (pipe<=5)   ){max_pipe_addr_width = 1;}
    uint8_t temp[5];
    int i;
    for(i=0;i<max_pipe_addr_width;i++){
            temp[i] = (address>>(8*i))&0xFF;
    }
    write_register_buffer(_NRF24L01P_REG_RX_ADDR_P0 + pipe,temp,max_pipe_addr_width);
}
PipeAddr_t get_RX_pipe_address(pipe_t pipe){
    int max_pipe_addr_width = 0;
    if((pipe>=0) && (pipe<=1)   ){max_pipe_addr_width = 5;}
    else if ((pipe>=2) && (pipe<=5)   ){max_pipe_addr_width = 1;}

    uint8_t temp[5];
    read_register_buffer(_NRF24L01P_REG_RX_ADDR_P0 + pipe,temp,max_pipe_addr_width);

    uint64_t temp_addr = 0;
    uint8_t *temp_addr_ptr = (uint8_t*) &temp_addr;
    int i;
    for(i=0;i<max_pipe_addr_width;i++){
            *(temp_addr_ptr+i)|= (temp[i]);
    }

    return temp_addr;   
}

void set_TX_pipe_address(PipeAddr_t address){
    uint8_t temp[5];
    int i;
    for( i=0;i<5;i++){
            temp[i] = (address>>(8*i))&0xFF;
    }
    write_register_buffer(_NRF24L01P_REG_TX_ADDR,temp,5);   
}
PipeAddr_t get_TX_pipe_address(){
    uint8_t temp[5];
    read_register_buffer(_NRF24L01P_REG_TX_ADDR,temp,5);

    uint64_t temp_addr = 0;
    uint8_t *temp_addr_ptr = (uint8_t*) &temp_addr;
    int i;
    for(i=0;i<5;i++){
            *(temp_addr_ptr+i)|= (temp[i]);
    }
    return temp_addr;   
}

uint8_t get_RX_pipe_width(pipe_t pipe){
    uint8_t temp;
    read_register_buffer((_NRF24L01P_REG_RX_PW_P0+pipe),&temp,sizeof(temp));
    return (temp&(0x3F));
}

bool get_fifo_flag_rx_empty(){
    if(read_register(_NRF24L01P_REG_FIFO_STATUS)&_NRF24L01P_FIFO_STATUS_RX_EMPTY ) return 1;
    else return 0;
}
bool get_fifo_flag_rx_full(){
    if(read_register(_NRF24L01P_REG_FIFO_STATUS)&_NRF24L01P_FIFO_STATUS_RX_FULL ) return 1;
    else return 0;   
}
bool get_fifo_flag_tx_empty(){
    if(read_register(_NRF24L01P_REG_FIFO_STATUS)&_NRF24L01P_FIFO_STATUS_TX_EMPTY ) return 1;
    else return 0;       
}
bool get_fifo_flag_tx_full(){
    if(read_register(_NRF24L01P_REG_FIFO_STATUS)&_NRF24L01P_FIFO_STATUS_TX_FULL ) return 1;
    else return 0;      
}
bool get_fifo_flag_tx_reuse(){
    if(read_register(_NRF24L01P_REG_FIFO_STATUS)&_NRF24L01P_FIFO_STATUS_RX_REUSE ) return 1;
    else return 0;      
}

void enable_dynamic_payload_pipe(pipe_t pipe, bool sel){
    if(sel){
          uint8_t temp = read_register(_NRF24L01P_REG_DYNPD);
        temp |= (1<<pipe);
        write_register(_NRF24L01P_REG_DYNPD,temp);   
    }else{
        uint8_t temp = read_register(_NRF24L01P_REG_DYNPD);
        temp &= ~(1<<pipe);
        write_register(_NRF24L01P_REG_DYNPD,temp);    
    }
   
}



void enable_dynamic_payload(bool sel){
    if(sel){
        uint8_t temp = read_register(_NRF24L01P_REG_FEATURE);
        temp |= _NRF24L01_FEATURE_EN_DPL;
        write_register(_NRF24L01P_REG_FEATURE,temp);     
    }
    else{
        uint8_t temp = read_register(_NRF24L01P_REG_FEATURE);
        temp &= ~_NRF24L01_FEATURE_EN_DPL;
        write_register(_NRF24L01P_REG_FEATURE,temp);
    }
   
}



void enable_payload_with_ack(bool sel){
    if(sel){
        uint8_t temp = read_register(_NRF24L01P_REG_FEATURE);
        temp |= _NRF24L01_FEATURE_EN_ACK_PAY;
        write_register(_NRF24L01P_REG_FEATURE,temp); 
    }
    else{
        uint8_t temp = read_register(_NRF24L01P_REG_FEATURE);
        temp &= ~_NRF24L01_FEATURE_EN_ACK_PAY;
        write_register(_NRF24L01P_REG_FEATURE,temp); 
    }
     
}



void enable_dynamic_payload_with_no_ack(bool sel){
    if(sel){
        uint8_t temp = read_register(_NRF24L01P_REG_FEATURE);
        temp |= _NRF24L01_FEATURE_EN_DYN_ACK;
        write_register(_NRF24L01P_REG_FEATURE,temp);        
    }
    else{
        uint8_t temp = read_register(_NRF24L01P_REG_FEATURE);
        temp &= ~_NRF24L01_FEATURE_EN_DYN_ACK;
        write_register(_NRF24L01P_REG_FEATURE,temp);  
    }
     
}
