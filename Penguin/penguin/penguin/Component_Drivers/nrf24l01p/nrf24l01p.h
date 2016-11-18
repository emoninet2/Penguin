/*
 * nrf24l01p.h
 *
 * Created: 08-May-16 1:42:35 PM
 *  Author: emon1
 */ 


#ifndef NRF24L01P_H_
#define NRF24L01P_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/** 
 * @brief crap in brief.
 * this is a brief continued. 
 * 
 * details starts here
 * This is contiinued detailed shit
 * @param  IRQn  interrupt number that specifies the interrupt
 * @return none.
 * Enable 
 * @note the specified interrupt in the NVIC Interrupt Controller.
 * Other settings of the interrupt such as priority are not affected.
 * @see _nrf24l01p_print_info();
 */



#include "nrf24l01p_arch_driver.h"

#define _NRF24L01P_PTX_noBlocking_MaxSoftRetry  10 //use 0 for blocking


#define _NRF24L01P_TX_FIFO_COUNT   3
#define _NRF24L01P_RX_FIFO_COUNT   3

#define _NRF24L01P_TX_FIFO_SIZE   32
#define _NRF24L01P_RX_FIFO_SIZE   32

#define _NRF24L01P_SPI_MAX_DATA_RATE     10000000



#define _NRF24L01P_EN_AA_NONE            0
#define _NRF24L01P_EN_RXADDR_NONE        0
#define _NRF24L01P_SETUP_AW_AW_MASK      (0x3<<0)

#define _NRF24L01P_MIN_RF_FREQUENCY    2400
#define _NRF24L01P_MAX_RF_FREQUENCY    2525
#define _NRF24L01P_DUMMYBYTE	0x65


/** @name NRF24L01+ commands
 *  These are the commands 
 */
/**@{*/ 
#define _NRF24L01P_SPI_CMD_RD_REG            0x00
#define _NRF24L01P_SPI_CMD_WR_REG            0x20
#define _NRF24L01P_SPI_CMD_RD_RX_PAYLOAD     0x61
#define _NRF24L01P_SPI_CMD_WR_TX_PAYLOAD     0xa0
#define _NRF24L01P_SPI_CMD_FLUSH_TX          0xe1
#define _NRF24L01P_SPI_CMD_FLUSH_RX          0xe2
#define _NRF24L01P_SPI_CMD_REUSE_TX_PL       0xe3
#define _NRF24L01P_SPI_CMD_R_RX_PL_WID       0x60
#define _NRF24L01P_SPI_CMD_W_ACK_PAYLOAD     0xa8
#define _NRF24L01P_SPI_CMD_W_TX_PYLD_NO_ACK  0xb0
#define _NRF24L01P_SPI_CMD_NOP               0xff
/**@}*/ 

/** @name NRF24L01+ register address
 *  These are the registers 
 */
/**@{*/ 
#define _NRF24L01P_REG_CONFIG                0x00
#define _NRF24L01P_REG_EN_AA                 0x01
#define _NRF24L01P_REG_EN_RXADDR             0x02
#define _NRF24L01P_REG_SETUP_AW              0x03
#define _NRF24L01P_REG_SETUP_RETR            0x04
#define _NRF24L01P_REG_RF_CH                 0x05
#define _NRF24L01P_REG_RF_SETUP              0x06
#define _NRF24L01P_REG_STATUS                0x07
#define _NRF24L01P_REG_OBSERVE_TX            0x08
#define _NRF24L01P_REG_RPD                   0x09
#define _NRF24L01P_REG_RX_ADDR_P0            0x0a
#define _NRF24L01P_REG_RX_ADDR_P1            0x0b
#define _NRF24L01P_REG_RX_ADDR_P2            0x0c
#define _NRF24L01P_REG_RX_ADDR_P3            0x0d
#define _NRF24L01P_REG_RX_ADDR_P4            0x0e
#define _NRF24L01P_REG_RX_ADDR_P5            0x0f
#define _NRF24L01P_REG_TX_ADDR               0x10
#define _NRF24L01P_REG_RX_PW_P0              0x11
#define _NRF24L01P_REG_RX_PW_P1              0x12
#define _NRF24L01P_REG_RX_PW_P2              0x13
#define _NRF24L01P_REG_RX_PW_P3              0x14
#define _NRF24L01P_REG_RX_PW_P4              0x15
#define _NRF24L01P_REG_RX_PW_P5              0x16
#define _NRF24L01P_REG_FIFO_STATUS           0x17
#define _NRF24L01P_REG_DYNPD                 0x1c
#define _NRF24L01P_REG_FEATURE               0x1d
#define _NRF24L01P_REG_ADDRESS_MASK          0x1f
/**@}*/ 



