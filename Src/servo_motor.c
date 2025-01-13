#include "servo_motor.h"

extern TIM_HandleTypeDef htim3;
extern volatile int TIM2_servo_motor_counter;
char servomotor_state = 0;

void servo_motor_main(void);
void servo_motor_main2(void);
/*
 *  동작주파수 : 84000000hz
	timer3: 50000hz 입력 => 84000000/1680 = 50000

	t = 1/50000 sec = 0.00002sec(20us)
	1 sec = 50000x 0.00002
	20ms = 0.02sec = 20us x 1000개 count
	180도 -> 2ms = 100 x 20us
	90도  -> 1.5ms = 75 x 20us
	0도   -> 1ms = 50 x 20us
 */

void servo_motor_main2(void)
{
	switch(servomotor_state)
	{
	case 0:
		//led_all_off();
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 35);
		break;
	case 1:
		//led_all_on();
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 79); //90도
		if(TIM2_servo_motor_counter >= 5000)
		{
			TIM2_servo_motor_counter = 0;
			servomotor_state = 0;
		}
	default:
		break;
	}
}

void servo_motor_rfid_main(void)
{
	if(servomotor_state == 1)
	{
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 79);
		if(TIM2_servo_motor_counter >= 1000)
		{
			servomotor_state = 0;
		}
	}
	else if (servomotor_state == 0)
	{
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 35);
	}
}

void servo_motor_main(void)
{
	static uint8_t servo_state = 0;

	if(TIM2_servo_motor_counter >= 1000) // 1sec
	{
		TIM2_servo_motor_counter = 0;
		// 180도 -> 90도 -> 0도
		switch(servo_state)
		{
		case 0:
			// 1. 180도 회전
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 125);
			break;
		case 1:
			// 2. 90도 회전
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 79);
			break;
		case 2:// 3. 0도 회전
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 35);
			break;
		}
	}
}
