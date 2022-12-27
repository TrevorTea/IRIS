/*
*  @file ax5243_macros.h
*  @brief Macro and misc definitions for AX5243 radio
*  @author Trevor Thomas
*  @note	Although there are references to the ax5043, the radios are identical
*  			from a programming standpoint
*/

#ifndef AX5243_AX5243_MACROS_H_
#define AX5243_AX5243_MACROS_H_

// power states
#define AX5043_PWRSTATE_POWERDOWN           0x0
#define AX5043_PWRSTATE_DEEPSLEEP           0x1
#define AX5043_PWRSTATE_REGS_ON             0x4
#define AX5043_PWRSTATE_XTAL_ON             0x5
#define AX5043_PWRSTATE_FIFO_ON             0x7
#define AX5043_PWRSTATE_SYNTH_RX            0x8
#define AX5043_PWRSTATE_FULL_RX             0x9
#define AX5043_PWRSTATE_WOR_RX              0xb
#define AX5043_PWRSTATE_SYNTH_TX            0xc
#define AX5043_PWRSTATE_FULL_TX             0xd

//fifo commands
#define AX5043_FIFOCMD_NOP                  0x00
#define AX5043_FIFOCMD_DATA                 0x01
#define AX5043_FIFOCMD_REPEATDATA           0x02
#define AX5043_FIFOCMD_TIMER                0x10
#define AX5043_FIFOCMD_RSSI                 0x11
#define AX5043_FIFOCMD_FREQOFFS             0x12
#define AX5043_FIFOCMD_RFFREQOFFS           0x13
#define AX5043_FIFOCMD_DATARATE             0x14
#define AX5043_FIFOCMD_ANTRSSI              0x15
#define AX5043_FIFOCMD_TXCTRL               0x1C
#define AX5043_FIFOCMD_TXPWR                0x1D

//errors
#define AXRADIO_ERR_NOERROR                 0x00
#define AXRADIO_ERR_NOTSUPPORTED            0x01
#define AXRADIO_ERR_BUSY                    0x02
#define AXRADIO_ERR_TIMEOUT                 0x03
#define AXRADIO_ERR_INVALID                 0x04
#define AXRADIO_ERR_NOCHIP                  0x05
#define AXRADIO_ERR_RANGING                 0x06
#define AXRADIO_ERR_LOCKLOST                0x07
#define AXRADIO_ERR_RETRANSMISSION          0x08
#define AXRADIO_ERR_RESYNC                  0x09
#define AXRADIO_ERR_RESYNCTIMEOUT           0x0a
#define AXRADIO_ERR_RECEIVESTART            0x0b

// RF settings
#define PLLVCODIV_RFDIV_ON					0x04
#define PLLVCODIV_RFDIV_OFF					0x00
#define PLLVCODIV_INTERNAL_INDUCTOR			0x00
#define PLLVCODIV_EXTERNAL_INDUCTOR			0x30

#define F34_RFDIV_ON						0x28
#define F34_RFDIV_OFF						0x08



#endif /* AX5243_AX5243_MACROS_H_ */
