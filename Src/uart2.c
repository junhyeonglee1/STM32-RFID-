#include "uart2.h"

extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;
extern uint8_t time[8];
void pc_command_processing(void);
extern void set_clock(char *date_time);
uint8_t rx_buff[20][40];
volatile static int front = 0;
volatile static int rear = 0;
// move from Drivers\STM32F4xx_HAL_Driver\Src\HAL_UART_RxCpltCallback of stm32f4xx_hal_uart to here
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static int i = 0;

	if(huart == &huart2)
	{
		unsigned char data;
		data = rx_data;
		if(data == '\r' || data =='\n')
		{
			rx_buff[rear][i] = '\0';
			i=0;
			rear++;
			rear %= 20;
		}
		else
		{
			rx_buff[rear][i++] = data;
		}
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);
	}
}

void pc_command_processing(void)
{
	if(front != rear)
	{
		printf("%s\n",rx_buff[front]); //rx_buff[front]0번째방의 주소
		if(strncmp(rx_buff[front],"settime",strlen("settime"))==0)
		{
			set_clock(&rx_buff[front][8]);
		}
		front++;
		front %= 20;
	}
}

