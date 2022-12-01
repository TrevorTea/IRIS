/*
 *	@file ax5243_api.c
 *	@brief Source file for ax5243 API definitions
 *	@author Trevor Thomas
 *  Created on: Nov 9, 2022
 */

#include "ax5243_api.h"


uint8_t AX_Radio_Init(SPI_HandleTypeDef * hspi)
	{
		radio_write8(AX5043_REG_PWRMODE, AX_RADIO_Get_Pwrmode_Upper(hspi) | AX5043_PWRSTATE_XTAL_ON, hspi);
		AX_Radio_Set_Registers(hspi);
		AX_Radio_Set_Registers_TX(hspi);
		//AX_Radio_Set_Registers_Common(hspi);	//may not be necessary
		radio_write8(AX5043_REG_MODULATION, 8, hspi);   // Set an FSK mode
		radio_write8(AX5043_REG_FSKDEV2, 0x00, hspi);
		radio_write8(AX5043_REG_FSKDEV1, 0x00, hspi);
		radio_write8(AX5043_REG_FSKDEV0, 0x00, hspi);
		radio_write8(AX5043_REG_TXRATE2, 0x00, hspi);
		radio_write8(AX5043_REG_TXRATE1, 0x00, hspi);
		radio_write8(AX5043_REG_TXRATE0, 0x01, hspi);
		radio_write8(AX5043_REG_PINFUNCDATA, 0x04, hspi);

		extern uint32_t axradio_phy_chanfreq[1];
		uint32_t freq = axradio_phy_chanfreq[0];
		radio_write8(AX5043_REG_FREQA0, freq, hspi);
		radio_write8(AX5043_REG_FREQA1, (freq >> 8), hspi);
		radio_write8(AX5043_REG_FREQA2, (freq >> 16), hspi);
		radio_write8(AX5043_REG_FREQA3, (freq >> 24), hspi);
		radio_write8(AX5043_REG_PWRMODE, AX_RADIO_Get_Pwrmode_Upper(hspi) | AX5043_PWRSTATE_SYNTH_TX, hspi);
		HAL_Delay(1000); //One second delay for XTAL settling. Minimum should be 500ms
		axradio_calvcoi(hspi);

		extern uint8_t * axradio_phy_chanpllrng;
		uint8_t r = 0x18; //big bet here, but I think this is right to start the PLL range
		radio_write8(AX5043_REG_PLLRANGINGA, r, hspi);

		//TODO, extract this into a separate function. I imagine waiting on the PLL will be helpful
		//Also if shit isn't working look here first.
		while(radio_read8(AX5043_REG_PLLRANGINGA, hspi) & 0x10) {
			if(radio_read8(AX5043_REG_PLLRANGINGA, hspi) & 0x20) {
				return AXRADIO_ERR_RANGING;
			}
		}

		if(radio_read8(AX5043_REG_PLLRANGINGA, hspi) & 0x20) {
			return AXRADIO_ERR_RANGING;
		}
		axradio_phy_chanpllrng[0] = radio_read8(AX5043_REG_PLLRANGINGA, hspi);
		radio_write8(AX5043_REG_PLLLOOP, (radio_read8(AX5043_REG_PLLLOOP, hspi) | 0x04), hspi);
		//Hidden register manipulation. If God asked me what this did I would have no answer
		{
			radio_write8(AX5043_REG_0xF35, 0x11, hspi);
//				uint8_t x = radio_read8(AX5043_REG_0xF35, hspi);
//				x |= 0x80;
//				if (2 & (uint8_t)~x)
//					++x;
//				radio_write8(AX5043_REG_0xF35, x, hspi);
		}

		HAL_Delay(3000);
		radio_write8(AX5043_REG_PWRMODE, AX_RADIO_Get_Pwrmode_Upper(hspi) | AX5043_PWRSTATE_FULL_TX, hspi);
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
	radio_write8(AX5043_REG_PLLRANGINGA, 0x18, hspi);
	uint8_t pll_contents;
	do
	{
		pll_contents = radio_read8(AX5043_REG_PLLRANGINGA, hspi);
		if(pll_contents & 0x20) {
			return 1;
		}
		if(pll_contents & 0x40) {
			return 0;
		}
	}
	while (pll_contents & 0x10);
	return 0; //Ideally this should be dead code... Make error?
}
