/*
             LUFA Library
     Copyright (C) Dean Camera, 2015.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2015  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include <stdlib.h>
#include <math.h>
#include "time.h"
#include "main.h"
#include "xmega_drivers.h"
#include "Component_Drivers/components.h"
#include "XMEGA_API/xmega_api.h"
#include "XMEGA_SERVICES/xmega_services.h"
#include "JSON/jsmn.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

volatile time_t local_timestamp;

DigitalPin_t led = {&PORTR, 0};
DigitalPin_t led2 = {&PORTR, 1};
DigitalPin_t myswitch = {&PORTF, 1};

DigitalPin_t mybutton1 = {&PORTA, 7};
DigitalPin_t mybutton2 = {&PORTA, 5};
DigitalPin_t mybutton3 = {&PORTC, 0};
DigitalPin_t mybutton4 = {&PORTC, 1};

DigitalPin_t myrelay1 = {&PORTA, 6};
DigitalPin_t myrelay2 = {&PORTA, 4};
DigitalPin_t myrelay3 = {&PORTA, 2};
DigitalPin_t myrelay4 = {&PORTE, 0};


volatile bool global_light0_stat = 0;
volatile bool global_light0_stat_request = 0;

bool relayUpdate=0;
bool relayState1=1,relayState2=1,relayState3=1,relayState4=1;




TWI_Master_t lcd03i2c;


/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;


void USBThread( void *pvParameters ){


	while(1){

		// want CDC and USB functions to run without interruption but
		// with interrupts enabled so ENTER/EXIT_CRITICAL won't work
		vTaskSuspendAll();
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
		xTaskResumeAll();
		vTaskDelay((portTickType) 1 );
	}
}





void thread_1( void *pvParameters ){

		//xQueue = xQueueCreate( 10, sizeof( uint8_t ) );

	uint8_t i =0;
	bool lcdbackstat = 0;
	while(1){
		//xQueueSend( xQueue,( void * ) &i,( TickType_t ) 10 );
		i++;
		vTaskDelay(500);
		//lcd03_backlight(lcdbackstat);
		lcdbackstat = !lcdbackstat;
		DigitalPin_ToggleValue(&led);
		
// 		vTaskSuspendAll();
// 		fprintf(&USBSerialStream, "hey\r\n");
// 		xTaskResumeAll();
		
		
	}
}




void thread_2( void *pvParameters ){

	uint8_t writeData[] = {0, 0x0c};

		ADCA.CTRLA =   ADC_ENABLE_bm;//no DMA request | enable ADCA module
		ADCA.CTRLB = ADC_CURRLIMIT_NO_gc | ADC_CONMODE_bm | ADC_RESOLUTION_12BIT_gc;//high imp | no current limit|unsigned | no free run | 12-bit |
		ADCA.REFCTRL =  ADC_REFSEL_INTVCC_gc /*| ADC_BANDGAP_bm | ADC_TEMPREF_bm*/;
		ADCA.EVCTRL = ADC_EVSEL_0_gc | ADC_EVACT_NONE_gc ;
		ADCA.PRESCALER = ADC_PRESCALER_DIV256_gc;
		ADCA.INTFLAGS = /*ADC_CH3IF_bm | ADC_CH2IF_bm | ADC_CH1IF_bm |*/ ADC_CH0IF_bm;
		
		ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;



	
	while(1){
	int tempsum = 0;
	float tempsum_avg = 0;
	
	for(int i=0;i<10;i++){
		ADCA.CTRLA |= ADC_CH0START_bm;
		while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
		tempsum += ADCA.CH0RES;
	}
	tempsum_avg = tempsum/10;		
			//printf("the temp is %d\n\r",  temp);
			float adcval = (tempsum_avg/2048)*(3.3/1.6);
			


			vTaskSuspendAll();
			fprintf(&USBSerialStream, "the temp is %f\n\r", adcval );
			xTaskResumeAll();


		vTaskDelay(100);
		DigitalPin_ToggleValue(&led2);
	}
}

volatile bool update_time = 0;
volatile uint32_t mytime = 0;

