/*
 *	@file ax5243_api.c
 *	@brief Source file for ax5243 API definitions
 *	@author Trevor Thomas
 *  Created on: Nov 9, 2022
 */

#include "ax5243_api.h"

uint8_t AX_Radio_Init(SPI_HandleTypeDef * hspi)
{
	//AX_Radio_Reset(hspi);
	AX_Radio_Set_Registers_TXCW(hspi);
	HAL_Delay(1000);
	if (AX_Radio_Range_PLL(hspi)) {
		return AXRADIO_ERR_RANGING;
	}
	HAL_Delay(3000);
	return 0;
}


uint16_t AX_Radio_Get_Status(SPI_HandleTypeDef * hspi)
{
	uint16_t addr, status;
	uint8_t data;
	addr = status = data = 0x00;

	return (AX_Radio_RW_Long_Address(&addr, &status, &data, hspi, 0xff, 0x0)) ? 0xffff : status;
}

uint8_t AX_Radio_Check_PLL(SPI_HandleTypeDef * hspi)
{
	return radio_read8(AX5043_REG_PLLRANGINGA, hspi);
}

uint8_t AX_Radio_Check_PLL_Lock(SPI_HandleTypeDef * hspi)
{
	return AX_Radio_Check_PLL(hspi) & 0x40;
}

uint8_t AX_Radio_Range_PLL(SPI_HandleTypeDef * hspi)
{
	uint8_t pll_contents;
	radio_write8(AX5043_REG_PLLRANGINGA, 0x18, hspi);
	do
	{
		pll_contents = radio_read8(AX5043_REG_PLLRANGINGA, hspi);
		if(pll_contents & 0x20) {
			return pll_contents;	//Range error
		}
	}
	while (pll_contents & 0x10);
	HAL_Delay(100);
	if(pll_contents & 0x40) {
		return 0;				//Range lock
	} else if (pll_contents & 0x20) {
		return pll_contents;	//Range error
	}
	else return 0; //Ideally this should be dead code... Make error?
}

uint8_t AX_Radio_Reset(SPI_HandleTypeDef * hspi)
{
	if (radio_write8(0x80, AX5043_REG_PWRMODE, hspi)) {return 1;}
	HAL_Delay(1000);
	return 0;
}

uint8_t AX_Radio_Poweroff(SPI_HandleTypeDef * hspi)
{
	return radio_write8(0x00, AX5043_REG_PWRMODE, hspi);
}
