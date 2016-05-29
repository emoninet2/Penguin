
#include "Component_Drivers/components.h"
#include "XMEGA_API/xmega_api.h"
#include "XMEGA_SERVICES/xmega_services.h"
#include "JSON/jsmn.h"
#include "FreeRTOS.h"
#include "task.h"

volatile int t1var = 0;
volatile int t2var = 0;
volatile int t3var = 0;


volatile uint32_t mytime;

void update_time(){
	
	uint8_t emon_rxData[33];
	char txData[] = "get time";
	_nrf24l01p_write_to_address_ack(0x1122334455, (uint8_t*)txData,strlen(txData));
	
	_delay_ms(500);
	//_nrf24l01p_write_ack(_NRF24L01P_PIPE_P1,ack_echo_data,32);
	if((_nrf24l01p_readable(_NRF24L01P_PIPE_P1))){
		PORTR.OUTTGL = (1<<0);

		volatile int width = _nrf24l01p_read_dyn_pld(_NRF24L01P_PIPE_P1, (uint8_t*) emon_rxData);
		emon_rxData[width] = '\0';
		_nrf24l01p_flush_rx();
		_nrf24l01p_clear_data_ready_flag();
		
		volatile char printlcd[20];
		sprintf(printlcd,"width : %d\0", width);
		
		
		int arg_index_msg = 0;
		char *pch_msg;
		char *args_msg[50];
		pch_msg = strtok(emon_rxData, "{}()\":");
		while(pch_msg != NULL) {
			args_msg[arg_index_msg] = pch_msg;
			arg_index_msg++;
			if(arg_index_msg >=50) break;
			pch_msg = strtok (NULL, "{}()\":");
		}
		
		ssd1306_clear();

		ssd1306_set_page_address(0);
		ssd1306_set_column_address(0);
		ssd1306_write_text(args_msg[0]);
		
		ssd1306_set_page_address(1);
		ssd1306_set_column_address(0);
		ssd1306_write_text(args_msg[1]);
		
		set_system_time(atol(args_msg[1]));
		//mytime = atol(args_msg[1]);
		//sscanf(args_msg[1],"%ul", &mytime );
		asm("nop");
	}
}



void thread_1( void *pvParameters ){
	//serial_startup();
	int i = 0;
	while(1){
		//printf("hello world %d\n\r",i);
		//i++;
		asm("nop");
		t1var++;
		PORTR.OUTTGL = (1<<0);
		vTaskDelay(1000); //wait 1 sec
	}
}
void thread_2( void *pvParameters ){
	//DDRB |= (1<<DDB0); //set PORTB, PIN0 as output
	while(1){
		//PORTB ^= (1<<PORTB0); //toggle PORTB, PIN0
		asm("nop");
		t2var++;
		PORTR.OUTTGL = (1<<1);
		vTaskDelay(2000); //wait 2 seconds
	}
}
void thread_3( void *pvParameters ){
	//DDRB |= (1<<DDB1); //set PORTB, PIN1 as output
	while(1){
		//PORTB ^= (1<<PORTB1); //togle PORTB, PIN0
		asm("nop");
		t3var++;
		PORTD.OUTTGL = (1<<4);
		vTaskDelay(200); //wait 200 ms
	}
}






/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{

	rtc_initialize();
	DigitalPin_t led = {&PORTR, 0};
	DigitalPin_t led2 = {&PORTR, 1};

	
// 	_nrf24l01p_init();
// 	_nrf24l01p_enable_dynamic_payload();
// 	_nrf24l01p_enable_payload_with_ack();
// 
// 	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P0);
// 	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P1);
// 	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P2);
// 	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P3);
// 	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P4);
// 	_nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P5);
// 	
// 	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P0);
// 	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P1);
// 	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P2);
// 	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P3);
// 	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P4);
// 	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P5);
// 	
// 	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P0);
// 	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P1);
// 	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P2);
// 	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P3);
// 	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P4);
// 	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P5);
// 
// 	_nrf24l01p_set_auto_retransmission_delay(15);
// 	_nrf24l01p_set_DataRate(_NRF24L01P_RF_SETUP_RF_DR_250KBPS);
// 	_nrf24l01p_flush_rx();
// 	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P1, 0xC2C2C2C255);	
	
	
	uint8_t page;
	uint8_t column;

	gfx_mono_set_framebuffer(framebuffer);

	ssd1306_init();
	asm("nop");
	ssd1306_clear();

	ssd1306_set_display_start_line_address(0);


	for (page = 0; page < GFX_MONO_LCD_PAGES; page++) {
		for (column = 0; column < GFX_MONO_LCD_WIDTH; column++) {
			gfx_mono_ssd1306_put_byte(page, column, 0x00, 1);
		}
	}

	asm("nop");
	ssd1306_set_page_address(0);
	ssd1306_write_text("EMON");



	DigitalPin_SetDIr(&led,1);
	DigitalPin_SetDIr(&led2,1);
	//PORT_SetDirection(&PORTR,(1<<0));

	CLKSYS_Enable( OSC_RC32MEN_bm );
	do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
	CLKSYS_Disable( OSC_RC2MEN_bm );
	


	//char ack_echo_data[] = "hello hello bolkey";
	

	
	char time_string[20];
	uint8_t rxData[33];
	
	
	PORTR.DIRSET = (1<<0);
	PORTR.DIRSET = (1<<1);
	PORTD.DIRSET = (1<<4);
	
	
	//creating the threads
	xTaskCreate(thread_1, (const char*) "t1", 300, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(thread_2,(const char *) "t2", 100, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(thread_3,(const char *) "t3", 100, NULL, tskIDLE_PRIORITY, NULL );
	
	
	//starting the scheduler
	vTaskStartScheduler();
	
	
	while(1)
	{
		
// 		struct tm * timeinfo;
// 		timeinfo = localtime ((const time_t*)&__system_time);
		
// 		ssd1306_clear();
// 		ssd1306_set_page_address(0);
// 		ssd1306_set_column_address(0);
// 		sprintf(time_string,"%lu", __system_time);
// 		//sprintf(time_string,"%d", timeinfo->tm_year);
// 		ssd1306_write_text(time_string);
// 		_delay_ms(500);
		
		
		if((_nrf24l01p_readable(_NRF24L01P_PIPE_P1))){
			int width = _nrf24l01p_read_dyn_pld(_NRF24L01P_PIPE_P1, (uint8_t*) rxData);
			rxData[width] = '\0';
			//mynrf.write_to_address_ack(0xC2C2C2C255, (uint8_t*)rxData,strlen(rxData));
			
			ssd1306_clear();

			ssd1306_set_page_address(0);
			ssd1306_set_column_address(0);
			ssd1306_write_text(rxData);
			
			PORTR.OUTTGL = (1<<0);
		}
// 		
		
		
	
//  		char txData[] = "sample text";
//  		_nrf24l01p_write((uint8_t*)txData,strlen(txData));
//  		_nrf24l01p_write_to_address_ack(0x1122334455, (uint8_t*)txData,strlen(txData));
// 		_delay_ms(1000);
				
			//char txData[] = "sample text";
			//_nrf24l01p_write_to_address_ack(0x1122334455, (uint8_t*)txData,strlen(txData));
			
 			//HID_Device_USBTask(&Generic_HID_Interface);
 			//USB_USBTask();
	}
}

