/*
 * nrf24l01p.h
 *
 * Created: 08-May-16 2:52:12 PM
 *  Author: emon1
 */ 

#include "nrf24l01p.h"


void _nrf24l01p_ce_pin(bool state){
	arch_nrf24l01p_ce_pin(state);
	if(state) _nrf24l01p_delay_us(_NRF24L01P_TIMING_Thce_us);
	_nrf24l01p_ce_value = state;
}
void _nrf24l01p_csn_pin(bool state){

	arch_nrf24l01p_csn_pin(state);
	_nrf24l01p_csn_value = state;
}

void _nrf24l01p_init(){
	arch_nrf24l01p_initialize();
	
	_nrf24l01p_startup();
	_nrf24l01p_default_config();
}


void _nrf24l01p_reinit_loop(void){
	//printf("testing NRF status\r\n");
	//volatile ntfstatus = _nrf24l01p_get_status();
	//if(ntfstatus == 0){
		while(_nrf24l01p_get_status() == 0){
			asm("nop");
		}
		_nrf24l01p_init();
		
	//}
	asm("nop");
	

}



void _nrf24l01p_read_register(uint8_t address, uint8_t *data, int len){

	_nrf24l01p_csn_pin(0);
	uint8_t temp = (address&(_NRF24L01P_REG_ADDRESS_MASK));
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(data,len);
	arch_nrf24l01p_csn_pin(1);
}

void _nrf24l01p_write_register(uint8_t address, uint8_t *data, int len){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (( _NRF24L01P_SPI_CMD_WR_REG | (address&(_NRF24L01P_REG_ADDRESS_MASK))));
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(data,len);
	_nrf24l01p_csn_pin(1);
}



void _nrf24l01p_read_rx_payload(uint8_t *data, int pay_len){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_RD_RX_PAYLOAD);
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(data,pay_len);
	_nrf24l01p_csn_pin(1);
}
void _nrf24l01p_write_tx_payload(uint8_t *data, int pay_len){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_WR_TX_PAYLOAD);
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(data,pay_len);
	_nrf24l01p_csn_pin(1);
}

void _nrf24l01p_flush_tx(){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_FLUSH_TX);
	arch_spi_master_transcieve(&temp,1);
	_nrf24l01p_csn_pin(1);
}
void _nrf24l01p_flush_rx(){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_FLUSH_RX);
	arch_spi_master_transcieve(&temp,1);
	_nrf24l01p_csn_pin(1);
}
void _nrf24l01p_reuse_tx_payload(){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_REUSE_TX_PL);
	arch_spi_master_transcieve(&temp,1);
	_nrf24l01p_csn_pin(1);
}
int _nrf24l01p_read_rx_payload_width(){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_R_RX_PL_WID);
	uint8_t temp2 = 0xff;
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(&temp2,1);
	_nrf24l01p_csn_pin(1);
	return temp2;
}

void _nrf24l01p_write_ack_payload(_nrf24l01p_pipe_t pipe, uint8_t *data, int pay_len){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_W_ACK_PAYLOAD | pipe);
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(data,pay_len);
	_nrf24l01p_csn_pin(1);
}
void _nrf24l01p_write_tx_payload_noack(uint8_t *data, int pay_len){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_W_TX_PYLD_NO_ACK);
	arch_spi_master_transcieve(&temp,1);
	arch_spi_master_transcieve(data,pay_len);
	_nrf24l01p_csn_pin(1);
}

int _nrf24l01p_get_status(){
	_nrf24l01p_csn_pin(0);
	uint8_t temp = (_NRF24L01P_SPI_CMD_NOP );
	arch_spi_master_transcieve(&temp,1);
	_nrf24l01p_csn_pin(1);
	return temp;
}



