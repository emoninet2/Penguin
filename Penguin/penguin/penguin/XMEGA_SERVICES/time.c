/*
 * (c)2012 Michael Duane Rice All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions
 * and the following disclaimer in the documentation and/or other materials
 * provided with the distribution. Neither the name of the copyright holders
 * nor the names of contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* $Id$ */

/*
	Standard time() function. Copying from __system_time must be atomic, since it
	may be incremented at interrupt time.
*/
#include "time.h"
#include <inttypes.h>

extern volatile time_t __system_time;

time_t
time(time_t * timer)
{
	time_t          ret;

	asm             volatile(
			                   "in __tmp_reg__, __SREG__" "\n\t"
				                 "cli" "\n\t"
				 ::
	);
	ret = __system_time;
	asm             volatile(
			                  "out __SREG__, __tmp_reg__" "\n\t"
				 ::
	);
	if (timer)
		*timer = ret;
	return ret;
}


static char monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};
	
char * __month[]={"Jan","Feb","Mar","Apr","May","Jun",
"Jul","Aug","Sep","Oct","Nov","Dec"};
	
char * __day[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

	
static char ascTimeBuffer[32];


void  system_tick(void){
	
	__system_time++;
	
}


void  set_system_time(time_t timestamp){
	__system_time = timestamp;
}


// validate the tm structure
static void CheckTime(struct tm *timeptr) {
	// we could do some normalization here, e.g.
	// change 40 october to 9 november
	#if !__TIME_UNSIGNED
	if (timeptr->tm_sec<0) timeptr->tm_sec=0;
	if (timeptr->tm_min<0) timeptr->tm_min=0;
	if (timeptr->tm_hour<0) timeptr->tm_hour=0;
	if (timeptr->tm_wday<0) timeptr->tm_wday=0;
	if (timeptr->tm_mon<0) timeptr->tm_mon=0;
	#endif
	
	if (timeptr->tm_sec>59) timeptr->tm_sec=59;
	if (timeptr->tm_min>59) timeptr->tm_min=59;
	if (timeptr->tm_hour>23) timeptr->tm_hour=23;
	if (timeptr->tm_wday>6) timeptr->tm_wday=6;
	if (timeptr->tm_mday<1) timeptr->tm_mday=1;
	else if (timeptr->tm_mday>31) timeptr->tm_mday=31;
	if (timeptr->tm_mon>11) timeptr->tm_mon=11;
	if (timeptr->tm_year<0) timeptr->tm_year=0;
}

//char           *asctime(const struct tm * timeptr);
// format the time into "Sat Feb 17 17:45:23 2001\n"
 char *asctime(const struct tm *timeptr) {
   CheckTime(timeptr);
   sprintf (ascTimeBuffer, "%s %s %2d %02d:%02d:%02d %04d\n",
            __day[timeptr->tm_wday], __month[timeptr->tm_mon], timeptr->tm_mday,timeptr->tm_year+1900);
   return ascTimeBuffer;
 }
 
 char *ctime(const time_t *timep) {
   return asctime(localtime(timep));
 }
 
 static struct tm lastTime;
 
 /* convert calendar time (seconds since 1970) to broken-time
    This only works for dates between 01-01-1970 00:00:00 and 
    19-01-2038 03:14:07
 
    A leap year is ((((year%4)==0) && ((year%100)!=0)) || ((year%400)==0)) 
    but since we have no fancy years between 1970 and 2038 we can do:
 */
 
 #define LEAP_YEAR(year) ((year%4)==0)
 
 // forget about timezones for now
 struct tm *localtime(const time_t *timep) {
     return gmtime(timep);
 }
 
 struct tm *gmtime(const time_t *timep) {
   unsigned long epoch=*timep;
   unsigned int year;
   unsigned char month, monthLength;
   unsigned long days;
   
   lastTime.tm_sec=epoch%60;
   epoch/=60; // now it is minutes
   lastTime.tm_min=epoch%60;
   epoch/=60; // now it is hours
   lastTime.tm_hour=epoch%24;
   epoch/=24; // now it is days
   lastTime.tm_wday=(epoch+4)%7;
   
   year=1970;
   days=0;
   while((days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch) {
     year++;
   }
   lastTime.tm_year=year-1900;
   
   days -= LEAP_YEAR(year) ? 366 : 365;
   epoch -= days; // now it is days in this year, starting at 0
   lastTime.tm_yday=epoch;
   
   days=0;
   month=0;
   monthLength=0;
   for (month=0; month<12; month++) {
     if (month==1) { // februari
       if (LEAP_YEAR(year)) {
         monthLength=29;
       } else 
	   
	   {
	   
	   }
	   
     } else 
	 {
       monthLength = monthDays[month];
     	}
     
     if (epoch>=monthLength) 
	 	{
       epoch-=monthLength;
     	} else 
		
		{
         break;
     }
   }
   lastTime.tm_mon=month;
   lastTime.tm_mday=epoch+1;
   
   lastTime.tm_isdst=0;
   
   return &lastTime;
 }
 
 // convert broken time to calendar time (seconds since 1970)
 time_t mktime(struct tm *timeptr) {
     int year=timeptr->tm_year+1900, month=timeptr->tm_mon, i;
     long seconds;
     
     CheckTime(timeptr);
 
     // seconds from 1970 till 1 jan 00:00:00 this year
     seconds= (year-1970)*(60*60*24L*365);
 
     // add extra days for leap years
     for (i=1970; i<year; i++) {
         if (LEAP_YEAR(i)) {
             seconds+= 60*60*24L;
         }
     }
 
     // add days for this year
     for (i=0; i<month; i++) {
       if (i==1 && LEAP_YEAR(year)) { 
         seconds+= 60*60*24L*29;
       } else {
         seconds+= 60*60*24L*monthDays[i];
       }
     }
 
     seconds+= (timeptr->tm_mday-1)*60*60*24L;
     seconds+= timeptr->tm_hour*60*60;
     seconds+= timeptr->tm_min*60;
     seconds+= timeptr->tm_sec;
     return seconds;
 }