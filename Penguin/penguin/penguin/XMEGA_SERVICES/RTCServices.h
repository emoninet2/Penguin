/*
 * RTCServices.h
 *
 * Created: 09-May-16 10:07:58 AM
 *  Author: emon1
 */ 


#ifndef RTCSERVICES_H_
#define RTCSERVICES_H_

#include "xmega_drivers.h"
#include "time.h"


#define USE_RTC_TICK	1


#define RTC_PRESCALER_VAL RTC_PRESCALER_DIV1_gc
#define RTC_PER_VAL 32
#define RTC_TICK_FREQ 32768/RTC_PER_VAL/(1<<(RTC_PRESCALER_VAL-1))
#define RTC_TICK_TIME 1/RTC_TICK_FREQ

#define rtc_ms_ticks(ms)  ms*RTC_TICK_FREQ/1000








/*
 *	initialize the rtc
 */
void rtc_initialize();



/*
 *	ms wait until
 */
void rtc_ms_delay(int ms);



#endif /* RTCSERVICES_H_ */