void _nrf24l01p_power_up(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	temp |= _NRF24L01P_CONFIG_PWR_UP;
	_nrf24l01p_write_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));

}
void _nrf24l01p_power_down(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	temp &= ~_NRF24L01P_CONFIG_PWR_UP;
	_nrf24l01p_write_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));

}
void _nrf24l01p_rx_mode(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	temp |= _NRF24L01P_CONFIG_PRIM_RX;
	_nrf24l01p_write_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	mode = _NRF24L01P_MODE_RX;
}
void _nrf24l01p_tx_mode(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	temp &= ~_NRF24L01P_CONFIG_PRIM_RX;
	_nrf24l01p_write_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	mode = _NRF24L01P_MODE_TX;
}
void _nrf24l01p_set_CRC(_nrf24l01p_crc_t opt){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
	temp &= ~(_NRF24L01P_CONFIG_CRC_MASK);
	temp |= opt;
	_nrf24l01p_write_register(_NRF24L01P_REG_CONFIG,&temp,sizeof(temp));
}

void _nrf24l01p_enable_auto_ack(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_EN_AA,&temp,sizeof(temp));
	set_bit(temp,pipe);
	_nrf24l01p_write_register(_NRF24L01P_REG_EN_AA,&temp,sizeof(temp));
}
void _nrf24l01p_disable_auto_ack(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_EN_AA,&temp,sizeof(temp));
	clr_bit(temp,pipe);
	_nrf24l01p_write_register(_NRF24L01P_REG_EN_AA,&temp,sizeof(temp));
}
void _nrf24l01p_disable_auto_ack_all_pipes(){
	uint8_t temp = 0;
	_nrf24l01p_write_register(_NRF24L01P_REG_EN_AA,&temp,sizeof(temp));
}

void _nrf24l01p_enable_rx_on_pipe(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_EN_RXADDR,&temp,sizeof(temp));
	set_bit(temp,pipe);
	_nrf24l01p_write_register(_NRF24L01P_REG_EN_RXADDR,&temp,sizeof(temp));
}
void _nrf24l01p_disable_rx_on_pipe(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_EN_RXADDR,&temp,sizeof(temp));
	clr_bit(temp,pipe);
	_nrf24l01p_write_register(_NRF24L01P_REG_EN_RXADDR,&temp,sizeof(temp));
}


void _nrf24l01p_set_address_width(_nrf24l01p_aw_t width){
	uint8_t temp = width;
	_nrf24l01p_write_register(_NRF24L01P_REG_SETUP_AW,&temp,sizeof(temp));
}

_nrf24l01p_aw_t _nrf24l01p_get_address_width(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_SETUP_AW,&temp,sizeof(temp));
	return (_nrf24l01p_aw_t) temp;
}

void _nrf24l01p_set_auto_retransmission_count(uint8_t count){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_SETUP_RETR,&temp,sizeof(temp));
	temp &= ~ 0x0F;
	temp |= (count<<0);
	_nrf24l01p_write_register(_NRF24L01P_REG_SETUP_RETR,&temp,sizeof(temp));
}
uint8_t _nrf24l01p_read_auto_retransmission_count(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_SETUP_RETR,&temp,sizeof(temp));
	return temp & 0x0F;
}
void _nrf24l01p_set_auto_retransmission_delay(uint8_t times250us){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_SETUP_RETR,&temp,sizeof(temp));
	temp &= ~(0xF0);
	temp |= (times250us<<4);
	_nrf24l01p_write_register(_NRF24L01P_REG_SETUP_RETR,&temp,sizeof(temp));
}
uint8_t _nrf24l01p_read_auto_retransmission_delay(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_SETUP_RETR,&temp,sizeof(temp));
	return temp & 0xF0;
}



void _nrf24l01p_set_frequency_offset(uint8_t offset){
	if( (offset >=0)  && ( offset <= 125)){
		_nrf24l01p_write_register(_NRF24L01P_REG_RF_CH,&offset,sizeof(offset));
	}
}
uint8_t _nrf24l01p_get_frequency_offset(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_RF_CH,&temp,sizeof(temp));
	return temp;
}