/** @name NRF24L01+ config address
 *  These are the congig registers 
 */
/**@{*/ 
#define _NRF24L01P_CONFIG_PRIM_RX        (1<<0)
#define _NRF24L01P_CONFIG_PWR_UP         (1<<1)
#define _NRF24L01P_CONFIG_CRC0           (1<<2)
#define _NRF24L01P_CONFIG_EN_CRC         (1<<3)
#define _NRF24L01P_CONFIG_MASK_MAX_RT    (1<<4)
#define _NRF24L01P_CONFIG_MASK_TX_DS     (1<<5)
#define _NRF24L01P_CONFIG_MASK_RX_DR     (1<<6)
#define _NRF24L01P_CONFIG_CRC_MASK       (_NRF24L01P_CONFIG_EN_CRC|_NRF24L01P_CONFIG_CRC0)
/**@}*/ 


/** @name NRF24L01+ setup register
 *  These are bits of the setup register
 */
/**@{*/ 
#define _NRF24L01P_RF_SETUP_RF_PWR_MASK          (0x3<<1)
#define _NRF24L01P_RF_SETUP_RF_DR_HIGH_BIT       (1 << 3)
#define _NRF24L01P_RF_SETUP_RF_DR_LOW_BIT        (1 << 5)
#define _NRF24L01P_RF_SETUP_RF_DR_MASK           (_NRF24L01P_RF_SETUP_RF_DR_LOW_BIT|_NRF24L01P_RF_SETUP_RF_DR_HIGH_BIT)
/**@}*/ 




/** @name NRF24L01+ status register
 *  These are bits of the status register
 */
/**@{*/ 
#define _NRF24L01P_STATUS_TX_FULL        (1<<0)
#define _NRF24L01P_STATUS_RX_P_NO        (0x7<<1)
#define _NRF24L01P_STATUS_MAX_RT         (1<<4)
#define _NRF24L01P_STATUS_TX_DS          (1<<5)
#define _NRF24L01P_STATUS_RX_DR          (1<<6)
/**@}*/ 

/** @name NRF24L01+ observe register
 *  These are bits of the observe register
 */
/**@{*/ 
#define _NRF24L01P_OBSERVE_TX_ARC_CNT_BP			0
#define _NRF24L01P_OBSERVE_TX_ARC_CNT_MASK			0x0F
#define _NRF24L01P_OBSERVE_TX_PLOS_CNT_BP			4
#define _NRF24L01P_OBSERVE_TX_PLOS_CNT_MASK			0xF0
/**@}*/ 


/** @name NRF24L01+ fifo status register
 *  These are bits of the fifo status register
 */
/**@{*/ 
#define _NRF24L01P_FIFO_STATUS_RX_EMPTY			(1<<0)
#define _NRF24L01P_FIFO_STATUS_RX_FULL			(1<<1)
#define _NRF24L01P_FIFO_STATUS_TX_EMPTY			(1<<4)
#define _NRF24L01P_FIFO_STATUS_TX_FULL			(1<<5)
#define _NRF24L01P_FIFO_STATUS_RX_REUSE			(1<<6)
/**@}*/ 


