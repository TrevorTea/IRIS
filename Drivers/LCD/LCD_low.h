/*
 *	@file LCD_low.h
 *	@brief Header file for LCD low level defintions
 *	@author Trevor Thomas
 *  Created on: Dec 21, 2022
 */

#ifndef LCD_LOW_H_
#define LCD_LOW_H_

/* Definition for memory map, see datasheet page 40 */
#define RAM_G 			0x000000 	//General purpose graphics RAM
#define ROM_FONT 		0x1E0000	//Font table
#define ROM_FONT_ADDR	0x2FFFFC	//Font table pointer address
#define RAM_DL			0x300000	//Display list RAM
#define RAM_REG			0x302000	//Registers
#define	RAM_CMD			0x308000	//Command Buffer

#include "stm32l4xx_hal.h"

/**
 * @brief Send three bytes of zeros to enter ACTIVE mode
 * @param hspi hspi handle
 * @retval HAL status
 * @note According to the datasheet two subsequent reads from 0x00 should also send ACTIVE command
 */
HAL_StatusTypeDef LCD_Active_Mode(SPI_HandleTypeDef * hspi);

/**
 *  @brief Begin SPI read per LCD controller read protocol
 *  @param addr 24 bit address to read from
 *  @param hspi hspi handle
 *  @retval HAL status
 */
HAL_StatusTypeDef LCD_Begin_Read(uint32_t addr, SPI_HandleTypeDef * hspi);


/**
 * @brief Read a single byte from the LCD
 * @param addr 32 (24) bit address literal
 * @param hspi hspi handle
 * @retval Byte received from LCD
 */
uint8_t LCD_Read8(uint32_t addr, SPI_HandleTypeDef *hspi);

/**
 * @brief Read two bytes from the LCD
 * @param addr 32 (24) bit address literal
 * @param hspi hspi handle
 * @retval Two bytes
 */
uint16_t LCD_Read16(uint32_t addr, SPI_HandleTypeDef *hspi);

/**
 * @brief Read four bytes from the LCD
 * @param addr 32 (24) bit address literal
 * @param hspi hspi handle
 * @retval Word received from LCD
 */
uint32_t LCD_Read32(uint32_t addr, SPI_HandleTypeDef *hspi);

/**
 * @brief Read chip ID code from RAM_G
 * @param hspi hspi handle
 * @retval Chip identification code
 */
uint32_t LCD_Read_Chip_ID(SPI_HandleTypeDef *hspi);


#endif
