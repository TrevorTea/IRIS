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

void AX_Radio_Set_Registers(SPI_HandleTypeDef * hspi)
	{
		radio_write8(AX5043_REG_MODULATION, 0x08, hspi);
		radio_write8(AX5043_REG_ENCODING, 0x00, hspi);
		radio_write8(AX5043_REG_FRAMING, 0x26, hspi);
		radio_write8(AX5043_REG_PINFUNCSYSCLK, 0x01, hspi);
		radio_write8(AX5043_REG_PINFUNCDCLK, 0x01, hspi);
		radio_write8(AX5043_REG_PINFUNCDATA, 0x01, hspi);
		radio_write8(AX5043_REG_PINFUNCANTSEL, 0x01, hspi);
		radio_write8(AX5043_REG_PINFUNCPWRAMP, 0x07, hspi);
		radio_write8(AX5043_REG_WAKEUPXOEARLY, 0x01, hspi);
		radio_write8(AX5043_REG_IFFREQ1, 0x01, hspi);
		radio_write8(AX5043_REG_IFFREQ0, 0xD0, hspi);
		radio_write8(AX5043_REG_DECIMATION, 0x17, hspi);
		radio_write8(AX5043_REG_RXDATARATE2, 0x00, hspi);
		radio_write8(AX5043_REG_RXDATARATE1, 0x3D, hspi);
		radio_write8(AX5043_REG_RXDATARATE0, 0x69, hspi);
		radio_write8(AX5043_REG_MAXDROFFSET2, 0x00, hspi);
		radio_write8(AX5043_REG_MAXDROFFSET1, 0x00, hspi);
		radio_write8(AX5043_REG_MAXDROFFSET0, 0x00, hspi);
		radio_write8(AX5043_REG_MAXRFOFFSET2, 0x80, hspi);
		radio_write8(AX5043_REG_MAXRFOFFSET1, 0x05, hspi);
		radio_write8(AX5043_REG_MAXRFOFFSET0, 0xC0, hspi);
		radio_write8(AX5043_REG_FSKDMAX1, 0x00, hspi);
		radio_write8(AX5043_REG_FSKDMAX0, 0xA6, hspi);
		radio_write8(AX5043_REG_FSKDMIN1, 0xFF, hspi);
		radio_write8(AX5043_REG_FSKDMIN0, 0x5A, hspi);
		radio_write8(AX5043_REG_AMPLFILTER, 0x00, hspi);
		radio_write8(AX5043_REG_RXPARAMSETS, 0xF4, hspi);
		radio_write8(AX5043_REG_AGCGAIN0, 0xC5, hspi);
		radio_write8(AX5043_REG_AGCTARGET0, 0x84, hspi);
		radio_write8(AX5043_REG_TIMEGAIN0, 0xF8, hspi);
		radio_write8(AX5043_REG_DRGAIN0, 0xF2, hspi);
		radio_write8(AX5043_REG_PHASEGAIN0, 0xC3, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINA0, 0x0F, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINB0, 0x1F, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINC0, 0x08, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAIND0, 0x08, hspi);
		radio_write8(AX5043_REG_AMPLITUDEGAIN0, 0x06, hspi);
		radio_write8(AX5043_REG_FREQDEV10, 0x00, hspi);
		radio_write8(AX5043_REG_FREQDEV00, 0x00, hspi);
		radio_write8(AX5043_REG_BBOFFSRES0, 0x00, hspi);
		radio_write8(AX5043_REG_AGCGAIN1, 0xC5, hspi);
		radio_write8(AX5043_REG_AGCTARGET1, 0x84, hspi);
		radio_write8(AX5043_REG_AGCAHYST1, 0x00, hspi);
		radio_write8(AX5043_REG_AGCMINMAX1, 0x00, hspi);
		radio_write8(AX5043_REG_TIMEGAIN1, 0xF6, hspi);
		radio_write8(AX5043_REG_DRGAIN1, 0xF1, hspi);
		radio_write8(AX5043_REG_PHASEGAIN1, 0xC3, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINA1, 0x0F, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINB1, 0x1F, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINC1, 0x08, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAIND1, 0x08, hspi);
		radio_write8(AX5043_REG_AMPLITUDEGAIN1, 0x06, hspi);
		radio_write8(AX5043_REG_FREQDEV11, 0x00, hspi);
		radio_write8(AX5043_REG_FREQDEV01, 0x3C, hspi);
		radio_write8(AX5043_REG_FOURFSK1, 0x16, hspi);
		radio_write8(AX5043_REG_BBOFFSRES1, 0x00, hspi);
		radio_write8(AX5043_REG_AGCGAIN3, 0xFF, hspi);
		radio_write8(AX5043_REG_AGCTARGET3, 0x84, hspi);
		radio_write8(AX5043_REG_AGCAHYST3, 0x00, hspi);
		radio_write8(AX5043_REG_AGCMINMAX3, 0x00, hspi);
		radio_write8(AX5043_REG_TIMEGAIN3, 0xF5, hspi);
		radio_write8(AX5043_REG_DRGAIN3, 0xF0, hspi);
		radio_write8(AX5043_REG_PHASEGAIN3, 0xC3, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINA3, 0x0F, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINB3, 0x1F, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAINC3, 0x0C, hspi);
		radio_write8(AX5043_REG_FREQUENCYGAIND3, 0x0C, hspi);
		radio_write8(AX5043_REG_AMPLITUDEGAIN3, 0x06, hspi);
		radio_write8(AX5043_REG_FREQDEV13, 0x00, hspi);
		radio_write8(AX5043_REG_FREQDEV03, 0x3C, hspi);
		radio_write8(AX5043_REG_FOURFSK3, 0x16, hspi);
		radio_write8(AX5043_REG_BBOFFSRES3, 0x00, hspi);
		radio_write8(AX5043_REG_MODCFGF, 0x00, hspi);
		radio_write8(AX5043_REG_FSKDEV2, 0x00, hspi);
		radio_write8(AX5043_REG_FSKDEV1, 0x03, hspi);
		radio_write8(AX5043_REG_FSKDEV0, 0xDE, hspi);
		radio_write8(AX5043_REG_MODCFGA, 0x06, hspi);
		radio_write8(AX5043_REG_TXRATE2, 0x00, hspi);
		radio_write8(AX5043_REG_TXRATE1, 0x0B, hspi);
		radio_write8(AX5043_REG_TXRATE0, 0x99, hspi);
		radio_write8(AX5043_REG_TXPWRCOEFFB1, 0x0F, hspi);
		radio_write8(AX5043_REG_TXPWRCOEFFB0, 0xFF, hspi);
		radio_write8(AX5043_REG_PLLVCOI, 0x80, hspi);
		radio_write8(AX5043_REG_PLLRNGCLK, 0x04, hspi);
		radio_write8(AX5043_REG_BBTUNE, 0x0F, hspi);
		radio_write8(AX5043_REG_BBOFFSCAP, 0x77, hspi);
		radio_write8(AX5043_REG_PKTADDRCFG, 0x01, hspi);
		radio_write8(AX5043_REG_PKTLENCFG, 0x80, hspi);
		radio_write8(AX5043_REG_PKTLENOFFSET, 0x00, hspi);
		radio_write8(AX5043_REG_PKTMAXLEN, 0xC8, hspi);
		radio_write8(AX5043_REG_MATCH0PAT3, 0xAA, hspi);
		radio_write8(AX5043_REG_MATCH0PAT2, 0xCC, hspi);
		radio_write8(AX5043_REG_MATCH0PAT1, 0xAA, hspi);
		radio_write8(AX5043_REG_MATCH0PAT0, 0xCC, hspi);
		radio_write8(AX5043_REG_MATCH0LEN, 0x1F, hspi);
		radio_write8(AX5043_REG_MATCH0MAX, 0x1F, hspi);
		radio_write8(AX5043_REG_MATCH1PAT1, 0x55, hspi);
		radio_write8(AX5043_REG_MATCH1PAT0, 0x55, hspi);
		radio_write8(AX5043_REG_MATCH1LEN, 0x8A, hspi);
		radio_write8(AX5043_REG_MATCH1MAX, 0x0A, hspi);
		radio_write8(AX5043_REG_TMGTXBOOST, 0x3F, hspi);
		radio_write8(AX5043_REG_TMGTXSETTLE, 0x31, hspi);
		radio_write8(AX5043_REG_TMGRXBOOST, 0x3F, hspi);
		radio_write8(AX5043_REG_TMGRXSETTLE, 0x31, hspi);
		radio_write8(AX5043_REG_TMGRXOFFSACQ, 0x00, hspi);
		radio_write8(AX5043_REG_TMGRXCOARSEAGC, 0x90, hspi);
		radio_write8(AX5043_REG_TMGRXRSSI, 0x03, hspi);
		radio_write8(AX5043_REG_TMGRXPREAMBLE2, 0x35, hspi);
		radio_write8(AX5043_REG_RSSIABSTHR, 0xE0, hspi);
		radio_write8(AX5043_REG_BGNDRSSITHR, 0x00, hspi);
		radio_write8(AX5043_REG_PKTCHUNKSIZE, 0x0D, hspi);
		radio_write8(AX5043_REG_PKTACCEPTFLAGS, 0x20, hspi);
		radio_write8(AX5043_REG_DACVALUE1, 0x00, hspi);
		radio_write8(AX5043_REG_DACVALUE0, 0x00, hspi);
		radio_write8(AX5043_REG_DACCONFIG, 0x00, hspi);
		radio_write8(AX5043_REG_REF, 0x03, hspi);
		radio_write8(AX5043_REG_XTALOSC, 0x03, hspi);
		radio_write8(AX5043_REG_XTALAMPL, 0x07, hspi);
		radio_write8(AX5043_REG_0xF1C, 0x07, hspi);
		radio_write8(AX5043_REG_0xF21, 0x68, hspi);
		radio_write8(AX5043_REG_0xF22, 0xFF, hspi);
		radio_write8(AX5043_REG_0xF23, 0x84, hspi);
		radio_write8(AX5043_REG_0xF26, 0x98, hspi);
		radio_write8(AX5043_REG_0xF34, 0x08, hspi);
		radio_write8(AX5043_REG_0xF35, 0x11, hspi);
		radio_write8(AX5043_REG_0xF44, 0x25, hspi);
	}