/** @name NRF24L01+ feature register
 *  These are bits of the feature register
 */
/**@{*/ 
#define _NRF24L01_FEATURE_EN_DPL			(1<<2)
#define _NRF24L01_FEATURE_EN_ACK_PAY		(1<<1)
#define _NRF24L01_FEATURE_EN_DYN_ACK		(1<<0)
/**@}*/ 



#define _NRF24L01P_RX_PW_Px_MASK         0x3F


/** @name NRF24L01+ config register
 *  These are bits of the config register
 */
/**@{*/ 
#define _NRF24L01P_TIMING_PowerOnReset_ms    100   // 100mS
#define _NRF24L01P_TIMING_Tundef2pd_us     100000   // 100mS
#define _NRF24L01P_TIMING_Tstby2a_us          130   // 130uS
#define _NRF24L01P_TIMING_Thce_us              10   //  10uS
#define _NRF24L01P_TIMING_Tpd2stby_us        4500   // 4.5mS worst case
#define _NRF24L01P_TIMING_Tpece2csn_us          4   //   4uS
/**@}*/ 


/** @name NRF24L01+ default values 
 *  These are bits of the default values 
 */
/**@{*/ 
#define DEFAULT_NRF24L01P_ADDRESS       ((unsigned long long) 0xE7E7E7E7E7 )
#define DEFAULT_NRF24L01P_ADDRESS_WIDTH  5
#define DEFAULT_NRF24L01P_CRC            NRF24L01P_CRC_8_BIT
#define DEFAULT_NRF24L01P_RF_FREQUENCY  (NRF24L01P_MIN_RF_FREQUENCY + 2)
#define DEFAULT_NRF24L01P_DATARATE       NRF24L01P_DATARATE_1_MBPS
#define DEFAULT_NRF24L01P_TX_PWR         NRF24L01P_TX_PWR_ZERO_DB
#define DEFAULT_NRF24L01P_TRANSFER_SIZE  4
/**@}*/ 


/**
 * @brief pipe address datatype
 * data type for the pipe address
 */
#define pipeAddrType_t uint64_t

/**
 * @brief CRC options
 * camn be 8 bit or 16 bit CRC
 */
typedef enum _nrf24l01p_crc_enum{
	_NRF24L01P_CONFIG_CRC_NONE      =  (0),
	_NRF24L01P_CONFIG_CRC_8BIT      =  (_NRF24L01P_CONFIG_EN_CRC),
	_NRF24L01P_CONFIG_CRC_16BIT     =  (_NRF24L01P_CONFIG_EN_CRC|_NRF24L01P_CONFIG_CRC0),
}_nrf24l01p_crc_t;

/**
 * @brief address width options
 * address width can be 3 , 4 or 5 bytes
 */
typedef enum _nrf24l01p_aw_enum{
	_NRF24L01P_SETUP_AW_AW_3BYTE   =  (0x1<<0),/**< 3 bytes address width */
	_NRF24L01P_SETUP_AW_AW_4BYTE   =  (0x2<<0),/**< 4 bytes address width */
	_NRF24L01P_SETUP_AW_AW_5BYTE   =  (0x3<<0),/**< 5 bytes address width */
}_nrf24l01p_aw_t;

/**
 * @brief rf power enumeration
 * antenna power options
 */
typedef enum _nrf24l01p_RF_power_enum{
	_NRF24L01P_RF_SETUP_RF_PWR_0DBM        =  (0x3<<1),
	_NRF24L01P_RF_SETUP_RF_PWR_MINUS_6DBM  =  (0x2<<1),
	_NRF24L01P_RF_SETUP_RF_PWR_MINUS_12DBM =  (0x1<<1),
	_NRF24L01P_RF_SETUP_RF_PWR_MINUS_18DBM =  (0x0<<1),
}_nrf24l01p_RFpower_t;

/**
 * @brief data rate enumeration
 * choose data rate between 250kbps, 1mbps or 2mbps
 */
