/*
 * lcd03.c
 *
 * Created: 11-Jun-16 9:40:05 PM
 *  Author: emon1
 */ 

#include "lcd03.h"

void (*psend) (uint8_t data);




void lcd03_init(uint8_t data){
	lcd03_arch_init();
}
void lcd03_send_via_i2c(uint8_t data){
	lcd03_arch_i2c_send(data, 0x64);
}
void lcd03_send_via_serial(uint8_t data){
	
}
void lcd03_send_command(LCD03_COMMAND_t cmd){
	//(*psend) (cmd);
	lcd03_arch_i2c_send(cmd, 0x64);
}
void lcd03_send_data(uint8_t data){
	(*psend) (data);
}
void lcd03_cursor_home(){
	lcd03_send_command(LCD03_CMD_CURSOR_HOME);
}
void lcd03_set_cursor_pos(uint8_t pos){
	lcd03_send_command(LCD03_CMD_SET_CURSOR_POS);
}
void lcd03_set_cursor_coordinate(uint8_t line, uint8_t col){
	lcd03_send_command(LCD03_CMD_SET_CURSOR_COORDINATE);
}
void lcd03_cursor_display_mode(LCD03_CURSOR_DISP_t _nrf24l01p_RxTxMode){
	switch(_nrf24l01p_RxTxMode){
		case 	LCD03_CURSOR_HIDE		:	lcd03_send_command(LCD03_CMD_HIDE_CURSOR);break;
		case	LCD03_CURSOR_UNDERLINE	:	lcd03_send_command(LCD03_CMD_SHOW_UNDERLINE_CURSOR);break;
		case	LCD03_CURSOR_BLINKING	:	lcd03_send_command(LCD03_CMD_SHOW_BLINKING_CURSOR);break;
		default							:	break;
	}
}
void lcd03_backspace(){
	lcd03_send_command(LCD03_CMD_BACKSPACE);
}
void lcd03_tab(){
	lcd03_send_command(LCD03_CMD_HORIZONTAL_TAB);
}
void lcd03_smart_line_feed(){
	lcd03_send_command(LCD03_CMD_SMART_LINE_FEED);
}
void lcd03_vertical_tab(){
	lcd03_send_command(LCD03_CMD_VERTICAL_TAB);
}
void lcd03_clear_screen(){
	lcd03_send_command(LCD03_CMD_CLEAR_SCREEN);
}
void lcd03_carriage_return(){
	lcd03_send_command(LCD03_CMD_CARRIAGE_RETURN);
}
void lcd03_clear_column(){
	lcd03_send_command(LCD03_CMD_CLEAR_COLUMN);
}
void lcd03_tab_set_size(uint8_t size){
	lcd03_send_command(LCD03_CMD_TAB_SET);
	lcd03_send_data(size);
}
void lcd03_backlight(bool _nrf24l01p_RxTxMode){
	if(_nrf24l01p_RxTxMode) {lcd03_send_command(LCD03_CMD_BACKLIGHT_ON);}
	else {lcd03_send_command(LCD03_CMD_BACKLIGHT_OFF);}
}
void lcd03_startup_message_mode(bool _nrf24l01p_RxTxMode){
	if(_nrf24l01p_RxTxMode) {lcd03_send_command(LCD03_CMD_ENABLE_STARTUP_MESSAGE);}
	else {lcd03_send_command(LCD03_CMD_DISABLE_STARTUP_MESSAGE);}
}
void lcd03_change_i2c_addr(LCD03_I2C_ADDRESS_t addr){
	if(psend == (&lcd03_send_via_i2c)){
		lcd03_send_command(LCD03_CMD_CHANGE_ADDRESS);
		lcd03_send_data(0xA0);
		lcd03_send_data(0xAA);
		lcd03_send_data(0xA5);
		lcd03_send_data(addr);
	}
}
uint8_t lcd03_get_fifo_status(){}
uint8_t lcd03_get_software_version(){}
void lcd03_custom_char_ger(){}