/*
 * spi.h
 *  
 */
#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define RFID_SS_PORT		GPIOC
#define RFID_SS_PIN		GPIO_PIN_4

void spi_init();                        // spi.c line 8
uint8_t spi_transmit(uint8_t data);     // spi.c line 15
void ENABLE_CHIP(void);
void DISABLE_CHIP(void);

//#define ENABLE_CHIP() (SPI_PORT &= (~(1<<SPI_SS)))
//#define DISABLE_CHIP() (SPI_PORT |= (1<<SPI_SS))



#endif