void AX_Radio_Set_Registers_TX(SPI_HandleTypeDef * hspi)
	{
		radio_write8(AX5043_REG_PLLLOOP, 0x04, hspi);
		radio_write8(AX5043_REG_PLLCPI, 0xff, hspi);
		radio_write8(AX5043_REG_PLLVCODIV, 0x34, hspi);
		radio_write8(AX5043_REG_XTALCAP, 0x08, hspi);
		radio_write8(AX5043_REG_0xF00, 0x0F, hspi);
		radio_write8(AX5043_REG_0xF18, 0x06, hspi);
	}

void AX_Radio_Set_Registers_RX(SPI_HandleTypeDef * hspi)
	{
		radio_write8(AX5043_REG_PLLLOOP, 0x04, hspi);
		radio_write8(AX5043_REG_PLLCPI, 0x01, hspi);
		radio_write8(AX5043_REG_PLLVCODIV, 0x10, hspi);
		radio_write8(AX5043_REG_XTALCAP, 0x01, hspi);
		radio_write8(AX5043_REG_0xF00, 0x0F, hspi);
		radio_write8(AX5043_REG_0xF18, 0x06, hspi);
	}

void AX_Radio_Set_Registers_RXWOR(SPI_HandleTypeDef * hspi)
	{
		radio_write8(AX5043_REG_TMGRXAGC, 0x0C, hspi);
		radio_write8(AX5043_REG_TMGRXPREAMBLE1, 0x19, hspi);
		radio_write8(AX5043_REG_PKTMISCFLAGS, 0x03, hspi);
	}

