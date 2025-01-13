/*
 * button.c
 *
 * Created: 2024-04-25 오후 1:30:31
 *  Author: kccistc
 */ 
#include "button.h" 

unsigned char button_status[BUTTON_NUMBER] =
	{BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE,BUTTON_RELEASE};

char rfid_state = IDLE;
char lcd_state = 0;
void button0_check(void)
{
	// 버튼을 한번 울렀다 떼었는가 ?
	if (get_button(BUTTON0_GPIO_Port, BUTTON0_Pin, 0)== BUTTON_PRESS)
	{
		rfid_state = IDLE;
	}
	else if (get_button(BUTTON2_GPIO_Port, BUTTON2_Pin, 2)== BUTTON_PRESS)
	{
		rfid_state = REGIST;
	}
	else if (get_button(BUTTON3_GPIO_Port, BUTTON3_Pin, 3)== BUTTON_PRESS)
	{
		rfid_state = DELETE;
	}
}


// 버튼이 온전하게 눌렸다 떼어진 상태 이면 1을 리턴 한다. 
int get_button(GPIO_TypeDef  *GPIO, int button_pin, int button_num)  // 예) GPIOC BUTTON0 4
{         
	int current_state; 

	current_state = HAL_GPIO_ReadPin(GPIO, button_pin);  //  active low press:0 , Release:1 버튼값을 읽는다.
	if (current_state == BUTTON_PRESS && button_status[button_num]==BUTTON_RELEASE)  // 버튼이 처음 눌려진상태
	{
		HAL_Delay(60);   // 노이즈가 지나가기를 기다린다
		button_status[button_num]=BUTTON_PRESS;  // 처음 눌려진 상태가 아니다
		return BUTTON_RELEASE;        // 아직은 완전히 눌렸다 떼어진 상태가 아니다.
	}     
	else if (button_status[button_num]==BUTTON_PRESS && current_state==BUTTON_RELEASE)
	       // 버튼이 이전에 눌러진 상태였으며 현재는 떼어진 상태
	{
		button_status[button_num]=BUTTON_RELEASE;   // 다음 버튼 상태를 체크하기위해 초기화
		HAL_Delay(60);   // 노이즈가 지나가기를 기다린다
	    return BUTTON_PRESS;    // 완전히 눌렸다 떼어진 상태임을 리턴
	}
	 
	 return BUTTON_RELEASE;   // 아직 완전히 스위치를 눌렀다 뗀 상태가 아닌경우 나
	             // 스위치가 open 된 상태                   	
}
