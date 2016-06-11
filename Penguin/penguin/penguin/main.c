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


DigitalPin_t led = {&PORTR, 0};
DigitalPin_t led2 = {&PORTR, 1};


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
		lcd03_backlight(lcdbackstat);
		lcdbackstat = !lcdbackstat;
		DigitalPin_ToggleValue(&led);
		
		vTaskSuspendAll();
		fprintf(&USBSerialStream, "fuck you\r\n");
		xTaskResumeAll();
		
		
	}
}




void thread_2( void *pvParameters ){

	uint8_t writeData[] = {0, 0x0c};

	
	while(1){

		vTaskDelay(100);
		DigitalPin_ToggleValue(&led2);
	}
}


TWI_Master_t lcd03i2c;


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	DigitalPin_SetDIr(&led,1);
	DigitalPin_SetDIr(&led2,1);
	
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

	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
	GlobalInterruptEnable();

	asm("nop");

	TWI_MasterInit(&lcd03i2c,
	&TWIE,
	TWI_MASTER_INTLVL_OFF_gc,
	TWI_BAUD(F_CPU, 100000));
	
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


	
	xTaskCreate(thread_1,(signed portCHAR *) "t1", 100, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(thread_2,(signed portCHAR *) "t2", 100, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(USBThread,(signed portCHAR *) "usb", 200, NULL, tskIDLE_PRIORITY, NULL );
	

	//starting the scheduler
	vTaskStartScheduler();


	for (;;)
	{
// 		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
// 		USB_USBTask();
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
