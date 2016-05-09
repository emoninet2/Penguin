
/*
 * Serial.h
 *
 * Created: 12-Mar-16 12:20:46 AM
 *  Author: emon1
 */ 


#ifndef STREAM_H_
#define STREAM_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#define create_serial_stream(streamName, putcharfunc, getcharfunc) \
int streamName ## _putchar(char ch, FILE *stream){\
	if(ch=='\n')\
	streamName ## _putchar('\r',stream);\
	putcharfunc((char) ch);\
	return 0;\
}\
\
int streamName ## _getchar(FILE *stream)\
{\
	char ch = getcharfunc();\
	streamName ## _putchar(ch,stream);\
	return ch;\
}\
FILE *streamName = fdevopen(streamName ## _putchar,streamName ## _getchar);

#ifdef __cplusplus
}
#endif

#endif /* STREAM_H_ */

