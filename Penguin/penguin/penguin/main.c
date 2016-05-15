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
 *  Main source file for the GenericHID demo. This file contains the main tasks of
 *  the demo and is responsible for the application hardware configuration.
 */



#include "main.h"
#include "Component_Drivers/components.h"
#include "XMEGA_API/xmega_api.h"
#include "XMEGA_SERVICES/xmega_services.h"
#include "JSON/jsmn.h"


/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Generic_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_GenericHID,
				.ReportINEndpoint             =
					{
						.Address              = GENERIC_IN_EPADDR,
						.Size                 = GENERIC_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = PrevHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
			},
	};


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	
	//set_system_time (1463064367);
	rtc_initialize();
	
	DigitalPin_t led = {&PORTR, 0};
	

	
	_nrf24l01p_init();
	_nrf24l01p_enable_dynamic_payload();
	_nrf24l01p_enable_payload_with_ack();
	
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P0);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P1);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P2);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P3);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P4);
	_nrf24l01p_enable_rx_on_pipe(_NRF24L01P_PIPE_P5);
	
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P0);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P1);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P2);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P3);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P4);
	_nrf24l01p_enable_dynamic_payload_pipe(_NRF24L01P_PIPE_P5);
	
	 _nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P0);
	 _nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P1);
	 _nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P2);
	 _nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P3);
	 _nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P4);
	 _nrf24l01p_enable_auto_ack(_NRF24L01P_PIPE_P5);
	 
	_nrf24l01p_set_auto_retransmission_delay(15);
	
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


	
 	//SetupHardware();
// 
 	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
 	GlobalInterruptEnable();

	DigitalPin_SetDIr(&led,1);
	//PORT_SetDirection(&PORTR,(1<<0));

	CLKSYS_Enable( OSC_RC32MEN_bm );
	do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
	CLKSYS_Disable( OSC_RC2MEN_bm );
	
	_nrf24l01p_flush_rx();
		
	//_nrf24l01p_set_TX_pipe_address(0xc2c2c2c2c4);	
		
	while(1)
	{
			uint8_t emon_rxData[100];
			
			//_delay_ms(1000);
// 			rtc_ms_delay(1000);
// 			
// 			char msg[] = "get time";
// 			_nrf24l01p_write(msg,strlen(msg));
			 //_nrf24l01p_write_ack(_NRF24L01P_PIPE_P0,msg,strlen(msg));
		
			
			if((_nrf24l01p_readable(_NRF24L01P_PIPE_P4))){
				PORTR.OUTTGL = (1<<0);
				//printf("status %x\r\n",_nrf24l01p_get_status());
				//printf("pipe : %d\r\n", _nrf24l01p_get_rx_payload_pipe());
				//led1 = !led1;
				
				int width = _nrf24l01p_read_dyn_pld(_NRF24L01P_PIPE_P4, (uint8_t*) emon_rxData);
				emon_rxData[width] = '\0';
				_nrf24l01p_flush_rx();
				_nrf24l01p_clear_data_ready_flag();
	
// 				int arg_index = 0;
// 				char *pch;
// 				char *remotch_args[ 10];
// 				pch = strtok((char*)emon_rxData, "\"{},\r");
// 				while(pch != NULL) {
// 					remotch_args[arg_index] = pch;
// 					arg_index++;
// 					if(arg_index >=10) break;
// 					pch = strtok (NULL, "\"{},\r");
// 				}
		  
// 	  			ssd1306_clear();
// 	  			ssd1306_set_column_address(0);
// 				ssd1306_set_page_address(0);
// 	  			ssd1306_write_text(emon_rxData);
				  
				_nrf24l01p_flush_rx();
			}


		
 			//HID_Device_USBTask(&Generic_HID_Interface);
 			//USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;

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

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

	USB_Device_EnableSOFEvents();

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Generic_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */

volatile int i=0;

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
	uint8_t* Data        = (uint8_t*)ReportData;
	uint8_t  CurrLEDMask = LEDs_GetLEDs();

	Data[0] = ((CurrLEDMask & LEDS_LED1) ? 1 : 0);
	Data[1] = ((CurrLEDMask & LEDS_LED2) ? 1 : 0);
	Data[2] = ((CurrLEDMask & LEDS_LED3) ? 1 : 0);
	Data[3] = ((CurrLEDMask & LEDS_LED4) ? 1 : 0);





	*ReportSize = GENERIC_REPORT_SIZE;
	return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
	uint8_t* Data       = (uint8_t*)ReportData;
	uint8_t  NewLEDMask = LEDS_NO_LEDS;


	char myprintbuff[20];
	sprintf(myprintbuff,"%d : %c%c%c%c%c",i, Data[0],Data[1],Data[2],Data[3],Data[4] );
	for (int page = 0; page < GFX_MONO_LCD_PAGES; page++) {
		for (int column = 0; column < GFX_MONO_LCD_WIDTH; column++) {
			gfx_mono_ssd1306_put_byte(page, column, 0x00, 1);
		}
	}
	i++;
	asm("nop");
	ssd1306_set_page_address(0);
	ssd1306_set_column_address(0);
	ssd1306_write_text(myprintbuff);



// 
// 	if (Data[0])
// 	  NewLEDMask |= LEDS_LED1;
// 
// 	if (Data[1])
// 	  NewLEDMask |= LEDS_LED2;
// 
// 	if (Data[2])
// 	  NewLEDMask |= LEDS_LED3;
// 
// 	if (Data[3])
// 	  NewLEDMask |= LEDS_LED4;
// 
// 	LEDs_SetAllLEDs(NewLEDMask);
}