void _nrf24l01p_set_DataRate(_nrf24l01p_datarate_t DataRate){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_RF_SETUP,&temp,sizeof(temp));
	temp &= ~_NRF24L01P_RF_SETUP_RF_DR_MASK;
	temp |= DataRate;
	_nrf24l01p_write_register(_NRF24L01P_REG_RF_SETUP,&temp,sizeof(temp));
	
}
_nrf24l01p_datarate_t _nrf24l01p_get_DataRate(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_RF_SETUP,&temp,sizeof(temp));
	temp &= _NRF24L01P_RF_SETUP_RF_DR_MASK;
	return (_nrf24l01p_datarate_t) temp;
}
void _nrf24l01p_set_RF_Power(_nrf24l01p_RFpower_t RFpower){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_RF_SETUP,&temp,sizeof(temp));
	temp &= ~_NRF24L01P_RF_SETUP_RF_PWR_MASK;
	temp |= RFpower;
	_nrf24l01p_write_register(_NRF24L01P_REG_RF_SETUP,&temp,sizeof(temp));
}
_nrf24l01p_RFpower_t _nrf24l01p_get_RF_Power(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_RF_SETUP,&temp,sizeof(temp));
	temp &= _NRF24L01P_RF_SETUP_RF_PWR_MASK;
	return (_nrf24l01p_RFpower_t) temp;
}


bool _nrf24l01p_get_tx_fifo_full_flag(){
	bool flag;
	//(_nrf24l01p_get_status()&_NRF24L01P_STATUS_TX_FULL)?flag = 1 : flag = 0;
	
	if(_nrf24l01p_get_status()&_NRF24L01P_STATUS_TX_FULL) flag = 1;
	else flag = 0;

	return flag;
}
bool _nrf24l01p_get_max_retry_flag(){
	bool flag;
	//(_nrf24l01p_get_status()&_NRF24L01P_STATUS_MAX_RT)?flag = 1 : flag = 0;
	if(_nrf24l01p_get_status()&_NRF24L01P_STATUS_MAX_RT) flag = 1;
	else flag  = 0;
	return flag;
}


void _nrf24l01p_clear_max_retry_flag(){
	uint8_t temp = _nrf24l01p_get_status();
	temp |= _NRF24L01P_STATUS_MAX_RT;
	_nrf24l01p_write_register(_NRF24L01P_REG_STATUS,&temp,sizeof(temp));
}

bool _nrf24l01p_get_data_sent_flag(){
	bool flag;
	//(_nrf24l01p_get_status()&_NRF24L01P_STATUS_TX_DS)?flag = 1 : flag = 0;
	if(_nrf24l01p_get_status()&_NRF24L01P_STATUS_TX_DS) flag = 1;
	else flag  = 0;
	return flag;
}

void _nrf24l01p_clear_data_sent_flag(){
	uint8_t temp = _nrf24l01p_get_status();
	temp |= _NRF24L01P_STATUS_TX_DS;
	_nrf24l01p_write_register(_NRF24L01P_REG_STATUS,&temp,sizeof(temp));
}

bool _nrf24l01p_get_data_ready_flag(){
	bool flag;
	if(_nrf24l01p_get_status()&_NRF24L01P_STATUS_RX_DR)flag = 1  ;
	else flag = 0;
	return flag;
}

void _nrf24l01p_clear_data_ready_flag(){
	uint8_t temp = _nrf24l01p_get_status();
	temp |= _NRF24L01P_STATUS_RX_DR;
	_nrf24l01p_write_register(_NRF24L01P_REG_STATUS,&temp,sizeof(temp));
}

_nrf24l01p_pipe_t _nrf24l01p_get_rx_payload_pipe(){
	_nrf24l01p_pipe_t pipe = (_nrf24l01p_pipe_t) ((_nrf24l01p_get_status()&_NRF24L01P_STATUS_RX_P_NO)>>1);
	return pipe;
}

uint8_t _nrf24l01p_get_arc_count(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_OBSERVE_TX,&temp,sizeof(temp));
	return ((temp>>_NRF24L01P_OBSERVE_TX_ARC_CNT_BP)&_NRF24L01P_OBSERVE_TX_ARC_CNT_MASK);

}
uint8_t _nrf24l01p_get_plos_count(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_OBSERVE_TX,&temp,sizeof(temp));
	return ((temp>>_NRF24L01P_OBSERVE_TX_PLOS_CNT_BP)&_NRF24L01P_OBSERVE_TX_PLOS_CNT_MASK);
}

bool _nrf24l01p_get_rpd(){
	uint8_t temp;
	bool flag;
	_nrf24l01p_read_register(_NRF24L01P_REG_RPD,&temp,sizeof(temp));
	if(temp!=0) flag = 1  ;
	else flag = 0;
	return flag;
}