typedef enum _nrf24l01p_datarate_enum{
	_NRF24L01P_RF_SETUP_RF_DR_250KBPS    =    (_NRF24L01P_RF_SETUP_RF_DR_LOW_BIT),
	_NRF24L01P_RF_SETUP_RF_DR_1MBPS      =    (0),
	_NRF24L01P_RF_SETUP_RF_DR_2MBPS      =    (_NRF24L01P_RF_SETUP_RF_DR_HIGH_BIT),
}_nrf24l01p_datarate_t;

/**
 * @brief pipe numbers enumeration
 * these are the available pipe numbers
 */
typedef enum _nrf24l01p_pipe_enum{
	_NRF24L01P_PIPE_P0       =    0,/**< Pipe 0 */
	_NRF24L01P_PIPE_P1       =    1,/**< Pipe 1 */
	_NRF24L01P_PIPE_P2       =    2,/**< Pipe 2 */
	_NRF24L01P_PIPE_P3       =    3,/**< Pipe 3 */
	_NRF24L01P_PIPE_P4       =    4,/**< Pipe 4 */
	_NRF24L01P_PIPE_P5       =    5,/**< Pipe 5 */
}_nrf24l01p_pipe_t;


/**
 * @brief this is brief enum.
 * brief enum continued.
 *
 * expect detailed enum here.
 * @note can also have note.wow
 */
typedef enum {
	_NRF24L01P_MODE_UNKNOWN,/**< NRF24L01+ unknown mode */
	_NRF24L01P_MODE_POWER_DOWN,/**< NRF24L01+ Power Down mode */
	_NRF24L01P_MODE_STANDBY,/**< NRF24L01+ Standby mode */
	_NRF24L01P_MODE_RX,/**< NRF24L01+ RX mode */
	_NRF24L01P_MODE_TX,/**< NRF24L01+ TX mode */
} nRF24L01P_Mode_Type;





/**
 * @name NRF24L01+ state variables
 */
 /**@{*/ 
bool _nrf24l01p_ce_value;/**< state of the CE pin*/
bool _nrf24l01p_csn_value;/**< state of the CSN pin */
uint8_t mode;/**< state of the as being in TX or RX mode */
nRF24L01P_Mode_Type _nrf24l01p_mode;/**< state of the radio */
bool _nrf24l01p_PTX_noBlockingExitFlag;
unsigned int _nrf24l01p_PTX_noBlocking_SoftRetryCnt;
/**@}*/ 




/**
 * @name NRF24L01+ hardware functions
 */
 /**@{*/ 
 
 /**
 * @brief Print Info
 * prints the general info of the NRF24L01+ registers
 */
void _nrf24l01p_print_info();
/**
 * @brief CE Pin
 * this function handles the CE pin value_com
 * @param state logic high or low
 */
void _nrf24l01p_ce_pin(bool state);
/**
 * @brief CSN Pin
 * this function handles the CE pin value_com
 * @param state logic high or low
 */
void _nrf24l01p_csn_pin(bool state);
/**
 * @brief Initialize
 * hardware initialization of the NRF24L01+
 */
void _nrf24l01p_init();
/**
 * @brief Initialize Blocking
 * Stays in loop until status is NULL
 */
void _nrf24l01p_reinit_loop();
/** 
 * @brief Read Registers
 * this function reads the registers
 * @param  address  address of the register to read
 * @param  dataout  address of array to read into
 * @param  len  number of bytes to read
 * @return none.
 */
void _nrf24l01p_read_register(uint8_t address, uint8_t *dataout, int len);
/** 
 * @brief Write Registers
 * this function writes into the registers
 * @param  address  address of the register to write into
 * @param  dataout  address of array which holds data to write
 * @param  len  number of bytes to write
 * @return none.
 */
void _nrf24l01p_write_register(uint8_t address, uint8_t *datain, int len);
/** 
 * @brief Read RX payload
 * this function reads the payload from the RX FIFO
 * @param  dataout  address of array to read payload data into
 * @param  paylen number of bytes to read from the payload
 * @return none.
 */