void AX_Radio_Set_Registers_RXWOR_SingleParamset(SPI_HandleTypeDef * hspi)
	{
		radio_write8(AX5043_REG_RXPARAMSETS, 0xFF, hspi);
		radio_write8(AX5043_REG_FREQDEV13, 0x00, hspi);
		radio_write8(AX5043_REG_FREQDEV03, 0x00, hspi);
		radio_write8(AX5043_REG_AGCGAIN3, 0xE7, hspi);
	}



// physical layer
const uint8_t  axradio_phy_pn9 = 0;
const uint8_t  axradio_phy_nrchannels = 1;
const uint32_t  axradio_phy_chanfreq[1] = { 0x063b47bf };
const uint16_t  axradio_phy_chanpllrnginit[1] = { 0xFF };
const uint8_t  axradio_phy_chanvcoiinit[1] = { 0x00 };
uint16_t axradio_phy_chanpllrng[1];
uint8_t axradio_phy_chanvcoi[1];
const uint8_t  axradio_phy_vcocalib = 1;
const int32_t  axradio_phy_maxfreqoffset = 4417;
const int8_t  axradio_phy_rssioffset = 64;
// axradio_phy_rssioffset is added to AX5043_RSSIREFERENCE and subtracted from chip RSSI value to prevent overflows (8bit RSSI only goes down to -128)
// axradio_phy_rssioffset is also added to AX5043_RSSIABSTHR
//Weird logic autogenerated here. Added warning suppression
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
const int8_t  axradio_phy_rssireference = 0xF8 + 64;
#pragma GCC diagnostic pop
const int8_t  axradio_phy_channelbusy = -96 + 64;
const uint16_t  axradio_phy_cs_period = 7; // timer0 units, 10ms
const uint8_t  axradio_phy_cs_enabled = 0;
const uint8_t  axradio_phy_lbt_retries = 0;
const uint8_t  axradio_phy_lbt_forcetx = 0;
const uint16_t  axradio_phy_preamble_wor_longlen = 4; // wor_longlen + wor_len totals to 240.0ms plus 32bits
const uint16_t  axradio_phy_preamble_wor_len = 160;
const uint16_t  axradio_phy_preamble_longlen = 0;
const uint16_t  axradio_phy_preamble_len = 32;
const uint8_t  axradio_phy_preamble_byte = 0x55;
const uint8_t  axradio_phy_preamble_flags = 0x38;
const uint8_t  axradio_phy_preamble_appendbits = 0;
const uint8_t  axradio_phy_preamble_appendpattern = 0x00;