void _nrf24l01p_set_RX_pipe_address(_nrf24l01p_pipe_t pipe,uint64_t address){
 	int max_pipe_addr_width = 0;

	if((pipe>=0) && (pipe<=1)   )
	{
		max_pipe_addr_width = 5;
	}
	else if ((pipe>=2) && (pipe<=5)   ){
		max_pipe_addr_width = 1;
	}
	
	
	
	uint8_t temp[5];
	int i;
	for(i=0;i<max_pipe_addr_width;i++){
		temp[i] = (address>>(8*i))&0xFF;
	}
	_nrf24l01p_write_register(_NRF24L01P_REG_RX_ADDR_P0 + pipe,temp,max_pipe_addr_width);
	
}
uint64_t _nrf24l01p_get_RX_pipe_address(_nrf24l01p_pipe_t pipe){
	
 	int max_pipe_addr_width = 0;

	if((pipe>=0) && (pipe<=1)   )
	{
		max_pipe_addr_width = 5;
	}
	else if ((pipe>=2) && (pipe<=5)   ){
		max_pipe_addr_width = 1;
	}
	
	uint8_t temp[5];
	_nrf24l01p_read_register(_NRF24L01P_REG_RX_ADDR_P0 + pipe,temp,max_pipe_addr_width);
	
	uint64_t temp_addr = 0;
	uint8_t *temp_addr_ptr = (uint8_t*) &temp_addr;
	int i;
	for(i=0;i<max_pipe_addr_width;i++){
		*(temp_addr_ptr+i)|= (temp[i]);
	}
	
	return temp_addr;	
}

void _nrf24l01p_set_TX_pipe_address(uint64_t address){
	uint8_t temp[5];
	int i;
	for( i=0;i<5;i++){
		temp[i] = (address>>(8*i))&0xFF;
	}
	_nrf24l01p_write_register(_NRF24L01P_REG_TX_ADDR,temp,5);
}

uint64_t _nrf24l01p_get_TX_pipe_address(){
	uint8_t temp[5];
	_nrf24l01p_read_register(_NRF24L01P_REG_TX_ADDR,temp,5);
	
	uint64_t temp_addr = 0;
	uint8_t *temp_addr_ptr = (uint8_t*) &temp_addr;
	int i;
	for(i=0;i<5;i++){
		*(temp_addr_ptr+i)|= (temp[i]);
	}

	return temp_addr;
	
}
uint8_t _nrf24l01p_get_RX_pipe_width(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register((_NRF24L01P_REG_RX_PW_P0+pipe),&temp,sizeof(temp));
	return (temp&(0x3F));
}
bool _nrf24l01p_get_fifo_flag_rx_empty(){
	uint8_t temp;
	bool flag;
	_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&temp,sizeof(temp));
	if(temp&_NRF24L01P_FIFO_STATUS_RX_EMPTY) flag = 1 ;
	else flag = 0;
	return flag;
	
}
bool _nrf24l01p_get_fifo_flag_rx_full(){
	uint8_t temp;
	bool flag;
	_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&temp,sizeof(temp));
	if(temp&_NRF24L01P_FIFO_STATUS_RX_FULL) flag = 1 ;
	else flag = 0;
	return flag;
}
bool _nrf24l01p_get_fifo_flag_tx_empty(){
	uint8_t temp;
	bool flag;
	_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&temp,sizeof(temp));
	if(temp&_NRF24L01P_FIFO_STATUS_TX_EMPTY) flag = 1 ;
	else flag = 0;
	return flag;
}
bool _nrf24l01p_get_fifo_flag_tx_full(){
	uint8_t temp;
	bool flag;
	_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&temp,sizeof(temp));
	if(temp&_NRF24L01P_FIFO_STATUS_TX_FULL) flag = 1 ;
	else flag = 0;
	return flag;
}
bool _nrf24l01p_get_fifo_flag_tx_reuse(){
	uint8_t temp;
	bool flag;
	_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&temp,sizeof(temp));
	if(temp&_NRF24L01P_FIFO_STATUS_RX_REUSE) flag = 1 ;
	else flag = 0;
	return flag;
}

