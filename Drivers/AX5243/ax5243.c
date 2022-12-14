/*
 *	@file ax5243.c
 *	@brief Source file for ax5243 interface definitions
 *	@author Trevor Thomas
 *  Created on: Nov 7, 2022
 */
#include "ax5243.h"

HAL_StatusTypeDef AX_Radio_RW_Short_Address(uint8_t * addr, uint8_t * status, uint8_t * data,
												SPI_HandleTypeDef * hspi, uint32_t timeout, uint8_t rw)
	{
		  HAL_StatusTypeDef status1, status2;
		  //bring chip select low
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		  //mask for read or write depending on flag
		  *addr = rw == 0 ? *addr & 0x7F : *addr | 0x80;
		  //send 7 bit address with MSB signifying rw, receive status bits
		  status1 = HAL_SPI_TransmitReceive(hspi, (uint8_t *)addr, (uint8_t *)status, 1, timeout);
		  //receive data bits back
		  status2 = rw == 0 ? HAL_SPI_Receive(hspi, data, 1, timeout) : HAL_SPI_Transmit(hspi, data, 1, timeout);
		  //bring chip select high
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

		  return status1 | status2;
	}



HAL_StatusTypeDef AX_Radio_RW_Long_Address(uint16_t * addr, uint16_t * status, uint8_t * data,
		SPI_HandleTypeDef * hspi, uint32_t timeout, uint8_t rw)
	{
		  HAL_StatusTypeDef status1, status2, status3;
		  uint8_t addr_upper, addr_lower, status_upper, status_lower;
		  //bring chip select low
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		  //mask for read or write depending on flag
		  *addr = rw == 0 ? ((*addr & 0x7FFF) | 0x7000) : *addr | 0xF000;
		  addr_upper = *addr >> 8;
		  addr_lower = *addr & 0xFF;
		  //send four bit mask and upper nibble of addr
		  status1 = HAL_SPI_TransmitReceive(hspi, (uint8_t *)&addr_upper, (uint8_t *)&status_upper, 1, timeout);
		  //send lower byte of addr
		  status2 = HAL_SPI_TransmitReceive(hspi, (uint8_t *)&addr_lower, (uint8_t *)&status_lower, 1, timeout);
		  //receive data bits back
		  status3 = rw == 0 ? HAL_SPI_Receive(hspi, data, 1, timeout) : HAL_SPI_Transmit(hspi, data, 1, timeout);
		  //bring chip select high
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

		  *status = (status_upper << 8) | status_lower;

		  return status1 | status2 | status3;
	}


uint8_t radio_write8(uint16_t reg, uint8_t data, SPI_HandleTypeDef * hspi)
	{
		uint16_t reg_local, status_local;
		uint8_t data_local;
		reg_local = reg;
		status_local = 0;
		data_local = data;
		return AX_Radio_RW_Long_Address(&reg_local, &status_local, &data_local,
				hspi, 0xFF, 0x1);
	}

uint8_t radio_read8(uint16_t reg, SPI_HandleTypeDef * hspi)
	{
		uint16_t reg_local, status_local;
		uint8_t data_local = 0;
		reg_local = reg;
		status_local = 0;
		AX_Radio_RW_Long_Address(&reg_local, &status_local, &data_local,
				hspi, 0xFF, 0x0);
		return data_local;
	}

// physical layer
//const uint32_t  axradio_phy_chanfreq[1] = { 0x063b47bf };
//const uint32_t  axradio_phy_chanfreq[1] = { 0x0A900000 }; //169MHz @ 16MHz XTAL
const uint32_t  axradio_phy_chanfreq[1] = { 0x1B100000 }; //433MHz @ 16MHz XTAL
const int8_t  axradio_phy_rssioffset = 64;
// axradio_phy_rssioffset is added to AX5043_RSSIREFERENCE and subtracted from chip RSSI value to prevent overflows (8bit RSSI only goes down to -128)
// axradio_phy_rssioffset is also added to AX5043_RSSIABSTHR
//Weird logic autogenerated here. Added warning suppression
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
const int8_t  axradio_phy_rssireference = 0xF8 + 64;
#pragma GCC diagnostic pop


uint8_t AX_Radio_Get_Pwrmode(SPI_HandleTypeDef * hspi)
{
	return radio_read8(AX5043_REG_PWRMODE, hspi);
}