void *command_handler(char **args,int arg_count){

	if(!strcmp(args[0], "light") ) {
		if(!strcmp(args[1], "0")) {
			if(!strcmp(args[2], "0")) {
				relayState1 = 1;
			}
			else if(!strcmp(args[2], "1")) {
				relayState1 = 0;
			}
		}
		else if(!strcmp(args[1], "1")) {
			if(!strcmp(args[2], "0")) {
				relayState2 = 1;
			}
			else if(!strcmp(args[2], "1")) {
				relayState2 = 0;
			}
		}
	}
	else if(!strcmp(args[0], "fan") ) {
		if(!strcmp(args[1], "0")) {
			relayState4 = 1;
		}
		else if(!strcmp(args[1], "1")) {
			relayState4 = 0;
		}
	}
	else if(!strcmp(args[0],"time")){
		if (!strcmp(args[1], "set")){
			mytime = atol(args[2]);
			update_time = 1;
		}
	}
	else{

	}

	return 0;
}

void command_parse_execute(char *command){

	int arg_index = 0;
	char *pch;
	char *remotch_args[ 10];
	pch = strtok(command, " ,");
	while(pch != NULL) {
		remotch_args[arg_index] = pch;
		arg_index++;
		if(arg_index >=10) break;
		pch = strtok (NULL, " ,");
	}
	command_handler(remotch_args,arg_index);
}

void thread_3( void *pvParameters ){
	_nrf24l01p_init();

	//LEFT
	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P1, 0x4C4C4C4C31);
	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P2, 0x4C4C4C4C32);
	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P3, 0x4C4C4C4C33);
	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P4, 0x4C4C4C4C34);
	_nrf24l01p_set_RX_pipe_address(_NRF24L01P_PIPE_P5, 0x4C4C4C4C35);
	char rxData[35];

	while(1){

		char config;
		char fifo;

		_nrf24l01p_read_register(_NRF24L01P_REG_CONFIG,&config,1);
		_nrf24l01p_read_register(_NRF24L01P_REG_FIFO_STATUS,&fifo,1);

		fprintf(&USBSerialStream,"status : %x\r\n", _nrf24l01p_get_status());
		fprintf(&USBSerialStream,"config : %x\r\n", config);
		fprintf(&USBSerialStream,"fifo : %x\r\n", fifo);



		
		vTaskDelay(500);
		if((_nrf24l01p_readable(_NRF24L01P_PIPE_P1))){
			asm("nop");
			DigitalPin_ToggleValue(&led2);
			int width = _nrf24l01p_read_dyn_pld(_NRF24L01P_PIPE_P1, (uint8_t*) rxData);
			rxData[width] = '\0';
			command_parse_execute(rxData);
		}
		
	}
}

struct tm rtc_time;



void time_parse_print(char *command){
	printf("now parsing string\n");
	int arg_index = 0;
	char *pch;
	char *remotch_args[ 10];
	pch = strtok(command, " ");
	while(pch != NULL) {
		remotch_args[arg_index] = pch;
		arg_index++;
		if(arg_index >=10) break;
		pch = strtok (NULL, " ");
	}

	char time[15];
	char date[30];
	char day[15];

	sprintf(day,"%s",remotch_args[0]);
	sprintf(time,"%s",remotch_args[3]);
	sprintf(date,"%s %s %s",remotch_args[2],remotch_args[1],remotch_args[4]);

	ssd1306_clear();
	ssd1306_set_page_address(0);
	ssd1306_set_column_address(0);
	ssd1306_write_text(time);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text(date);
	ssd1306_set_page_address(2);
	ssd1306_set_column_address(0);
	ssd1306_write_text(day);


}