void _nrf24l01p_read_rx_payload(uint8_t *dataout, int pay_len);
/** 
 * @brief Write RX payload
 * this function writes the payload into TX FIFO
 * @param  datain  address of array containing the data to write
 * @param  paylen number of bytes to write into the payload
 * @return none.
 */
void _nrf24l01p_write_tx_payload(uint8_t *datain, int pay_len);
/** 
 * @brief Flush TX
 * this function flushes the TX FIFO buffer
 */
void _nrf24l01p_flush_tx();
/** 
 * @brief Flush RX
 * this function flushes the TX FIFO buffer
 */
void _nrf24l01p_flush_rx();
/** 
 * @brief Reuse TX
 * this function reuses the last data in the TX FIFO
 *
 * if the FIFO buffer is not flushed, this command resends the payload
 * can be used by software when failed to recieve ACK from receiver
 * saves CPU time since the payload need not be written all over again
 */
void _nrf24l01p_reuse_tx_payload();
/** 
 * @brief Read payload width
 * reads the number of byte in the last FIFO payload
 *
 * can only be used if dynamic payload is enabled in FEATURE register
 * @see _nrf24l01p_enable_dynamic_payload()
 */
int _nrf24l01p_read_rx_payload_width();
/** 
 * @brief write ACK payload
 * payload data to send along with ack
 *
 * this is done prior to recieving data on a pipe. 
 * when an ack payload is written for a specific pipe
 * and that pipe is enabled and auto ack is enableds as well
 * on receiving a data on that pipe, it will send this data 
 * on the payload along with an ack. If nothing is written on
 * this payload, then it will send 0 bytes along with the ack
 * @param  pipe  pipe number agains which the ack payload is written
 * @param  datain  address of array containing the data to write
 * @param  paylen number of bytes to write into the payload
 */
void _nrf24l01p_write_ack_payload(_nrf24l01p_pipe_t pipe, uint8_t *datain, int pay_len);
/** 
 * @brief write ACK no payload
 * payload data to send without an ACK
 *
 * @param  datain  address of array containing the data to write
 * @param  paylen number of bytes to write into the payload
 */
void _nrf24l01p_write_tx_payload_noack(uint8_t *datain, int pay_len);
/** 
 * @brief Get Status
 * reads the status of the NRF24L01+
 * @return status of the NRF24L01+
 */
int _nrf24l01p_get_status();



/**@}*/ 










/**
 * @name NRF24L01+ register control functions
 */
 /**@{*/ 
void _nrf24l01p_power_up();
void _nrf24l01p_power_down();
void _nrf24l01p_rx_mode();
void _nrf24l01p_tx_mode();
void _nrf24l01p_set_CRC(_nrf24l01p_crc_t opt);

void _nrf24l01p_enable_auto_ack(_nrf24l01p_pipe_t pipe);
void _nrf24l01p_disable_auto_ack(_nrf24l01p_pipe_t pipe);
void _nrf24l01p_disable_auto_ack_all_pipes();

void _nrf24l01p_enable_rx_on_pipe(_nrf24l01p_pipe_t pipe);
void _nrf24l01p_disable_rx_on_pipe(_nrf24l01p_pipe_t pipe);

void _nrf24l01p_set_address_width(_nrf24l01p_aw_t width);
_nrf24l01p_aw_t _nrf24l01p_get_address_width();

void _nrf24l01p_set_auto_retransmission_count(uint8_t count);
uint8_t _nrf24l01p_read_auto_retransmission_count();
void _nrf24l01p_set_auto_retransmission_delay(uint8_t times250us);
uint8_t _nrf24l01p_read_auto_retransmission_delay();

void _nrf24l01p_set_frequency_offset(uint8_t offset);
uint8_t _nrf24l01p_get_frequency_offset();

