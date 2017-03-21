#include <stdlib.h>
#include <math.h>

#include "xmega_drivers.h"
#include "Component_Drivers/components.h"
#include "XMEGA_API/xmega_api.h"
#include "XMEGA_SERVICES/xmega_services.h"
#include "JSON/jsmn.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"



DigitalPin_t led1 = {&PORTR, 0};
DigitalPin_t led2 = {&PORTR, 1};
DigitalPin_t powLedR = {&PORTD, 4};
DigitalPin_t powLedG = {&PORTD, 5};

DigitalPin_t myswitch1 = {&PORTF, 1};
DigitalPin_t myswitch2 = {&PORTF, 2};


DigitalPin_t button[4] = {{&PORTA, 7}, {&PORTA, 5},{&PORTC, 0},{&PORTC, 1} };
DigitalPin_t relay[4] = {{&PORTA, 6}, {&PORTA, 4},{&PORTA, 2},{&PORTE, 0}  };	
volatile bool relayState[4] = {1,1,1,1};	
volatile bool radioRXnotificationLed  =0;
	
	
void board_config(){

	CLKSYS_Enable( OSC_RC32MEN_bm );
	do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
	CLKSYS_Disable( OSC_RC2MEN_bm );

	DigitalPin_SetDir(&led1,1);
	DigitalPin_SetDir(&led2,1);
	DigitalPin_SetDir(&powLedR,1);
	DigitalPin_SetDir(&powLedG,1);

	DigitalPin_SetValue(&led1);
	DigitalPin_SetValue(&led2);
	DigitalPin_SetValue(&powLedR);//off
	DigitalPin_SetValue(&powLedG);//on
	//DigitalPin_ClearValue(&powLedG);//off

	
}


