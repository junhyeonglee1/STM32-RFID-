/*
 * i2c_lcd.c
 *
 *  Created on: 2019. 9. 4.
 *      Author: k
 */
#include "main.h"

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "i2c_lcd.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart3;

extern char rfid_state;
extern void readTime(uint8_t *time);
extern uint8_t time[3];

const char* weekdays[] = {
    "SUN", // 일요일
    "MON", // 월요일
    "TUE", // 화요일
    "WED", // 수요일
    "THU", // 목요일
    "FRI", // 금요일
    "SAT"  // 토요일
};

extern char rfid_state;

void i2c_lcd_ds1302(void)
{
	char lcd_buff[40];
	char lcd_buff1[40];

	readTime(time);
	sprintf(lcd_buff, "%02x%02x%02x",time[6],time[4],time[3]);
	move_cursor(0,0);
	lcd_string(lcd_buff);

	move_cursor(0,8);
	lcd_string(weekdays[time[5]-1]);

	sprintf(lcd_buff1, "%02x:%02x:%02x ",time[2], time[1], time[0]);
	move_cursor(1,0);
	lcd_string(lcd_buff1);

	move_cursor(1,10);
	switch(rfid_state)
	{
	case IDLE: lcd_string("IDLE  ");
	break;
	case REGIST: lcd_string("REGIST");
	break;
	case DELETE : lcd_string("DELETE");
	break;
	}

}

void i2c_lcd_main(void){

	uint8_t value=0;
	i2c_lcd_init();


	while(1){
		move_cursor(0,0);
		lcd_string("Hello World!!!");
		move_cursor(1,0);
		lcd_data(value + '0');
		value++;
		if(value>9)value=0;
		HAL_Delay(500);
	}
}

void lcd_command(uint8_t command){

	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = command & 0xf0;
	low_nibble = (command<<4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,
			i2c_buffer, 4, 100)!=HAL_OK){
		//HAL_Delay(1);
	}
	return;
}
void lcd_data(uint8_t data){

	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];
	high_nibble = data & 0xf0;
	low_nibble = (data<<4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
	while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS,
			i2c_buffer, 4, 100)!=HAL_OK){
		//HAL_Delay(1);
	}
	return;
}
void i2c_lcd_init(void){

	lcd_command(0x33);
	lcd_command(0x32);
	lcd_command(0x28);	//Function Set 4-bit mode
	lcd_command(DISPLAY_ON);
	lcd_command(0x06);	//Entry mode set
	lcd_command(CLEAR_DISPLAY);
	HAL_Delay(2);
}
void lcd_string(uint8_t *str){
	while(*str)lcd_data(*str++);
}
void move_cursor(uint8_t row, uint8_t column){
	lcd_command(0x80 | row<<6 | column);
	return;
}