//framing
const uint8_t  axradio_framing_maclen = 3;
const uint8_t  axradio_framing_addrlen = 2;
const uint8_t  axradio_framing_destaddrpos = 1;
const uint8_t  axradio_framing_sourceaddrpos = 0xff;
const uint8_t  axradio_framing_lenpos = 0;
const uint8_t  axradio_framing_lenoffs = 0;
const uint8_t  axradio_framing_lenmask = 0xff;
const uint8_t  axradio_framing_swcrclen = 0;

const uint8_t  axradio_framing_synclen = 32;
const uint8_t  axradio_framing_syncword[] = { 0xcc, 0xaa, 0xcc, 0xaa};
const uint8_t  axradio_framing_syncflags = 0x18;
const uint8_t  axradio_framing_enable_sfdcallback = 0;

const uint32_t  axradio_framing_ack_timeout = 1213; // 37.0ms in wtimer0 units (32768Hz)
const uint32_t  axradio_framing_ack_delay = 313; // 1.0ms in wtimer1 units (20MHz/64)
const uint8_t  axradio_framing_ack_retransmissions = 0;
const uint8_t  axradio_framing_ack_seqnrpos = 0xff;

const uint8_t  axradio_framing_minpayloadlen = 1; // must be set to 1 if the payload directly follows the destination address, and a CRC is configured
//WOR
const uint16_t  axradio_wor_period = 128;

