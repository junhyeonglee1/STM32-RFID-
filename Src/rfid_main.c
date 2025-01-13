#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mfrc522.h"    // for RFID

uint8_t rfid_check_flag=0;  // 현재 rfid를 checking중인지
uint8_t buzzer_start_flag=0; // 부저 start/stop indicator flag
uint8_t door_open_flag=0;    // 서보 모터 가동

#define CARD_SU  20
extern SPI_HandleTypeDef hspi1;   // for RFID interface
extern TIM_HandleTypeDef htim3;  // for servo motor interface
extern char rfid_state;
volatile int T300ms_counter = 0;
extern char servomotor_state;
uint8_t readData;   // 1 byte save variable
uint8_t rxDataStr[MAX_LEN];   // rfid tagging data
uint8_t cardNotFound=0;       // rfid card found ?
uint8_t regCardKey[CARD_SU][5] =
{0
};
extern char lcd_state;
void rfid_reader_init(void);
void rfid_tag_processing_servo(void);
// RFID READER 초기화 함수
void rfid_reader_init(void)
{
	mfrc522_init(&hspi1);
	HAL_Delay(1000);    // wait for RFID init

	readData = mfrc522_read(VersionReg);
	if (readData == 0x92)
	{
		printf("MIFARE RC522v2\n");
		printf("Detected !!!\n");
	}
	else if (readData == 0x91 || readData == 0x90)
	{
		printf("MIFARE RC522v1\n");
		printf("Detected !!!\n");
	}
	else
	{
		printf("No RFID Reader found !!!\n");
	}
}
/*
 * switch(servo_state)
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
 */
extern volatile int TIM2_servo_motor_counter;
extern int get_button(GPIO_TypeDef  *GPIO, int button_pin, int button_num);
// RFID tagging 처리 함수
#if 1

void rfid_tag_account_check(void)
{
	if(get_button(BUTTON1_GPIO_Port, BUTTON1_Pin, 1)== 0)
	{
		printf("ALL_ACOUNT print\n");
		for(int j=0;j < CARD_SU; j++)
		{
			printf("num%d : %2x %2x %2x %2x %2x\n",j ,regCardKey[j][0],regCardKey[j][1],regCardKey[j][2],regCardKey[j][3],regCardKey[j][4]);
		}
	}
}
void rfid_tag_processing_servo(void)
{
	static int count = 0;
	if (T300ms_counter >= 300)  // polling 300ms rfid reader
	{
		T300ms_counter=0;
		if (!rfid_check_flag)   // 현재 RFID를 checking중이 아니면
		{
			readData = mfrc522_request(PICC_REQALL, rxDataStr);
			// RFID contack check
			if (readData == CARD_FOUND)
			{

				for (int i=0; i < MAX_LEN; i++)
					rxDataStr[i]=' ';

				readData = mfrc522_get_card_serial(rxDataStr);
				for (int i=0; i < 5; i++)
				{
					printf("%02x ", rxDataStr[i]);
				}
				printf("\n");
				switch(rfid_state)
				{
				case IDLE:
					for(int k=0;k < CARD_SU; k++)
					{
						if(memcmp(rxDataStr, regCardKey[k], 5)==0)
						{
							printf("PASS\n");
							TIM2_servo_motor_counter=0;
							servomotor_state = 1;
							break;
						}
						else;
						if(k==CARD_SU-1)
						{
							printf("NOT REGISTED!\n");
						}
					}
					break;
				case REGIST:
					for(int x=0;x < CARD_SU;x++)
					{
						if(memcmp(rxDataStr, regCardKey[x], 5)== 0)
						{
							printf("already registed!!\n");
							break;
						}
						if(x == CARD_SU-1)
						{
							memcpy(regCardKey[count], rxDataStr, 5);
							count++;
							printf("REGIST COMPLETE!\n");
						}
					}
					break;
				case DELETE:
					for(int b=0;b < CARD_SU; b++)
					{
						if(memcmp(rxDataStr, regCardKey[b], 5)== 0)
						{
							for(int n=0;n<5;n++)
							{
								regCardKey[b][n]=0;
							}
							printf("DELETED!!!\n");
							break;
						}
						else;
					}
					break;
				default:
					break;
				}

			}
		}
	}

}
#endif
//------- start RFID DEMO ----------------
#if 1
void rfid_control_main(void)
{
printf("rfid_control_main start !!!!\n");

	rfid_reader_init();

	while(1)
	{
		readData = mfrc522_request(PICC_REQALL, rxDataStr);
		// RFID contack check
		if (readData == CARD_FOUND)
		{

			for (int i=0; i < MAX_LEN; i++)
				rxDataStr[i]=' ';
			readData = mfrc522_get_card_serial(rxDataStr);
			for (int i=0; i < 5; i++)
			{
				printf("%02x ", rxDataStr[i]);
			}

//			for (int i=0; i < CARD_SU; i++)
//			{
//				if (strncmp(rxDataStr,regCardKey[i],5) == 0)
//				{
//					for (int i=0; i < 5; i++)
//					{
//						printf("%02x ", rxDataStr[i]);
//					}
//					printf("Valid Card !!!\n");
//					printf("DOOR Opened !!!\n");
//					__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 100); //2ms 100(2ms)/1000(20ms) :180degree
//					HAL_Delay(2000);
//
//					__HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 50);  //1ms :180degree
//					HAL_Delay(1000);
//					printf("DOOR Closed !!!\n");
//
//					cardNotFound=0;
//					break;
//				}
//				cardNotFound=1;
//			}
//			if (cardNotFound)
//			{
//				for (int i=0; i < 5; i++)
//				{
//					printf("%02x ", rxDataStr[i]);
//				}
//				printf("Invalid Card !!!\n");
//			}
		}
		HAL_Delay(1000);
	}
}
#endif
//-------- end RFID DEMO -----------------