void thread_4( void *pvParameters ){
	ds1302_initialize();
	uint32_t myinittime = 1469834372;
	//uint32_t mytime = 1471011608;
	//ds1302_setTimestamp(myinittime    - UNIX_OFFSET);
	//set_zone(+4 * ONE_HOUR);

	while(1){
// 		char temp = ds1302_readReg(0x81);
// 		fprintf(&USBSerialStream, "halla bol %x %x %x %x %x %x %x %x\r\n",temp);
// 		
// 		fprintf(&USBSerialStream, "halla bol ");
// 		for(int i=0x81;i<=0x91;i+=2){
// 			char temp = ds1302_readReg(i);
// 			fprintf(&USBSerialStream, "%x ",temp);
// 		}
// 		fprintf(&USBSerialStream, "\r\n");

		if(update_time){
			ds1302_setTimestamp(mytime    - UNIX_OFFSET);
			update_time = 0;
		}


		local_timestamp = ds1302_getTimestamp() + 4*ONE_HOUR + ONE_HOUR+1000; //OFFSET GMT+4
		
		//fprintf(&USBSerialStream, "UNIX Timestamp : %lu\r\n",ds1302_getTimestamp() + UNIX_OFFSET);
		//fprintf(&USBSerialStream,"Time as a basic string = %s\n\r", ctime(&local_timestamp));
		
		_nrf24l01p_print_info();

		time_parse_print(ctime(&local_timestamp));//function to print the time and date on lcd

		
		//set_system_time(system_time);
			
		vTaskDelay(200);
	}
}




void thread_5( void *pvParameters ){


	DigitalPin_Config(&mybutton1,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_INPUT_DISABLE_gc);
	DigitalPin_Config(&mybutton2,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_INPUT_DISABLE_gc);
	DigitalPin_Config(&mybutton3,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);
	DigitalPin_Config(&mybutton4,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);


	while(1){
		vTaskDelay(100);

		if(!DigitalPin_GetValue(&mybutton1)){
			while(!DigitalPin_GetValue(&mybutton1));
			relayState1=!relayState1;


		}

		if(!DigitalPin_GetValue(&mybutton2)){
			while(!DigitalPin_GetValue(&mybutton2));
			relayState2=!relayState2;

		}

		if(!DigitalPin_GetValue(&mybutton3)){
			while(!DigitalPin_GetValue(&mybutton3));
			relayState3=!relayState3;

		}

		if(!DigitalPin_GetValue(&mybutton4)){
			while(!DigitalPin_GetValue(&mybutton4));
			relayState4=!relayState4;

		}

	
// 		if(DigitalPin_GetValue(&mybutton4)){
// 			DigitalPin_SetValue(&led);
// 			DigitalPin_SetValue(&myrelay1);
// 			DigitalPin_SetValue(&myrelay2);
// 			DigitalPin_SetValue(&myrelay3);
// 			DigitalPin_SetValue(&myrelay4);
// 		}
// 		else{
// 			DigitalPin_ClearValue(&led);
// 			DigitalPin_ClearValue(&myrelay1);
// 			DigitalPin_ClearValue(&myrelay2);
// 			DigitalPin_ClearValue(&myrelay3);
// 			DigitalPin_ClearValue(&myrelay4);
// 		}

	}

}

void thread_6( void *pvParameters ){

	DigitalPin_Config(&mybutton1,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_INPUT_DISABLE_gc);
	DigitalPin_Config(&mybutton2,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_INPUT_DISABLE_gc);
	DigitalPin_Config(&mybutton3,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);
	DigitalPin_Config(&mybutton4,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);

	DigitalPin_SetDir(&myrelay1,1);
	DigitalPin_SetDir(&myrelay2,1);
	DigitalPin_SetDir(&myrelay3,1);
	DigitalPin_SetDir(&myrelay4,1);

	DigitalPin_Config(&myrelay4,0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);

	while(1){

		vTaskDelay(100);

		if(!DigitalPin_GetValue(&mybutton1)){
			while(!DigitalPin_GetValue(&mybutton1));
			relayState1=!relayState1;
		}

		if(!DigitalPin_GetValue(&mybutton2)){
			while(!DigitalPin_GetValue(&mybutton2));
			relayState2=!relayState2;
		}

		if(!DigitalPin_GetValue(&mybutton3)){
			while(!DigitalPin_GetValue(&mybutton3));
			relayState3=!relayState3;
		}

		if(!DigitalPin_GetValue(&mybutton4)){
			while(!DigitalPin_GetValue(&mybutton4));
			relayState4=!relayState4;
		}


		if(relayState1) DigitalPin_SetValue(&myrelay1);
		else DigitalPin_ClearValue(&myrelay1);

		if(relayState2) DigitalPin_SetValue(&myrelay2);
		else DigitalPin_ClearValue(&myrelay2);

		if(relayState3) DigitalPin_SetValue(&myrelay3);
		else DigitalPin_ClearValue(&myrelay3);

		if(relayState4) DigitalPin_SetValue(&myrelay4);
		else DigitalPin_ClearValue(&myrelay4);
	}
}




