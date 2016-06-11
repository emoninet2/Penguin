/*
 * lcd03.h
 *
 * Created: 11-Jun-16 9:39:56 PM
 *  Author: emon1
 */ 


#ifndef LCD03_H_
#define LCD03_H_

#include "lcd03_arch_driver.h"

typedef enum LCD03_COMM_MODE_ENUM{
	LCD03_I2C,
	LCD03_SERIAL,
}COMM_MODE_t;
typedef enum LCD03_SIZE_ENUM{
	LCD03_16_2,
	LCD03_20_4,
}LCD_SIZE_t;
typedef enum LCD03_COMMAND_ENUM{
	LCD03_CMD_NULL = 0,
	LCD03_CMD_CURSOR_HOME = 1,
	LCD03_CMD_SET_CURSOR_POS = 2,
	LCD03_CMD_SET_CURSOR_COORDINATE = 3,
	LCD03_CMD_HIDE_CURSOR = 4,
	LCD03_CMD_SHOW_UNDERLINE_CURSOR = 5,
	LCD03_CMD_SHOW_BLINKING_CURSOR = 6,
	LCD03_CMD_BACKSPACE = 8,
	LCD03_CMD_HORIZONTAL_TAB = 9,
	LCD03_CMD_SMART_LINE_FEED = 10,
	LCD03_CMD_VERTICAL_TAB = 11,
	LCD03_CMD_CLEAR_SCREEN  = 12,
	LCD03_CMD_CARRIAGE_RETURN = 13,
	LCD03_CMD_CLEAR_COLUMN = 17,
	LCD03_CMD_TAB_SET = 18,
	LCD03_CMD_BACKLIGHT_ON = 19,
	LCD03_CMD_BACKLIGHT_OFF = 20,
	LCD03_CMD_DISABLE_STARTUP_MESSAGE = 21,
	LCD03_CMD_ENABLE_STARTUP_MESSAGE = 22,
	LCD03_CMD_CHANGE_ADDRESS = 25,
	LCD03_CMD_CUSTOM_CHAR_GENERATOR = 27,
	LCD03_CMD_DOUBLE_KEYPAD_SCAN_RATE = 28,
	LCD03_CMD_NORMAL_KEYPAD_SCAN_RATE = 29,
	LCD03_CMD_FIFO_STATUS = 14,
	LCD03_CMD_SOFTWARE_VERSION = 15,
	LCD03_CMD_KEYPAD_STATUS = 16,
}LCD03_COMMAND_t;
typedef enum LCD03_CURSOR_DISPLAY_ENUM{
	LCD03_CURSOR_HIDE,
	LCD03_CURSOR_UNDERLINE,
	LCD03_CURSOR_BLINKING,
}LCD03_CURSOR_DISP_t;
typedef enum LCD03_I2C_ADDRESS_ENUM{
	LCD03_I2C_ADDRESS_0xC6 = 0xc6,
	LCD03_I2C_ADDRESS_0xc8 = 0xc8,
	LCD03_I2C_ADDRESS_0xca = 0xca,
	LCD03_I2C_ADDRESS_0xcc = 0xcc,
	LCD03_I2C_ADDRESS_0xce = 0xce,
}LCD03_I2C_ADDRESS_t;



/**
	* i2c transcieve
	*/
void lcd03_init(uint8_t data);
/**
	* i2c transcieve
	*/
void lcd03_send_via_i2c(uint8_t data);
/**
	* serial transcieve
	*/
void lcd03_send_via_serial(uint8_t data);
/**
	* send command
	*/
void lcd03_send_command(LCD03_COMMAND_t cmd);
/**
	* send data
	*/
void lcd03_send_data(uint8_t data);
/**
	* set the cursor to the home position
	*/
void lcd03_cursor_home();
/**
	* set the cursor position
	* @param pos the number (1 through 80) on the lcd
	*/
void lcd03_set_cursor_pos(uint8_t pos);
/**
	* set the cursor to coordinates based on line no and colomn no
	* @param line the line number
	* @param col the column number
	*/
void lcd03_set_cursor_coordinate(uint8_t line, uint8_t col);
/**
	* the mode for the cursor display to either hide, underline or blink
	* @param mode the mode of cursor display
	*/
void lcd03_cursor_display_mode(LCD03_CURSOR_DISP_t mode);
/**
	* backspace
	*/
void lcd03_backspace();
/**
	* tab
	*/
void lcd03_tab();
/**
	* smart line feed: move the cursor down one line to the position beneath the same column
	*/
void lcd03_smart_line_feed();
/**
	* vertical tab
	*/
void lcd03_vertical_tab();
/**
	* clear screen
	*/
void lcd03_clear_screen();
/**
	* carriage return, move the cursor to the start of the next line
	*/
void lcd03_carriage_return();
/**
	* clear column
	*/
void lcd03_clear_column();
/**
	* tab set size, set the required tab size
	* @param size any size between 1 and 10
	*/
void lcd03_tab_set_size(uint8_t size);
/**
	* back light 
	* @param mode 0-> backlight off, 1-> backlight on
	*/
void lcd03_backlight(bool mode);
/**
	* startup message mode, enable or disable the startup message
	* @param mode 0-> disable, 1-> enable
	*/
void lcd03_startup_message_mode(bool mode);
/**
	* change address of i2c device
	*/
void lcd03_change_i2c_addr(LCD03_I2C_ADDRESS_t addr);
/**
	* custom character generator
	*/
void lcd03_custom_char_ger();
/**
	* get_fifo_status
	* return fifo status
	*/
uint8_t lcd03_get_fifo_status();
/**
	* software version
	*/
uint8_t lcd03_get_software_version();
/**
	* print string
	* @param string_addr
	* @param len length of the string
	*/
void lcd03_print_string(char *strn_add, int len);
/**
	* print string
	* @param string_addr
	* @param len length of the string
	*/
void lcd03_custom_char_ger();

#endif /* LCD03_H_ */