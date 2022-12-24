/*
 *	@file LCD_low.c
 *	@brief Source file for LCD low level interface
 *	@author Trevor Thomas
 *  Created on: Dec 21, 2022
 */
#include "LCD_low.h"

HAL_StatusTypeDef LCD_Active_Mode(SPI_HandleTypeDef *hspi) {
	HAL_StatusTypeDef status = 0;
	uint8_t payload[3] = { 0, 0, 0 };
	//bring chip select low
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	//Transmit three bytes of zeros
	status = HAL_SPI_Transmit(hspi, payload, 3, 0xFF);
	//Bring chip select high
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);

	return status;
}

uint8_t LCD_Begin_Read(uint32_t addr, SPI_HandleTypeDef *hspi) {
	HAL_StatusTypeDef status;
	uint8_t addrBytes[3] = { (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr
			& 0xFF };
	uint8_t dummyByte = 0;
	//bring chip select low
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
	//Ship address bytes
	status = HAL_SPI_Transmit(hspi, addrBytes, 3, 0xFF);
	//Send dummy byte
	status |= HAL_SPI_Transmit(hspi, &dummyByte, 1, 0xFF);
	return status;
}

uint8_t LCD_Read8(uint32_t addr, SPI_HandleTypeDef *hspi) {
	uint8_t dataByte = 0;
	//Begin read
	LCD_Begin_Read(addr, hspi);
	//Read single byte
	HAL_SPI_Receive(hspi, &dataByte, 1, 0xFF);
	//Bring chip select high
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	return dataByte;
}

uint16_t LCD_Read16(uint32_t addr, SPI_HandleTypeDef *hspi) {
	uint8_t dataBytes[2] = { 0, 0 };
	//Begin read
	LCD_Begin_Read(addr, hspi);
	//Read single byte
	HAL_SPI_Receive(hspi, dataBytes, 2, 0xFF);
	//Bring chip select high
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	uint16_t result = (dataBytes[1] << 8) | (dataBytes[0]);
	return result;
}


uint32_t LCD_Read32(uint32_t addr, SPI_HandleTypeDef *hspi) {
	uint8_t dataBytes[4] = { 0, 0, 0, 0 };
	//Begin read
	LCD_Begin_Read(addr, hspi);
	//Read single byte
	HAL_SPI_Receive(hspi, dataBytes, 4, 0xFF);
	//Bring chip select high
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	uint32_t result = (dataBytes[3] << 24) | (dataBytes[2] << 16)
			| (dataBytes[1] << 8) | (dataBytes[0]);
	return result;
}

uint32_t LCD_Read_Chip_ID(SPI_HandleTypeDef *hspi) {
	return LCD_Read32( 0xC0000, hspi);
}


