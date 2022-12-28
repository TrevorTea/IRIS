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
const uint32_t  axradio_phy_chanfreq[1] = { 0x063b47bf };
//const uint32_t  axradio_phy_chanfreq[1] = { 0x0A900000 }; //169MHz @ 16MHz XTAL
//const uint32_t  axradio_phy_chanfreq[1] = { 0x1B100000 }; //433MHz @ 16MHz XTAL
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
	radio_write8(AX5043_REG_PLLVCODIV, 0x30, hspi);
	radio_write8(AX5043_REG_XTALCAP, 0x08, hspi); // C_L = 12pF

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
	radio_write8(AX5043_REG_0xF34, 0x08, hspi);
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

uint8_t AX_Radio_Data_Transmission_Setup(SPI_HandleTypeDef * hspi)
{
	// Initially testing with 169 MHz
	uint32_t rate = 9600; //9600bps
	AX_Radio_169_MHz_Settings(hspi);
	AX_Radio_Set_Data_Rate(1, rate, hspi);
	AX_Radio_Set_Frequency_Deviation(rate, hspi);
	radio_write8(AX5043_REG_MODULATION, 0x08, hspi);

	// General XTAL Settings
	radio_write8(AX5043_REG_XTALOSC, 0x03, hspi);
	radio_write8(AX5043_REG_XTALAMPL, 0x07, hspi);
	radio_write8(AX5043_REG_XTALCAP, 0x08, hspi); // C_L = 12pF

	// Full power @ 16 dBm
	radio_write8(AX5043_REG_TXPWRCOEFFB1, 0x0F, hspi);
	radio_write8(AX5043_REG_TXPWRCOEFFB0, 0xFF, hspi);

	// G3RUH encoding, HDLC framing, CCITT-16 CRC
	radio_write8(AX5043_REG_ENCODING, 0x07, hspi); // differential encoding, bit inversion, scrambler
	radio_write8(AX5043_REG_FRAMING, 0x14, hspi); // HDLC, CRC check in CCITT 16-bit
	radio_write8(AX5043_REG_PKTLENCFG, 0xF0, hspi);
	radio_write8(AX5043_REG_PKTMAXLEN, 0xFF, hspi);
	radio_write8(AX5043_REG_PKTACCEPTFLAGS, 0x20, hspi);

	// FIFOOOOOOOOOO but lets do it in main, kay?
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_Data_Reception_Setup(uint32_t datarate, SPI_HandleTypeDef * hspi)
{
	AX_Radio_169_MHz_Settings(hspi);
	AX_Radio_Set_Data_Rate(0, datarate, hspi);
	radio_write8(AX5043_REG_MODULATION, 0x08, hspi);

	//totally just trusting NBM here:
	uint8_t timegain = 0x90 | 0x08;
	uint8_t drgain = 0x90 | 0x04;
	radio_write8(AX5043_REG_TIMEGAIN0, timegain, hspi);
	// instructions AND datasheet unclear, idk if these are touched \/ but my guess is no since they're just exponent-mantissa artifacts, and we need just need one byte?
//	radio_write8(AX5043_REG_TIMEGAIN1, timegain, hspi);
//	radio_write8(AX5043_REG_TIMEGAIN2, timegain, hspi);
//	radio_write8(AX5043_REG_TIMEGAIN3, timegain, hspi);
	radio_write8(AX5043_REG_DRGAIN0, drgain, hspi);
//	radio_write8(AX5043_REG_DRGAIN1, drgain, hspi);
//	radio_write8(AX5043_REG_DRGAIN2, drgain, hspi);
//	radio_write8(AX5043_REG_DRGAIN3, drgain, hspi);
	radio_write8(AX5043_REG_FREQDEV00, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV01, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV02, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV03, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV10, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV11, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV12, 0, hspi);
	radio_write8(AX5043_REG_FREQDEV13, 0, hspi);
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_27_MHz_Settings(SPI_HandleTypeDef * hspi)
{
	radio_write8(AX5043_REG_PLLVCODIV, PLLVCODIV_EXTERNAL_INDUCTOR |
			PLLVCODIV_RFDIV_ON, hspi);
	radio_write8(AX5043_REG_0xF34, F34_RFDIV_ON, hspi);
	radio_write8(AX5043_REG_0xF35, 0x10, hspi);

	// 27 MHz, allowable range is 26.957 MHz to 27.283 MHz, max BW = 40kHz
	uint32_t fc = 27;
	AX_Radio_Set_Center_Frequency(fc, hspi);
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_169_MHz_Settings(SPI_HandleTypeDef * hspi)
{
	radio_write8(AX5043_REG_PLLVCODIV, PLLVCODIV_EXTERNAL_INDUCTOR |
			PLLVCODIV_RFDIV_OFF, hspi);
	radio_write8(AX5043_REG_0xF34, F34_RFDIV_OFF, hspi);
	radio_write8(AX5043_REG_0xF35, 0x11, hspi);

	// 169 MHz, BW = idk
	// I cannot find the laws on legality on this for the fucking life of me, yolo ig
	uint32_t fc = 169;
	AX_Radio_Set_Center_Frequency(fc, hspi);
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_915_MHz_Settings(SPI_HandleTypeDef * hspi)
{
	radio_write8(AX5043_REG_PLLVCODIV, PLLVCODIV_INTERNAL_INDUCTOR |
			PLLVCODIV_RFDIV_OFF, hspi);
	radio_write8(AX5043_REG_0xF34, F34_RFDIV_OFF, hspi);
	radio_write8(AX5043_REG_0xF35, 0x11, hspi); // currently still has 27.12 MHz crystal on the board

	// 915 MHz, range of 902 MHz to 928 Mhz, max BW of 26 MHz
	uint32_t fc = 915;
	AX_Radio_Set_Center_Frequency(fc, hspi);
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_Set_Center_Frequency(uint32_t freq, SPI_HandleTypeDef * hspi)
{
	// need to replace 27 MHz crystals soon...
	if (freq == 169)
	{
		freq = 0x063b47bf; // freq=fc/fxtal*(2^24) | since fxtal=16MHz, freq=fc*(2^20)
		radio_write8(AX5043_REG_FREQA0, freq, hspi);
		radio_write8(AX5043_REG_FREQA1, (freq >> 8), hspi);
		radio_write8(AX5043_REG_FREQA2, (freq >> 16), hspi);
		radio_write8(AX5043_REG_FREQA3, (freq >> 24), hspi);
	}
	else if (freq == 915)
	{
		freq = 0x21BD2B0B; // freq=fc/fxtal*(2^24) | since fxtal=16MHz, freq=fc*(2^20)
		radio_write8(AX5043_REG_FREQA0, freq, hspi);
		radio_write8(AX5043_REG_FREQA1, (freq >> 8), hspi);
		radio_write8(AX5043_REG_FREQA2, (freq >> 16), hspi);
		radio_write8(AX5043_REG_FREQA3, (freq >> 24), hspi);
	}
	else
	{
		freq *= (1<<20); // freq=fc/fxtal*(2^24) | since fxtal=16MHz, freq=fc*(2^20)
		radio_write8(AX5043_REG_FREQA0, freq, hspi);
		radio_write8(AX5043_REG_FREQA1, (freq >> 8), hspi);
		radio_write8(AX5043_REG_FREQA2, (freq >> 16), hspi);
		radio_write8(AX5043_REG_FREQA3, (freq >> 24), hspi);
	}
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_Set_Frequency_Deviation(uint32_t rate, SPI_HandleTypeDef * hspi)
{
	uint32_t dev = (rate>>2)*(1<<20); // FSKDEV = m * 0.5 * BITRATE / fxtal * (2^24), m(odulation index)=0.5, fxtal = 16 MHz
	radio_write8(AX5043_REG_FSKDEV0, dev, hspi);
	radio_write8(AX5043_REG_FSKDEV1, (dev >> 8), hspi);
	radio_write8(AX5043_REG_FSKDEV2, (dev >> 16), hspi);
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_Set_Data_Rate(uint8_t rw, uint32_t rate, SPI_HandleTypeDef * hspi)
{
	if (rw)
	{
		radio_write8(AX5043_REG_TXRATE0, rate, hspi);
		radio_write8(AX5043_REG_TXRATE1, (rate >> 8), hspi);
		radio_write8(AX5043_REG_TXRATE2, (rate >> 16), hspi);
	}
	else
	{
		// NOTE: XTALDIV = 1 for 27 MHz, currently XTALDIV = 2 for 169 & 915 boards..
		// TODO: define these magic numbers and even abstract away at some point...
		uint32_t bandwidth = 14400; // (1+m)*rate
		uint32_t decimation = 27120000 / ((1<<4) * 2 * 4 * bandwidth); // fxtal/(2^4*fxtaldiv*(1/fcoeff)*BW)
		uint32_t rxdatarate = (1<<7) / (2 * 9600 * decimation) * 27120000;
		uint32_t iffreq = (((bandwidth / 2) * 2) * (2<<20) / 27120000) ; // setting f_if = bw/2, iffreq = f_if*fxtaldiv*(2^20)/fxtal
		uint32_t freqOffset = 4182; //27.12MHz crystals are rated for 40ppm frequency stability, so 2*6760hz at worst case

		// leaving PHASEGAINx registers at default
		radio_write8(AX5043_REG_DECIMATION, decimation, hspi);
		radio_write8(AX5043_REG_RXDATARATE0, rxdatarate, hspi);
		radio_write8(AX5043_REG_RXDATARATE1, (rxdatarate >> 8), hspi);
		radio_write8(AX5043_REG_RXDATARATE2, (rxdatarate >> 16), hspi);
		radio_write8(AX5043_REG_IFFREQ0, iffreq,hspi);
		radio_write8(AX5043_REG_IFFREQ0, (iffreq >> 8), hspi);
		radio_write8(AX5043_REG_MAXRFOFFSET0, freqOffset, hspi);
		radio_write8(AX5043_REG_MAXRFOFFSET1, (freqOffset >> 8), hspi);
		radio_write8(AX5043_REG_MAXRFOFFSET2, (1<<8) | (freqOffset >> 16), hspi); // frequency correction at LO1
		radio_write8(AX5043_REG_AGCTARGET0, 0x89, hspi); // 0.75x max range of 512 to avoid ADC saturation but provide max performance
		radio_write8(AX5043_REG_AGCGAIN0, 0x74, hspi); // DECAY | ATTACK; F3db~bitrate, so 0x04 corresponds to 9636~9600bps
	}
	return AXRADIO_ERR_NOERROR;
}

uint8_t AX_Radio_FIFO_Routine(SPI_HandleTypeDef * hspi)
{
	uint8_t preamble = 0x55;
	uint8_t numBytesToSend = 100;
	char beeData[] = "According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible. Yellow, black. Yellow, black. Yellow, black. Yellow, black. Ooh, black and yellow! Let's shake it up a little. Barry! Breakfast is ready! Coming! Hang on a second. Hello? Barry? Adam? Can you believe this is happening? I can't. I'll pick you up. Looking sharp. Use the stairs, Your father paid good money for those. Sorry. I'm excited. Here's the graduate. We're very proud of you, son. A perfect report card, all B's. Very proud. Ma! I got a thing going here. You got lint on your fuzz. Ow! That's me! Wave to us! We'll be in row 118,000. Bye! Barry, I told you, stop flying in the house! Hey, Adam. Hey, Barry. Is that fuzz gel? A little. Special day, graduation. Never thought I'd make it. Three days grade school, three days high school. Those were awkward. Three days college. I'm glad I took a day and hitchhiked around The Hive. You did come back different. Hi, Barry. Artie, growing a mustache? Looks good. Hear about Frankie? Yeah. You going to the funeral? No, I'm not going. Everybody knows, sting someone, you die. Don't waste it on a squirrel. Such a hothead. I guess he could have just gotten out of the way. I love this incorporating an amusement park into our day. That's why we don't need vacations. Boy, quite a bit of pomp under the circumstances. Well, Adam, today we are men. We are! Bee-men. Amen! Hallelujah!";
	/* Radio Control for Sending Packets*/
//	radio_write8(AX5043_REG_PWRMODE, 0x67, hspi); // Enable FIFO
	while ((radio_read8(AX5043_REG_POWSTAT, hspi) & (1 << 3)) == 0); // wait for pwrmode change to settle
	radio_write8(AX5043_REG_FIFOSTAT, 0x03, hspi); // clear FIFO

	// preamble
	radio_write8(AX5043_REG_FIFODATA, 0x62, hspi); // REPEATDATA command
	radio_write8(AX5043_REG_FIFODATA, 0x18, hspi); // bypass framing & encoding, suppress CRC, no pkt start/end
	radio_write8(AX5043_REG_FIFODATA, 0x14, hspi); // repeat byte given in next command 20 times
	radio_write8(AX5043_REG_FIFODATA, preamble, hspi); // byte to be sent 10 times

	// frame
	radio_write8(AX5043_REG_FIFODATA, 0xE1, hspi); // TX Data command
	radio_write8(AX5043_REG_FIFODATA, (numBytesToSend+1), hspi); // 100 bytes +1 for control field byte
	radio_write8(AX5043_REG_FIFODATA, 0x03, hspi); // pkt start/end used here
	for (uint8_t i = 0; i < numBytesToSend; i++)
		radio_write8(AX5043_REG_FIFODATA, beeData[i], hspi);

	// commit
	radio_write8(AX5043_REG_FIFOSTAT, 0x04, hspi);

	// set to full tx
	radio_write8(AX5043_REG_PWRMODE,0x6D,hspi);
	while ((radio_read8(AX5043_REG_POWSTAT, hspi) & (1 << 3)) == 0);
	return AXRADIO_ERR_NOERROR;
}
