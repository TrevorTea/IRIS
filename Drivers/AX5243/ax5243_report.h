/*
 *	@file ax5243_report.h
 *	@brief Header file for ax5243 report definitions
 *	@author Trevor Thomas
 *  Created on: Dec 27, 2022
 */

/**
 * @brief Structure for storing RSSI report types
 * @member time_struct c lib time structure for holding time and date
 * 		report was generated
 * @member ref* referential RSSI value
 * @member sut* structure under test RSSI value
 */

#ifndef AX5243_AX5243_REPORT_H_
#define AX5243_AX5243_REPORT_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

struct RSSI_Report {
	struct tm time_struct;
	uint8_t ref27, ref169, ref915, ref24;
	uint8_t sut27, sut169, sut915, sut24;
};

/**
 * @brief Generates a string representing an RSSI report
 * @param report RSSI report structure
 * @retval pointer to formatted string
 * @note Remember to deallocate string when finished
 */
char* RSSI_Report_To_String(struct RSSI_Report report);
#endif