/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	volatile time_t mytime = atol("1471006630");
	//volatile time_t mytime = 1471006630 ;
	asm("nop");


	DigitalPin_SetDir(&led,1);
	DigitalPin_SetDir(&led2,1);
	
	SetupHardware();

	uint8_t page;
	uint8_t column;
	gfx_mono_set_framebuffer(framebuffer);
	ssd1306_init();
	ssd1306_clear();
	ssd1306_set_display_start_line_address(0);
 	for (page = 0; page < GFX_MONO_LCD_PAGES; page++) {
		for (column = 0; column < GFX_MONO_LCD_WIDTH; column++) {
			gfx_mono_ssd1306_put_byte(page, column, 0x00, 1);
		}
	}

	asm("nop");
	ssd1306_clear();
	ssd1306_set_page_address(0);
	ssd1306_write_text("EMON");


	DigitalPin_SetDir(&led,1);
	DigitalPin_SetDir(&led2,1);
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

	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
	GlobalInterruptEnable();

	asm("nop");

// 	TWI_MasterInit(&lcd03i2c,
// 	&TWIE,
// 	TWI_MASTER_INTLVL_OFF_gc,
// 	TWI_BAUD(F_CPU, 100000));
	
	asm("nop");

	


//  	uint8_t writeData[] = {0,0x14};
// 	TWI_MasterWrite(&lcd03i2c,
//                      0x64,
//                      writeData,
//                      sizeof(writeData));
// 	asm("nop");
// 
// 
// 	while(lcd03i2c.status != 0){
// 		TWI_MasterWriteHandler(&lcd03i2c);
// 		while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
// 	}
	


	asm("nop");




// 	TWIE.MASTER.ADDR = 0xC8;  // write to RTC
// 	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
// 	TWIE.MASTER.DATA = 0x00;       // write word addr
// 	while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
// 	for(int i=0;i<2;i++){                  // write date and time
// 		TWIE.MASTER.DATA =writeData[i];
// 		while(!(TWIE.MASTER.STATUS&TWI_MASTER_WIF_bm));
// 	}

	



	asm("nop");


	
	//xTaskCreate(thread_1,(signed portCHAR *) "t1", 100, NULL, tskIDLE_PRIORITY, NULL );
	//xTaskCreate(thread_2,(signed portCHAR *) "t2", 500, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(thread_3,(signed portCHAR *) "t3", 500, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(thread_4,(signed portCHAR *) "t4", 500, NULL, tskIDLE_PRIORITY, NULL );
	//xTaskCreate(thread_5,(signed portCHAR *) "t5", 500, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(thread_6,(signed portCHAR *) "t6", 500, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(USBThread,(signed portCHAR *) "usb", 200, NULL, tskIDLE_PRIORITY, NULL );
	

	//starting the scheduler
	vTaskStartScheduler();


	for (;;)
	{

	}
}


/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}


/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** CDC class driver callback function the processing of changes to the virtual
 *  control lines sent from the host..
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
	/* You can get changes to the virtual CDC lines in this callback; a common
	   use-case is to use the Data Terminal Ready (DTR) flag to enable and
	   disable CDC communications in your application when set to avoid the
	   application blocking while waiting for a host to become ready and read
	   in the pending data from the USB endpoints.
	*/
	bool HostReady = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR) != 0;
}