void _nrf24l01p_enable_dynamic_payload_pipe(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_DYNPD,&temp,sizeof(temp));
	temp |= (1<<pipe);
	_nrf24l01p_write_register(_NRF24L01P_REG_DYNPD,&temp,sizeof(temp));
}
void _nrf24l01p_disable_dynamic_payload_pipe(_nrf24l01p_pipe_t pipe){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_DYNPD,&temp,sizeof(temp));
	temp &= ~(1<<pipe);
	_nrf24l01p_write_register(_NRF24L01P_REG_DYNPD,&temp,sizeof(temp));
}
void _nrf24l01p_disable_dynamic_payload_all_pipe(){
	uint8_t temp = 0x00;
	_nrf24l01p_write_register(_NRF24L01P_REG_DYNPD,&temp,sizeof(temp));	
}
void _nrf24l01p_enable_dynamic_payload(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
	temp |= _NRF24L01_FEATURE_EN_DPL;
	_nrf24l01p_write_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
}
void _nrf24l01p_disable_dynamic_payload(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
	temp &= ~_NRF24L01_FEATURE_EN_DPL;
	_nrf24l01p_write_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
}
void _nrf24l01p_enable_payload_with_ack(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
	temp |= _NRF24L01_FEATURE_EN_ACK_PAY;
	_nrf24l01p_write_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
}
void _nrf24l01p_disable_payload_with_ack(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
	temp &= ~_NRF24L01_FEATURE_EN_ACK_PAY;
	_nrf24l01p_write_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
}
void _nrf24l01p_enable_dynamic_payload_with_ack(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
	temp |= _NRF24L01_FEATURE_EN_DYN_ACK;
	_nrf24l01p_write_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
}
void _nrf24l01p_disable_dynamic_payload_with_ack(){
	uint8_t temp;
	_nrf24l01p_read_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
	temp &= ~_NRF24L01_FEATURE_EN_DYN_ACK;
	_nrf24l01p_write_register(_NRF24L01P_REG_FEATURE,&temp,sizeof(temp));
}

//////////////////////////////////////////////////////////////////////////


void _nrf24l01p_print_info(){

}


//////////////////////////////////////////////////////////////////////////


int _nrf24l01p_startup(){
	_nrf24l01p_ce_pin(0);
	_nrf24l01p_csn_pin(0);
	
	_nrf24l01p_delay_ms(_NRF24L01P_TIMING_PowerOnReset_ms);
	
	_nrf24l01p_stateMode(_NRF24L01P_MODE_POWER_DOWN);
	_nrf24l01p_stateMode(_NRF24L01P_MODE_RX);
	
	_nrf24l01p_flush_rx();
	_nrf24l01p_flush_tx();
	
	uint8_t status_rst_val = 0x0e;//reset status
	_nrf24l01p_write_register(_NRF24L01P_REG_STATUS, &status_rst_val,1);
	uint8_t config_rst_val = 0x0b;//reset config
	_nrf24l01p_write_register(_NRF24L01P_REG_CONFIG, &config_rst_val,1);
	
	return 0;
}

int _nrf24l01p_default_config(){
	
	_nrf24l01p_disable_auto_ack_all_pipes();
	_nrf24l01p_disable_dynamic_payload_all_pipe();/////////ALSO CREEATE FOR DISABLE AUTO ACK FOR ALL PIPE
	//_nrf24l01p_startup();
	
	
	_nrf24l01p_enable_dynamic_payload();
	_nrf24l01p_enable_payload_with_ack();

	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P0);
	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P1);
	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P2);
	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P3);
	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P4);
	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P5);

	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P0);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P1);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P2);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P3);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P4);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P5);

	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P0);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P1);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P2);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P3);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P4);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P5);

	_nrf24l01p_set_auto_retransmission_count(15);
	_nrf24l01p_set_auto_retransmission_delay(15);
	_nrf24l01p_set_DataRate(_NRF24L01P_RF_SETUP_RF_DR_250KBPS);
	
	return 0;
}

