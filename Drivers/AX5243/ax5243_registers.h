/*
*  @file ax5243_registers.h
*  @brief Register definitions for AX5243 radio
*  @author Trevor Thomas
*  @note	Although there are references to the ax5043, the radios are identical
*  			from a programming standpoint
*/


#define AX5043_REG_MODULATION			0x010
#define AX5043_REG_ENCODING				0x011
#define AX5043_REG_FRAMING				0x012
#define AX5043_REG_PINFUNCSYSCLK		0x021
#define AX5043_REG_PINFUNCDCLK			0x022
#define AX5043_REG_PINFUNCDATA			0x023
#define AX5043_REG_PINFUNCANTSEL		0x025
#define AX5043_REG_PINFUNCPWRAMP		0x026
#define AX5043_REG_WAKEUPXOEARLY		0x06E
#define AX5043_REG_IFFREQ1				0x100
#define AX5043_REG_IFFREQ0				0x101
#define AX5043_REG_DECIMATION			0x102
#define AX5043_REG_RXDATARATE2			0x103
#define AX5043_REG_RXDATARATE1			0x104
#define AX5043_REG_RXDATARATE0			0x105
#define AX5043_REG_MAXDROFFSET2			0x106
#define AX5043_REG_MAXDROFFSET1			0x107
#define AX5043_REG_MAXDROFFSET0			0x108
#define AX5043_REG_MAXRFOFFSET2			0x109
#define AX5043_REG_MAXRFOFFSET1			0x10A
#define AX5043_REG_MAXRFOFFSET0			0x10B
#define AX5043_REG_FSKDMAX1				0x10C
#define AX5043_REG_FSKDMAX0				0x10D
#define AX5043_REG_FSKDMIN1				0x10E
#define AX5043_REG_FSKDMIN0				0x10F
#define AX5043_REG_AMPLFILTER			0x115
#define AX5043_REG_RXPARAMSETS			0x117
#define AX5043_REG_AGCGAIN0				0x120
#define AX5043_REG_AGCTARGET0			0x121
#define AX5043_REG_TIMEGAIN0			0x124
#define AX5043_REG_DRGAIN0				0x125
#define AX5043_REG_PHASEGAIN0			0x126
#define AX5043_REG_FREQUENCYGAINA0		0x127
#define AX5043_REG_FREQUENCYGAINB0		0x128
#define AX5043_REG_FREQUENCYGAINC0		0x129
#define AX5043_REG_FREQUENCYGAIND0		0x12A
#define AX5043_REG_AMPLITUDEGAIN0		0x12B
#define AX5043_REG_FREQDEV10			0x12C
#define AX5043_REG_FREQDEV00			0x12D
#define AX5043_REG_BBOFFSRES0			0x12F
#define AX5043_REG_AGCGAIN1				0x130
#define AX5043_REG_AGCTARGET1			0x131
#define AX5043_REG_AGCAHYST1			0x132
#define AX5043_REG_AGCMINMAX1			0x133
#define AX5043_REG_TIMEGAIN1			0x134
#define AX5043_REG_DRGAIN1				0x135
#define AX5043_REG_PHASEGAIN1			0x136
#define AX5043_REG_FREQUENCYGAINA1		0x137
#define AX5043_REG_FREQUENCYGAINB1		0x138
#define AX5043_REG_FREQUENCYGAINC1		0x139
#define AX5043_REG_FREQUENCYGAIND1		0x13A
#define AX5043_REG_AMPLITUDEGAIN1		0x13B
#define AX5043_REG_FREQDEV11			0x13C
#define AX5043_REG_FREQDEV01			0x13D
#define AX5043_REG_FOURFSK1				0x13E
#define AX5043_REG_BBOFFSRES1			0x13F
#define AX5043_REG_AGCGAIN3				0x150
#define AX5043_REG_AGCTARGET3			0x151
#define AX5043_REG_AGCAHYST3			0x152
#define AX5043_REG_AGCMINMAX3			0x153
#define AX5043_REG_TIMEGAIN3			0x154
#define AX5043_REG_DRGAIN3				0x155
#define AX5043_REG_PHASEGAIN3			0x156
#define AX5043_REG_FREQUENCYGAINA3		0x157
#define AX5043_REG_FREQUENCYGAINB3		0x158
#define AX5043_REG_FREQUENCYGAINC3		0x159
#define AX5043_REG_FREQUENCYGAIND3		0x15A
#define AX5043_REG_AMPLITUDEGAIN3		0x15B
#define AX5043_REG_FREQDEV13			0x15C
#define AX5043_REG_FREQDEV03			0x15D
#define AX5043_REG_FOURFSK3				0x15E
#define AX5043_REG_BBOFFSRES3			0x15F
#define AX5043_REG_MODCFGF				0x160
#define AX5043_REG_FSKDEV2				0x161
#define AX5043_REG_FSKDEV1				0x162
#define AX5043_REG_FSKDEV0				0x163
#define AX5043_REG_MODCFGA				0x164
#define AX5043_REG_TXRATE2				0x165
#define AX5043_REG_TXRATE1				0x166
#define AX5043_REG_TXRATE0				0x167
#define AX5043_REG_TXPWRCOEFFB1			0x16A
#define AX5043_REG_TXPWRCOEFFB0			0x16B
#define AX5043_REG_PLLVCOI				0x180
#define AX5043_REG_PLLRNGCLK			0x183
#define AX5043_REG_BBTUNE				0x188
#define AX5043_REG_BBOFFSCAP			0x189
#define AX5043_REG_PKTADDRCFG			0x200
#define AX5043_REG_PKTLENCFG			0x201
#define AX5043_REG_PKTLENOFFSET			0x202
#define AX5043_REG_PKTMAXLEN			0x203
#define AX5043_REG_MATCH0PAT3			0x210
#define AX5043_REG_MATCH0PAT2			0x211
#define AX5043_REG_MATCH0PAT1			0x212
#define AX5043_REG_MATCH0PAT0			0x213
#define AX5043_REG_MATCH0LEN			0x214
#define AX5043_REG_MATCH0MAX			0x216
#define AX5043_REG_MATCH1PAT1			0x218
#define AX5043_REG_MATCH1PAT0			0x219
#define AX5043_REG_MATCH1LEN			0x21C
#define AX5043_REG_MATCH1MAX			0x21E
#define AX5043_REG_TMGTXBOOST			0x220
#define AX5043_REG_TMGTXSETTLE			0x221
#define AX5043_REG_TMGRXBOOST			0x223
#define AX5043_REG_TMGRXSETTLE			0x224
#define AX5043_REG_TMGRXOFFSACQ			0x225
#define AX5043_REG_TMGRXCOARSEAGC		0x226
#define AX5043_REG_TMGRXRSSI			0x228
#define AX5043_REG_TMGRXPREAMBLE2		0x22A
#define AX5043_REG_RSSIABSTHR			0x22D
#define AX5043_REG_BGNDRSSITHR			0x22F
#define AX5043_REG_PKTCHUNKSIZE			0x230
#define AX5043_REG_PKTACCEPTFLAGS		0x233
#define AX5043_REG_DACVALUE1			0x330
#define AX5043_REG_DACVALUE0			0x331
#define AX5043_REG_DACCONFIG			0x332
#define AX5043_REG_REF					0xF0D
#define AX5043_REG_XTALOSC				0xF10
#define AX5043_REG_XTALAMPL				0xF11
#define AX5043_REG_0xF1C				0xF1C
#define AX5043_REG_0xF21				0xF21
#define AX5043_REG_0xF22				0xF22
#define AX5043_REG_0xF23				0xF23
#define AX5043_REG_0xF26				0xF26
#define AX5043_REG_0xF34				0xF34
#define AX5043_REG_0xF35				0xF35
#define AX5043_REG_0xF44				0xF44
#define AX5043_REG_PLLLOOP				0x1030
#define AX5043_REG_PLLCPI				0x1031
#define AX5043_REG_PLLVCODIV			0x1032
#define AX5043_REG_XTALCAP				0x1184
#define AX5043_REG_0xF00				0x1F00
#define AX5043_REG_0xF18				0x1F18
//There were some redefinitions here that had to be deleted
#define AX5043_REG_TMGRXAGC_(WOR)		0x3000
#define AX5043_REG_TMGRXPREAMBLE1_(WOR)	0x3001
#define AX5043_REG_PKTMISCFLAGS_(WOR)	0x3002
#define AX5043_REG_RSSIREFERENCE		0x3006

//Registers below had to be manually added, might be because I didn't configure receiver
#define AX5043_REG_TMGRXAGC				0x227
#define AX5043_REG_TMGRXPREAMBLE1		0x229
#define AX5043_REG_PKTMISCFLAGS			0x231
#define AX5043_REG_PWRMODE				0x002

#define AX5043_REG_FREQA3				0x034
#define AX5043_REG_FREQA2				0x035
#define AX5043_REG_FREQA1				0x036
#define AX5043_REG_FREQA0				0x037

#define AX5043_REG_PLLRANGINGA			0x033
#define AX5043_REG_PLLRANGINGB			0x03B
#define AX5043_REG_GPADCCTRL			0x300

#define AX5043_REG_GPADC13VALUE1		0x308
#define AX5043_REG_GPADC13VALUE0		0x309