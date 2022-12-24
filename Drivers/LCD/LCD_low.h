/*
 *	@file LCD_low.h
 *	@brief Header file for LCD low level defintions
 *	@author Trevor Thomas
 *  Created on: Dec 21, 2022
 */

#ifndef LCD_LOW_H_
#define LCD_LOW_H_

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
 * @brief Read a four bytes from the LCD
 * @param addr 32 (24) bit address literal
 * @param hspi hspi handle
 * @retval Word received from LCD
 */
uint32_t LCD_Read32(uint32_t addr, SPI_HandleTypeDef *hspi);

#endif