int _nrf24l01p_stateMode(nRF24L01P_Mode_Type mode){
	switch(mode){
		case _NRF24L01P_MODE_POWER_DOWN: {
			_nrf24l01p_power_down();
			_nrf24l01p_ce_pin(0);
			_nrf24l01p_mode = _NRF24L01P_MODE_POWER_DOWN;
			break;
		}
		case _NRF24L01P_MODE_STANDBY: {
			if(_nrf24l01p_mode == _NRF24L01P_MODE_POWER_DOWN){
				_nrf24l01p_power_up();
				_nrf24l01p_delay_us(_NRF24L01P_TIMING_Tpd2stby_us);
			}
			else{
				_nrf24l01p_ce_pin(0);
			}
			_nrf24l01p_mode = _NRF24L01P_MODE_STANDBY;
			break;
		}
		case _NRF24L01P_MODE_RX: {
			_nrf24l01p_rx_mode();
			_nrf24l01p_ce_pin(1);
			_nrf24l01p_delay_us(_NRF24L01P_TIMING_Tstby2a_us);
			_nrf24l01p_mode = _NRF24L01P_MODE_RX;
			break;
		}
		case _NRF24L01P_MODE_TX: {
			_nrf24l01p_tx_mode();
			_nrf24l01p_ce_pin(1);
			_nrf24l01p_delay_us(_NRF24L01P_TIMING_Tstby2a_us);
			_nrf24l01p_mode = _NRF24L01P_MODE_TX;
			break;
		}		
	}
	
	return 0;
}

volatile uint8_t fiffooo = 0;
volatile uint8_t stattoo = 0;



int _nrf24l01p_PTX_Handle(){

	_nrf24l01p_PTX_noBlocking_SoftRetryCnt = 0;
	_nrf24l01p_PTX_noBlockingExitFlag = 0;
	int error_status = 0;
	if(_nrf24l01p_writable() && !_nrf24l01p_readable() ){

		nRF24L01P_Mode_Type originalMode = _nrf24l01p_mode; //backup mode
		
		_nrf24l01p_stateMode(_NRF24L01P_MODE_STANDBY);
		
		_nrf24l01p_clear_max_retry_flag();


		//send all data in payload. comment the line to send single packet at a time
		while(!_nrf24l01p_get_fifo_flag_tx_empty() ){//while TX FIFO has data
			_nrf24l01p_clear_data_sent_flag();  //clear data sent flag before the next packet is sent
			
			while(!_nrf24l01p_get_data_sent_flag()){ //wait until flag set when data sent
				_nrf24l01p_stateMode(_NRF24L01P_MODE_TX);
				_nrf24l01p_stateMode(_NRF24L01P_MODE_STANDBY);
				
				//if max retry is reached when Shockburst is enabled, 
				if(_nrf24l01p_get_max_retry_flag()){
					//_nrf24l01p_reuse_tx_payload();//////////////////??????

					if(_nrf24l01p_PTX_noBlocking_SoftRetryCnt++ > _NRF24L01P_PTX_noBlocking_MaxSoftRetry){
						_nrf24l01p_PTX_noBlockingExitFlag = 1;
						break;
					}
				}
			}
			if(_nrf24l01p_PTX_noBlockingExitFlag){
				_nrf24l01p_reuse_tx_payload();//////////////////??????
				break;
			}
			asm("nop");
		}
		
		_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&fiffooo,sizeof(fiffooo));
		_nrf24l01p_read_register(_NRF24L01P_REG_STATUS,&stattoo,sizeof(stattoo));
		if ( originalMode == _NRF24L01P_MODE_RX ) _nrf24l01p_stateMode(_NRF24L01P_MODE_RX);
		else if( originalMode == _NRF24L01P_MODE_STANDBY ) _nrf24l01p_stateMode(_NRF24L01P_MODE_STANDBY);
		
		
	}
	return error_status;
}
int _nrf24l01p_PRX_Handle(){//UNDER CONSTRUCTION
	int error_status = 0;
	nRF24L01P_Mode_Type originalMode =_nrf24l01p_mode; //backup mode
	if(_nrf24l01p_mode!=_NRF24L01P_MODE_RX){
		_nrf24l01p_stateMode(_NRF24L01P_MODE_STANDBY);
		_nrf24l01p_stateMode(_NRF24L01P_MODE_RX);
	}
	
	return error_status;
}

