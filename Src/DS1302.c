#include "DS1302.h"
#include "main.h"

uint8_t decToformat(uint8_t c)
{
  return (c/10)<<4 | (c%10);
}

void set_clock(char *date_time, uint8_t *time);

void commandWrite(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O,uint8_t value)
{
  /* I_O PORT OUTPUT SETTING */

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin_I_O;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIO_I_O, &GPIO_InitStruct);
  int i;

  /* WRITE SETTING */
  for(i=0;i<8;i++) //LSB
  {
     if( ((value>>i) & 0x01))
       HAL_GPIO_WritePin(GPIO_I_O, GPIO_Pin_I_O, 1);
     else
       HAL_GPIO_WritePin(GPIO_I_O, GPIO_Pin_I_O, 0);
     HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 1);
     HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 0);
  }
}

uint8_t commandRead(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O)
{
  /* I_O PORT INPUT SETTING */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_GPIO_WritePin(GPIO_I_O, GPIO_Pin_I_O, 0);
  GPIO_InitStruct.Pin = GPIO_Pin_I_O;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIO_I_O, &GPIO_InitStruct);
  uint8_t value=0;
  uint8_t currentBit = 0;
  int i;

  /* INPUT SETTING */
  for(i=0;i<8;i++)
  {
    currentBit = HAL_GPIO_ReadPin(GPIO_I_O, GPIO_Pin_I_O);
    value |= (currentBit<<i);
    HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 1);
    HAL_GPIO_WritePin(GPIO_SCLK, GPIO_Pin_SCLK, 0);
  }
  return value;
}

void DS1302_HALT(GPIO_TypeDef *GPIO_SCLK, uint16_t GPIO_Pin_SCLK, GPIO_TypeDef *GPIO_I_O, uint16_t GPIO_Pin_I_O, GPIO_TypeDef *GPIO_CE, uint16_t GPIO_Pin_CE, uint8_t flag)
{
  HAL_GPIO_WritePin(GPIO_CE, GPIO_Pin_CE, 1);
  commandWrite(GPIO_SCLK, GPIO_Pin_SCLK, GPIO_I_O, GPIO_Pin_I_O, 0x80);
  commandWrite(GPIO_SCLK, GPIO_Pin_SCLK, GPIO_I_O, GPIO_Pin_I_O, 0x00 | flag<<7);
  HAL_GPIO_WritePin(GPIO_CE, GPIO_Pin_CE, 0);
}

void readTime(uint8_t *time)
{
  //array : 0: seconds, 1: minutes, 2: hours

  //seconds read
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x81);
  time[0] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  // minutes read
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x83);
  time[1] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  // hours read
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x85);
  time[2] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  //Dates
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x87);
  time[3] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  //Month
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x89);
  time[4] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  //Day
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x8B);
  time[5] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  //year
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x8D);
  time[6] = commandRead(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
}

void setTime(uint8_t year, uint8_t month, uint8_t date, uint8_t day,uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  //second
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x80);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(seconds));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
  //minutes
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x82);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(minutes));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
  //hours
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x84);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(hours));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
  //Dates
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x86);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(date));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  //Month
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x88);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(month));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);

  //Day
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x8A);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(day));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
  //year
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, 0x8C);
  commandWrite(SCLK_GPIO_Port, SCLK_Pin, I_O_GPIO_Port, I_O_Pin, decToformat(year));
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0);
}

void set_clock(char *date_time, uint8_t *time)
{
	char yy[4], mm[4], dd[4]; // date
	char hh[4], min[4], ss[4]; //time
	char day[2];
	strncpy(yy,date_time,2);
	strncpy(mm,date_time+2,2); //date_time+2 == &date_time[2]
	strncpy(dd,date_time+4,2);

	strncpy(hh,date_time+6,2);
	strncpy(min,date_time+8,2);
	strncpy(ss,date_time+10,2);
	strncpy(day,date_time+12,1);
	//1. ascii to int 2. int to BCD 3. RTC적용

	setTime(atoi(yy), atoi(mm), atoi(dd), atoi(day), atoi(hh), atoi(min), atoi(ss));
}