void _nrf24l01p_set_DataRate(_nrf24l01p_datarate_t DataRate);
_nrf24l01p_datarate_t _nrf24l01p_get_DataRate();
void _nrf24l01p_set_RF_Power(_nrf24l01p_RFpower_t RFpower);
_nrf24l01p_RFpower_t _nrf24l01p_get_RF_Power();

bool _nrf24l01p_get_tx_fifo_full_flag();
bool _nrf24l01p_get_max_retry_flag();
void _nrf24l01p_clear_max_retry_flag();
bool _nrf24l01p_get_data_sent_flag();
void _nrf24l01p_clear_data_sent_flag();
bool _nrf24l01p_get_data_ready_flag();
void _nrf24l01p_clear_data_ready_flag();
_nrf24l01p_pipe_t _nrf24l01p_get_rx_payload_pipe();

uint8_t _nrf24l01p_get_arc_count();
uint8_t _nrf24l01p_get_plos_count();
void _nrf24l01p_clear_plos_count();
bool _nrf24l01p_get_rpd();

void _nrf24l01p_set_RX_pipe_address(_nrf24l01p_pipe_t pipe,pipeAddrType_t address);
pipeAddrType_t _nrf24l01p_get_RX_pipe_address(_nrf24l01p_pipe_t pipe);
void _nrf24l01p_set_TX_pipe_address(pipeAddrType_t address);
pipeAddrType_t _nrf24l01p_get_TX_pipe_address();

uint8_t _nrf24l01p_get_RX_pipe_width(_nrf24l01p_pipe_t pipe);


bool _nrf24l01p_get_fifo_flag_rx_empty();
bool _nrf24l01p_get_fifo_flag_rx_full();
bool _nrf24l01p_get_fifo_flag_tx_empty();
bool _nrf24l01p_get_fifo_flag_tx_full();
bool _nrf24l01p_get_fifo_flag_tx_reuse();

void _nrf24l01p_enable_dynamic_payload_pipe(_nrf24l01p_pipe_t pipe);
void _nrf24l01p_disable_dynamic_payload_pipe(_nrf24l01p_pipe_t pipe);
void _nrf24l01p_disable_dynamic_payload_all_pipe();
void _nrf24l01p_enable_dynamic_payload();
void _nrf24l01p_disable_dynamic_payload();
void _nrf24l01p_enable_payload_with_ack();
void _nrf24l01p_disable_payload_with_ack();
void _nrf24l01p_enable_dynamic_payload_with_ack();
void _nrf24l01p_disable_dynamic_payload_with_ack();
/**@}*/ 

/**
 * @name NRF24L01+ mid level functions
 */
 /**@{*/ 
int _nrf24l01p_startup();
int _nrf24l01p_default_config();
int _nrf24l01p_stateMode(nRF24L01P_Mode_Type mode);
int _nrf24l01p_PTX_Handle();
int _nrf24l01p_PRX_Handle();
bool _nrf24l01p_writable();
bool _nrf24l01p_readable();
int _nrf24l01p_send(uint8_t *data, int datalen);

/**@}*/ 

/**
 * @name NRF24L01+ top level functions
 */
 /**@{*/ 
bool _nrf24l01p_readableOnPipe(_nrf24l01p_pipe_t pipe);


int _nrf24l01p_send_to_address(pipeAddrType_t address, uint8_t *data, int datalen);
int _nrf24l01p_send_to_address_ack(pipeAddrType_t address, uint8_t *data, int datalen);
void _nrf24l01p_send_until_ack(pipeAddrType_t address, uint8_t *data, int datalen);
int _nrf24l01p_read(_nrf24l01p_pipe_t pipe, uint8_t *data, int datalen);
int _nrf24l01p_read_dyn_pld(_nrf24l01p_pipe_t pipe, uint8_t *data);
void _nrf24l01p_write_ack(_nrf24l01p_pipe_t pipe, uint8_t *data, int datalen);
/**@}*/ 


#endif /* NRF24L01P_H_ */