bool _nrf24l01p_readable(){
	bool HasData = 0;
	HasData = !_nrf24l01p_get_fifo_flag_rx_empty();
	return HasData;
}

bool _nrf24l01p_writable(){
	bool HasData = 0;
	HasData = !_nrf24l01p_get_fifo_flag_tx_empty();
	return HasData;
	
}


int _nrf24l01p_send(uint8_t *data, int datalen){

	if ( datalen <= 0 ) return 0;
	if ( datalen > _NRF24L01P_TX_FIFO_SIZE ) datalen = _NRF24L01P_TX_FIFO_SIZE;

	if(_nrf24l01p_get_tx_fifo_full_flag()) return -1;
	//while(_nrf24l01p_get_tx_fifo_full_flag());

	_nrf24l01p_write_tx_payload(data,datalen);
	return 0;
}


bool _nrf24l01p_readableOnPipe(_nrf24l01p_pipe_t pipe){
	bool flag = 0;
	if((pipe >=0)   && (pipe <=5)){
		int status = _nrf24l01p_get_status();
		if(   (status&_NRF24L01P_STATUS_RX_DR)  && ((status&_NRF24L01P_STATUS_RX_P_NO)>>1)==pipe){
			flag = 1;
		}
		else{
			flag = 0;
		}
	}
	return flag;
}


volatile int mystat;


int _nrf24l01p_send_to_address(uint64_t address, uint8_t *data, int datalen){
	//_nrf24l01p_disable_payload_with_ack();
	_nrf24l01p_set_TX_pipe_address(address);
	return _nrf24l01p_send(data,datalen);
	
}
int _nrf24l01p_send_to_address_ack(uint64_t address, uint8_t *data, int datalen){
	//_nrf24l01p_enable_payload_with_ack();
	_nrf24l01p_set_TX_pipe_address(address);
	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P0, address);
	return _nrf24l01p_send(data,datalen);
}




int _nrf24l01p_read(_nrf24l01p_pipe_t pipe, uint8_t *data, int datalen){

	int rxPayloadWidth;
	
	if ( ( pipe < 0 ) || ( pipe > 5 ) ) {
		return -1;
	}
	
	if (_nrf24l01p_readableOnPipe(pipe) ) {
		asm("nop");
		rxPayloadWidth = _NRF24L01P_TX_FIFO_SIZE;
		
		if ( ( rxPayloadWidth < 0 ) || ( rxPayloadWidth > _NRF24L01P_RX_FIFO_SIZE ) ) {
			_nrf24l01p_flush_rx();
		}
		else{
			_nrf24l01p_read_rx_payload(data,rxPayloadWidth);
			
			if(_nrf24l01p_get_fifo_flag_rx_empty()) {
				_nrf24l01p_clear_data_ready_flag();
			}
		}

		return rxPayloadWidth;
	}

	else {//if pipe not readable
		return 0;
	}
	return 0;	
}

int _nrf24l01p_read_dyn_pld(_nrf24l01p_pipe_t pipe, uint8_t *data){
	
	int rxPayloadWidth;
	
	if ( ( pipe < 0 ) || ( pipe > 5 ) ) {
		return -1;
	}
	
	if (_nrf24l01p_readableOnPipe(pipe) ) {
		asm("nop");
		rxPayloadWidth = _nrf24l01p_read_rx_payload_width();
	
		if ( ( rxPayloadWidth < 0 ) || ( rxPayloadWidth > _NRF24L01P_RX_FIFO_SIZE ) ) {
			_nrf24l01p_flush_rx();
		}
		else{
			_nrf24l01p_read_rx_payload(data,rxPayloadWidth);
					
			if(_nrf24l01p_get_fifo_flag_rx_empty()) {
				_nrf24l01p_clear_data_ready_flag();
			}
		}

		return rxPayloadWidth;
	}
	else {//if pipe not readable
		return 0;
	}
	return 0;
}


void _nrf24l01p_write_ack(_nrf24l01p_pipe_t pipe, uint8_t *data, int datalen){

	if ( datalen <= 0 ) return;
	if ( datalen > _NRF24L01P_TX_FIFO_SIZE ) datalen = _NRF24L01P_TX_FIFO_SIZE;

	_nrf24l01p_write_ack_payload(pipe,data,datalen);
	
}