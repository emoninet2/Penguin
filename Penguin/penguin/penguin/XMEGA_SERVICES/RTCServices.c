/*
 * RTCServices.c
 *
 * Created: 09-May-16 10:08:11 AM
 *  Author: emon1
 */ 


#include "RTCServices.h"





#if (USE_RTC_TICK == 1)
typedef volatile int rtctick_t;
volatile rtctick_t rtc_tick_global = 0;
volatile int rtc_global_ms = 0 ;


#endif





/*
 *	initialize the rtc
 */
void rtc_initialize(){
	//enabling interrupt all three level, high, mid, and low
	OSC.RC32KCAL = 0x10;//smaller, the faster the clock
	PMIC_CTRL |= (7<<0);
	//selecting clock source 32.768kHz from 32.768kHz crystal oscillator on TOSC, enabling RTC clock source
	CLK_RTCCTRL = (5<<1) | (1<<0);//crystal oscillator on TOSC| enabling clock
	//enabling interrupts for overflow
	RTC_INTCTRL	|= (0b00000001);
	RTC_PER = RTC_PER_VAL; //period_val  = 2
	//selecting clock prescaling
	RTC_CTRL = (1<<0);//no prescaling
	sei();
	
	
}


/*
 *	interrupt service routine for overflow interrupt event
 */
ISR(RTC_OVF_vect)
{
	#if (USE_RTC_TICK == 1)
	rtc_tick_global++;
	if(rtc_tick_global>=256){
		rtc_tick_global = 0;
		system_tick();
	}	
	#endif
	
	
	
	
}


/*
 *	interrupt service routine for compare interrupt event
 */
ISR(RTC_COMP_vect)
{


}
