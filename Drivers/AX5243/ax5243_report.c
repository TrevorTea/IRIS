/*
 *	@file ax5243_report.c
 *	@brief Source file for ax5243 report definitions
 *	@author Trevor Thomas
 *  Created on: Dec 27, 2022
 */

#include "ax5243_report.h"

char* RSSI_Report_To_String(struct RSSI_Report report) {
	char format[] = "%d%c: [ref: %d, sut: %d]\n";
	char *str = (char*) malloc(256 * sizeof(char));
	char *substr = (char*) malloc(32 * sizeof(char));
	sprintf(substr, format, 27, 'M', report.ref27, report.sut27);
	str = strcat(str, substr);
	sprintf(substr, format, 169, 'M', report.ref169, report.sut169);
	str = strcat(str, substr);
	sprintf(substr, format, 915, 'M', report.ref915, report.sut915);
	str = strcat(str, substr);
	sprintf(substr, format, 24, 'G', report.ref24, report.sut24);
	str = strcat(str, substr);
	str = strcat(str, asctime(&report.time_struct));

	free(substr);
	return str;
}

char* RSSI_Report_To_JSON_Object(struct RSSI_Report report) {
	char freq_format[] = "\"%d%c\" : {\n\"Ref RSSI\" : %d,\n\"Sut RSSI\" : %d\n}";
	char *str = (char*) malloc(512 * sizeof(char));
	char *objstr = (char*) malloc(128 * sizeof(char));
	sprintf(str, "{\n\"Timestamp\" : \"%s\",\n", asctime(&report.time_struct));
	sprintf(objstr, freq_format, 27, 'M', report.ref27, report.sut27);
	str = strcat(str, objstr);
	str = strcat(str, ",\n");
	sprintf(objstr, freq_format, 169, 'M', report.ref169, report.sut169);
	str = strcat(str, objstr);
	str = strcat(str, ",\n");
	sprintf(objstr, freq_format, 915, 'M', report.ref915, report.sut915);
	str = strcat(str, objstr);
	str = strcat(str, ",\n");
	sprintf(objstr, freq_format, 24, 'G', report.ref24, report.sut24);
	str = strcat(str, objstr);
	str = strcat(str, "\n}");

	free(objstr);
	return str;
}
