/*
 * spi.c
 */

#include "spi.h"

SPI_HandleTypeDef *hspi;

void spi_init(SPI_HandleTypeDef *mySpi)
{
	hspi = mySpi;
	/*
	SPI_DDR = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS); 
#if 1
	SPI_DDR &= ~(1 << SPI_MISO);   // MISO���� �Է����� ����
#endif 
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//prescaler 16  SPE: SPIȰ��ȭ MSTR: �����͸�� 
    //  SPCR = 0x00;
     *
     */
}


uint8_t spi_transmit(uint8_t data)
{
	uint8_t rxData;
	HAL_SPI_TransmitReceive(hspi, &data, &rxData, 1, 1000);
	/*
	SPDR = data;
//UART0_printf_string("spi_transmit SPDR start \r\n");
	while(!(SPSR & (1<<SPIF)));
//UART0_printf_string("spi_transmit SPDR end!!! \r\n");	
	return SPDR;
	*/
	return rxData;
}

void ENABLE_CHIP(void)
{
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);
	HAL_GPIO_WritePin(RFID_SS_PORT, RFID_SS_PIN, RESET);
}

void DISABLE_CHIP(void)
{
	HAL_GPIO_WritePin(RFID_SS_PORT, RFID_SS_PIN, SET);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
}