// synchronous mode
const uint32_t  axradio_sync_period = 1262; // ACTUALLY FREQ, NOT PERIOD!
const uint32_t  axradio_sync_xoscstartup = 49;
const uint32_t  axradio_sync_slave_syncwindow = 3785; // 0.116s
const uint32_t  axradio_sync_slave_initialsyncwindow = 5898240; //180.000s
const uint32_t  axradio_sync_slave_syncpause = 19660800; // 600.000s
const int16_t  axradio_sync_slave_maxperiod = 1617; // in (2^SYNC_K1) * wtimer0 units
const uint8_t  axradio_sync_slave_resyncloss = 11;  // resyncloss is one more than the number of missed packets to cause a resync
// window 0 is the first window after synchronisation
// window 1 is the window normally used when there are no lost packets
// window 2 is used after one packet is lost, etc
const uint8_t  axradio_sync_slave_nrrx = 3;
const uint32_t  axradio_sync_slave_rxadvance[] = { 673, 647, 719 };// 20.521ms, 19.727ms, 21.924ms
const uint32_t  axradio_sync_slave_rxwindow[] = { 674, 622, 766 }; // 20.551ms, 18.964ms, 23.359ms
const uint32_t  axradio_sync_slave_rxtimeout = 1052; // 32.1ms, maximum duration of a packet
const uint16_t  axradio_lposckfiltmax = 0x1355;
const uint32_t  axradio_fxtal = 	27120000;

static int16_t axradio_tunevoltage(SPI_HandleTypeDef * hspi)
{
	int16_t r = 0;
	uint8_t cnt = 64;
	do {
		radio_write8(AX5043_REG_GPADCCTRL, 0x84, hspi);
		do {} while (radio_read8(AX5043_REG_GPADCCTRL, hspi) & 0x80);
	} while (--cnt);
	cnt = 32;
	do {
		radio_write8(AX5043_REG_GPADCCTRL, 0x84, hspi);
		do {} while (radio_read8(AX5043_REG_GPADCCTRL, hspi) & 0x80);
		{
			int16_t x = radio_read8(AX5043_REG_GPADC13VALUE1, hspi) & 0x03;
			x <<= 8;
			x |= radio_read8(AX5043_REG_GPADC13VALUE0, hspi);
			r += x;
		}
	} while (--cnt);
	return r;
}

uint8_t axradio_calvcoi(SPI_HandleTypeDef * hspi)
{
	uint8_t i;
	uint8_t r = 0;
	uint16_t vmin = 0xffff;
	uint16_t vmax = 0x0000;
	uint8_t axradio_rxbuffer[260];
	for (i = 0x40; i != 0;) {
		uint16_t curtune;
		--i;
		radio_write8(AX5043_REG_PLLVCOI, (0x80 | i), hspi);
		radio_read8(AX5043_REG_PLLRANGINGA, hspi); // clear PLL lock loss
		curtune = axradio_tunevoltage(hspi);
		radio_read8(AX5043_REG_PLLRANGINGA, hspi); // clear PLL lock loss
		((uint16_t *)axradio_rxbuffer)[i] = curtune;
		if (curtune > vmax)
			vmax = curtune;
		if (curtune < vmin) {
			vmin = curtune;
			// check whether the PLL is locked
			if (!(0xC0 & (uint8_t)~(radio_read8(AX5043_REG_PLLRANGINGA, hspi))))
				r = i | 0x80;
		}
	}
	if (!(r & 0x80) || vmax >= 0xFF00 || vmin < 0x0100 || vmax - vmin < 0x4000)
		return 0;
	return r;
}

uint8_t AX_Radio_Set_Registers_Common(SPI_HandleTypeDef * hspi)
{
	uint8_t rng = axradio_phy_chanpllrng[0];
	if (rng & 0x20)
		return AXRADIO_ERR_RANGING;
	if (radio_read8(AX5043_REG_PLLLOOP, hspi) & 0x80)
		radio_write8(AX5043_REG_PLLRANGINGB, (rng & 0x0F), hspi);
	else
		radio_write8(AX5043_REG_PLLRANGINGA, (rng & 0x0F), hspi);
	rng = radio_read8(AX5043_REG_PLLVCOI, hspi);
	if (rng & 0x80)
		radio_write8(AX5043_REG_PLLVCOI, rng, hspi); //This is the only line that executes.
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_RADIO_Get_Pwrmode(SPI_HandleTypeDef * hspi) 
{
	return radio_read8(AX5043_REG_PWRMODE, hspi);
}

uint8_t AX_RADIO_Get_Pwrmode_Upper(SPI_HandleTypeDef * hspi)
{
	return AX_RADIO_Get_Pwrmode(hspi) & 0xF0;
}
