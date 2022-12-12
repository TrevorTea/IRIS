/*
 *	@file ax5243.h
 *	@brief Header file for ax5243 interface definitions
 *	@author Trevor Thomas
 *  Created on: Nov 7, 2022
 */

#ifndef AX5243_AX5243_H_
#define AX5243_AX5243_H_

#include "stm32l4xx_hal.h"
#include "ax5243_registers.h"
#include "ax5243_macros.h"

	/**
	 * @brief Read a register from the AX Radio utilizing the short address configuration
	 * @param addr Pointer to the seven bit shortened address to be accessed
	 * @param status Pointer to status bit buffer
	 * @param data Pointer to data bit buffer
	 * @param hspi Handle to SPI bus
	 * @param timeout Timeout for SPI transmit/receive
	 * @param rw flag for designating read or write
	 * @retval HAL Status, result should be 0
	 */
	HAL_StatusTypeDef AX_Radio_RW_Short_Address(uint8_t * addr, uint8_t * status, uint8_t * data,
													SPI_HandleTypeDef * hspi, uint32_t timeout, uint8_t rw);


	/**
	 * @brief Read a register from the AX Radio utilizing the long address configuration
	 * @param addr Pointer to the 12 bit full address to be accessed
	 * @param status Pointer to status bit buffer
	 * @param data Pointer to data bit buffer
	 * @param hspi Handle to SPI bus
	 * @param timeout Timeout for SPI transmit/receive
	 * @param rw flag for designating read or write
	 * @retval HAL Status, result should be 0
	 */
	HAL_StatusTypeDef AX_Radio_RW_Long_Address(uint16_t * addr, uint16_t * status, uint8_t * data,
													SPI_HandleTypeDef * hspi, uint32_t timeout, uint8_t rw);

	/**
	 * @brief Wrapper for AX Radio Lab SPI interfacing to satisfy library
	 * @note This is a blatant misuse of pointers, the only change is the inclusion of hspi handle
	 * @param reg 16 bit register address literal
	 * @param data 8 bit data value literal
	 * @param hspi hspi handle
	 * @retval HAL SPI return status from all three interactions
	 */
	uint8_t radio_write8(uint16_t reg, uint8_t data, SPI_HandleTypeDef * hspi);

	/**
		 * @brief Wrapper for AX Radio Lab SPI interfacing to satisfy library
		 * @note This is a blatant misuse of pointers, the only change is the inclusion of hspi handle
		 * @param reg 16 bit register address literal
		 * @param hspi hspi handle
		 */
	uint8_t radio_read8(uint16_t reg, SPI_HandleTypeDef * hspi);

	/**
	 	 * @brief Read power mode register
		 * @param hspi hspiu handle 
		 * @retval AX Radio powermode register contents
	*/
	uint8_t AX_Radio_Get_Pwrmode(SPI_HandleTypeDef * hspi);

	/**
	 	 * @brief Read power mode register
		 * @param hspi hspiu handle 
		 * @retval AX Radio powermode register upper nibbkle
	*/
	uint8_t AX_Radio_Get_Pwrmode_Upper(SPI_HandleTypeDef * hspi);

	/**
	 	 * @brief Init AX Radio with minimum definitions for CW TX
	 	 * @param hspi hspi handle
	 	 * @retval status 0 indicates success
	 */
	uint8_t AX_Radio_Set_Registers_TXCW(SPI_HandleTypeDef * hspi);

#endif /* AX5243_AX5243_H_ */