uint8_t AX_Radio_Get_Pwrmode_Upper(SPI_HandleTypeDef * hspi)
{
	return AX_Radio_Get_Pwrmode(hspi) & 0xF0;
}

uint8_t AX_Radio_Set_Registers_TXCW(SPI_HandleTypeDef * hspi)
{
	radio_write8(AX5043_REG_MODULATION, 0x08, hspi);
//	radio_write8(AX5043_REG_PINFUNCSYSCLK, 0x04, hspi); // This causes crosstalk with external inductor in V1 PCB
	radio_write8(AX5043_REG_TXPWRCOEFFB1, 0x0F, hspi);
	radio_write8(AX5043_REG_TXPWRCOEFFB0, 0xFF, hspi);
	radio_write8(AX5043_REG_XTALOSC, 0x03, hspi);
	radio_write8(AX5043_REG_XTALAMPL, 0x07, hspi);
	radio_write8(AX5043_REG_PLLVCODIV, 0x04, hspi);
	radio_write8(AX5043_REG_XTALCAP, 0x01, hspi);

	radio_write8(AX5043_REG_FSKDEV2, 0x00, hspi);
	radio_write8(AX5043_REG_FSKDEV1, 0x00, hspi);
	radio_write8(AX5043_REG_FSKDEV0, 0x00, hspi);
	radio_write8(AX5043_REG_TXRATE2, 0x00, hspi);
	radio_write8(AX5043_REG_TXRATE1, 0x00, hspi);
	radio_write8(AX5043_REG_TXRATE0, 0x01, hspi);

	uint32_t freq = axradio_phy_chanfreq[0];
	radio_write8(AX5043_REG_FREQA0, freq, hspi);
	radio_write8(AX5043_REG_FREQA1, (freq >> 8), hspi);
	radio_write8(AX5043_REG_FREQA2, (freq >> 16), hspi);
	radio_write8(AX5043_REG_FREQA3, (freq >> 24), hspi);

	radio_write8(AX5043_REG_0xF00, 0x0F, hspi);
	radio_write8(AX5043_REG_0xF0D, 0x03, hspi);
	radio_write8(AX5043_REG_0xF18, 0x06, hspi);
	radio_write8(AX5043_REG_0xF1C, 0x07, hspi);
	radio_write8(AX5043_REG_0xF34, 0x28, hspi);
	radio_write8(AX5043_REG_0xF35, 0x10, hspi);
	radio_write8(AX5043_REG_0xF44, 0x24, hspi);

	// NOTE: There is currently an intermittent bug where the PLL will lose its
	// lock but find it again @ 433 MHz. This may be remediated by updating the
	// FIFO every now and probably setting up encoding and framing properly.
	// ------- Crispin starts filling the FIFO here -------
	uint8_t fakePreamble = 0x55;
	uint8_t numBytesToSend = 100;
	uint8_t fakePayload = 0xAA;

	/* Radio Control for Sending Packets*/
	radio_write8(AX5043_REG_FIFOSTAT, 0x03, hspi); // clear FIFO

	// preamble
	radio_write8(AX5043_REG_FIFODATA, 0x62, hspi); // REPEATDATA command
	radio_write8(AX5043_REG_FIFODATA, 0x18, hspi); // bypass framing & encoding, suppress CRC, no pkt start/end
	radio_write8(AX5043_REG_FIFODATA, 0x14, hspi); // repeat byte given in next command 20 times
	radio_write8(AX5043_REG_FIFODATA, fakePreamble, hspi); // byte to be sent 10 times

	// frame
	radio_write8(AX5043_REG_FIFODATA, 0xE1, hspi); // TX Data command
	radio_write8(AX5043_REG_FIFODATA, (numBytesToSend+1), hspi); // 100 bytes +1 for control field byte
	radio_write8(AX5043_REG_FIFODATA, 0x03, hspi); // pkt start/end used here
	for (uint8_t i = 0; i < numBytesToSend; i++)
		radio_write8(AX5043_REG_FIFODATA, fakePayload, hspi);

	// commit
	radio_write8(AX5043_REG_FIFOSTAT, 0x04, hspi);
	// ------- Crispin stopped messing with the FIFO here -------

	return AXRADIO_ERR_NOERROR;
}
