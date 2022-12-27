/*
 *	@file ax5243_api.h
 *	@brief Header file for ax5243 API definitions
 *	@author Trevor Thomas
 *  Created on: Nov 9, 2022
 */

#ifndef AX5243_AX5243_API_H_
#define AX5243_AX5243_API_H_

#include "stm32l4xx_hal.h"
#include "ax5243.h"

	/**
	 * @brief Initializes the AX Radio
	 * @param hspi hspi handle
	 * @retval 8 bit status indicating init success
	 */
	uint8_t AX_Radio_Init(SPI_HandleTypeDef * hspi);

	/**
	 * @brief Initializes the AX Radio
	 * @param hspi hspi handle
	 * @retval 8 bit status indicating init success
	 */
	uint8_t AX_Radio_Full_Init(SPI_HandleTypeDef * hspi);


	/**
	 * @brief Returns the AX Radios status bits
	 * @param hspi hspi handle
	 * @retval 16 bit status returned from SPI read
	 * @note see page 23 https://www.onsemi.com/pdf/datasheet/ax5243-d.pdf
	 */
	uint16_t AX_Radio_Get_Status(SPI_HandleTypeDef * hspi);

	/**
	 * @brief Returns the contents of PLLRANGINGA, 0x33
	 * @param hspi hspi handle
	 * @retval Contents of PLLRANGINGA
	 * @note see page 26 https://www.onsemi.com/pdf/datasheet/ax5243-d.pdf
	 */
	uint8_t AX_Radio_Check_PLL(SPI_HandleTypeDef * hspi);

	/**
	 * @brief Ranges the PLL
	 * @param hspi hspi handle
	 * @retval status 0 indicates success
	 */
	uint8_t AX_Radio_Range_PLL(SPI_HandleTypeDef * hspi);

	/**
	 * @brief Resets the AX Radio by setting the RST bit high in the pwrmode register
	 * @param hspi hspi handle
	 * @retval status 0 indicates success
	 */
	uint8_t AX_Radio_Reset(SPI_HandleTypeDef * hspi);

	/**
	 * @brief Powers off the AX Radio, register contents are lost
	 * @param hspi hspi handle
	 * @retval status 0 indicates success
	 */
	uint8_t AX_Radio_Poweroff(SPI_HandleTypeDef * hspi);


#endif /* AX5243_AX5243_API_H_ */