void *command_handler(char **args,int arg_count){

	if(!strcmp(args[0], "light") ) {
		if(!strcmp(args[1], "0")) {
			if(!strcmp(args[2], "0")) {
				relayState[0] = 1;
			}
			else if(!strcmp(args[2], "1")) {
				relayState[0] = 0;
			}
		}
		else if(!strcmp(args[1], "1")) {
			if(!strcmp(args[2], "0")) {
				relayState[1] = 1;
			}
			else if(!strcmp(args[2], "1")) {
				relayState[1] = 0;
			}
		}
		else if(!strcmp(args[1], "2")) {
			if(!strcmp(args[2], "0")) {
				relayState[2] = 1;
			}
			else if(!strcmp(args[2], "1")) {
				relayState[2] = 0;
			}
		}
	}
	else if(!strcmp(args[0], "fan") ) {
		if(!strcmp(args[1], "0")) {
			relayState[3] = 1;
		}
		else if(!strcmp(args[1], "1")) {
			relayState[3] = 0;
		}
	}
	else if(!strcmp(args[0], "all") ) {
		if(!strcmp(args[1], "0")) {
			relayState[0] = 1;
			relayState[1] = 1;
			relayState[2] = 1;
			relayState[3] = 1;
		}
		else if(!strcmp(args[1], "1")) {
			relayState[0] = 0;
			relayState[1] = 0;
			relayState[2] = 0;
			relayState[3] = 0;
		}
	}

	else if(!strcmp(args[0],"bootloader")){

#if defined(PROGRAM_ENTERS_BOOTLOADER)
		cli();
		CCPWrite(&(PMIC.CTRL), PMIC_IVSEL_bm );
		soft_reset();
#else
		uint8_t jmpE = (uint8_t)((BOOT_SECTION_START/2)>>16);
		uint8_t jmpH = (uint8_t)((BOOT_SECTION_START/2)>>8);
		uint8_t jmpL = (uint8_t)((BOOT_SECTION_START/2)>>0);
		//cli();
		//CCPWrite(&(PMIC.CTRL), PMIC_IVSEL_bm );
		asm("ldi ZH, %0"::"i" (jmpH):"r30","r31");
		asm("ldi ZL, %0"::"i" (jmpL):"r30","r31");
		EIND = jmpE;
		asm("eijmp");
#endif

	}
	else{
		//no recognized command
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




RadioConfig_t RadioConfig;
RxPipeConfig_t RxPipeConfig[6];

void RadioReset(){

  RadioConfig.DataReadyInterruptEnabled = 0;
  RadioConfig.DataSentInterruptFlagEnabled = 0;
  RadioConfig.MaxRetryInterruptFlagEnabled = 0;
  RadioConfig.Crc = CONFIG_CRC_16BIT;
  RadioConfig.AutoReTransmissionCount = 15;
  RadioConfig.AutoReTransmitDelayX250us = 15;
  RadioConfig.frequencyOffset = 2;
  RadioConfig.datarate = RF_SETUP_RF_DR_2MBPS;
  RadioConfig.RfPower = RF_SETUP_RF_PWR_0DBM;
  RadioConfig.PllLock = 0;
  RadioConfig.ContWaveEnabled = 0;
  RadioConfig.FeatureDynamicPayloadEnabled = 1;
  RadioConfig.FeaturePayloadWithAckEnabled = 1;
  RadioConfig.FeatureDynamicPayloadWithNoAckEnabled = 1;
  
    RxPipeConfig[0].address = 0x11223344EE;
    RxPipeConfig[1].address = 0x9A4524CE01;
    RxPipeConfig[2].address = 0x9A4524CE02;
    RxPipeConfig[3].address = 0x9A4524CE03;
    RxPipeConfig[4].address = 0x9A4524CE04;
    RxPipeConfig[5].address = 0x9A4524CE05;

  
  int i;
  for(i=0;i<6;i++){
    RxPipeConfig[i].PipeEnabled = 1;
    RxPipeConfig[i].autoAckEnabled = 1;
    RxPipeConfig[i].dynamicPayloadEnabled = 1;
  }
  
  
  ResetConfigValues(&RadioConfig, RxPipeConfig);
}







void radio_thread( void *pvParameters ){
	
  RadioReset();

  char myMesg[32];
  Payload_t payload;
  
  payload.UseAck = 1;
  

  payload.address = 0x11223344EE;
  payload.data = (uint8_t*)myMesg;
  payload.length = strlen(myMesg);
  payload.retransmitCount = 15;
  
  
	while(1){
		if(readable()){
			uint8_t RxData[32];
			Payload_t payload;
			payload.data = RxData;
          
			clear_data_ready_flag();
			readPayload(&payload);
			payload.data[payload.length] = '\0';
			command_parse_execute(payload.data);

			flush_rx();

		}
    
		vTaskDelay(200);
	}
  

	
}

	
void relay_thread( void *pvParameters ){	
	DigitalPin_Config(&button[0],0,0,PORT_OPC_PULLUP_gc,PORT_ISC_INPUT_DISABLE_gc);
	DigitalPin_Config(&button[1],0,0,PORT_OPC_PULLUP_gc,PORT_ISC_INPUT_DISABLE_gc);
	DigitalPin_Config(&button[2],0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);
	DigitalPin_Config(&button[3],0,0,PORT_OPC_PULLUP_gc,PORT_ISC_BOTHEDGES_gc);
	
	for(int i=0;i<4;i++){
		DigitalPin_SetValue(&relay[i]);
		DigitalPin_SetDir(&relay[i],1);
	}
	
	
	while(1){
		vTaskDelay(100);
		
		for(int i=0;i<4;i++){
			if( !DigitalPin_GetValue(&button[i] )  ){
				while(!DigitalPin_GetValue(&button[i]));
				relayState[i] =!relayState[i];
			}
		}
		
		for(int i=0;i<4;i++){
			if( relayState[i] ){
				DigitalPin_SetValue(&relay[i]);
			}
			else{
				DigitalPin_ClearValue(&relay[i]);
			}
		}
		
		
	}
	
}
	
	
void glcd_thread( void *pvParameters ){
	ssd1306_init();
	ssd1306_clear();

	char glcd_line[4][30];

	while(1){

		sprintf(glcd_line[0],"Light 0 :   %d",!relayState[0]);
		sprintf(glcd_line[1],"Light 1 :   %d",!relayState[1]);
		sprintf(glcd_line[2],"Light 2 :   %d",!relayState[2]);
		sprintf(glcd_line[3],"Fan      :   %d",!relayState[3]);



		ssd1306_clear();


		for(int i=0;i<4;i++){
			ssd1306_set_page_address(i);
			ssd1306_set_column_address(0);
			ssd1306_write_text(glcd_line[i]);
		}

		if(radioRXnotificationLed){
			DigitalPin_ClearValue(&led1);
			vTaskDelay(200);
			DigitalPin_SetValue(&led1);
			radioRXnotificationLed = 0;
		}
		else{
			vTaskDelay(200);
		}

	}
}
	
	
/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{


	board_config();
	
	xTaskCreate(radio_thread,(signed portCHAR *) "t1", 500, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(relay_thread,(signed portCHAR *) "t2", 500, NULL, tskIDLE_PRIORITY, NULL );
	xTaskCreate(glcd_thread,(signed portCHAR *) "t3", 500, NULL, tskIDLE_PRIORITY, NULL );
	
	vTaskStartScheduler();


	while(1)
	{

	}
}