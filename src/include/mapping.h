/*
** Mapování tagů z PLC,
** využívá se pro adresování v souboru controler_parser.c
*/

#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <s7lib_allocation.h>

/* pomocná makra */
#define ADDR_BASE_OFFSET 1
#define ADDRESS(DB_ADDRESS)(DB_ADDRESS-ADDR_BASE_OFFSET)

/* mapování do komunikačního DB */
#define ADDR_JOB_NUMBER ADDRESS(2)
#define SIZE_JOB_NUMBER SIZEOF_CHAR*10

#define ADDR_CALLOF_DATE_TIME ADDRESS(12)
#define SIZE_CALLOF_DATE_TIME SIZEOF_CHAR*16

#define ADDR_VEHICLE_NUMBER ADDRESS(28)
#define VEHICLE_NUMBER SIZEOF_CHAR*13

#define ADDR_REAR_WINDOW_TYPE ADDRESS(42)
#define CHARACTERS_REAR_WINDOW_TYPE 18 //počet znaků v řetězci
#define REAR_WINDOW_TYPE SIZEOF_STRING(REAR_WINDOW_TYPE_CHARACTERS) //zkutečná velikost v db

#define ADDR_VEHICLE_MODEL ADDRESS(62)
#define VEHICLE_MODEL SIZEOF_BYTE

#define ADDR_WINDOW_ID ADDRESS(64)
#define WINDOW_ID SIZEOF_UDINT

#define ADDR_PRODUCTION_SEQ ADDRESS(68)
#define PRODUCTION_SEQ SIZEOF_CHAR*6

#define ADDR_PRIMER_APPLICATION_TIME ADDRESS(74)
#define PRIMER_APPLICATION_TIME SIZEOF_DTL

#define ADDR_PRIMER_FLUSHOFF_TIME ADDRESS(86)
#define PRIMER_FLUSHOFF_TIME SIZEOF_DTL

#define ADDR_GLUE_APPLICATION_START_TIME ADDRESS(98)
#define GLUE_APPLICATION_START_TIME SIZEOF_DTL

#define ADDR_GLUE_APPLICATION_END_TIME ADDRESS(110)
#define GLUE_APPLICATION_END_TIME SIZEOF_DTL

#define ADDR_ASSEMBLY_TIME ADDRESS(122)
#define ASSEMBLY_TIME SIZEOF_DTL

#define ADDR_TIME_SINCE_LAST_DISPENCSE ADDRESS(134)
#define TIME_FROM_LAST_DISPENSE SIZEOF_DTL

#define ADDR_BEAD_CHECK_AREA ADDRESS(146)
#define BEAD_CHECK_AREA SIZEOF_BYTE

#define ADDR_DRAWER_INDEX ADDRESS(148)
#define DRAWER_INDEX SIZEOF_INT

#define ADDR_PISTOL_TEMPERATURE_UPPER_LIMIT ADDRESS(150)
#define PISTOL_TEMPERATURE_UPPER_LIMIT SIZEOF_INT

#define ADDR_PISTOL_TEMPERATURE_LOWER_LIMIT ADDRESS(152)
#define PISTOL_TEMPERATURE_LOWER_LIMIT SIZEOF_INT

#define ADDR_A_POT_TEMPERATURE_UPPER_LIMIT ADDRESS(154)
#define A_POT_TEMPERATURE_UPPER_LIMIT SIZEOF_INT

#define ADDR_A_POT_TEMPERATURE_LOWER_LIMIT ADDRESS(156)
#define A_POT_TEMPERATURE_LOWER_LIMIT SIZEOF_INT

#define ADDR_MIXER_TUBE_LIFE ADDRESS(158)
#define MIXER_TUBE_LIFE SIZEOF_DINT

#define ADDR_PRIMER_INSPECTION_BYPASS ADDRESS(162)
#define INDEX_PRIMER_INSPECTION_BYPASS 0

#define ADDR_PRIMER_INSPECTION_RESULT ADDRESS(162)
#define INDEX_PRIMER_INSPECTION_RESULT 1

#define ADDR_GLUE_APPLICATION_RESULT ADDRESS(162)
#define INDEX_GLUE_APPLICATION_RESULT 2

#define ADDR_GLUE_APP_INSPECTION_ENABLE ADDRESS(162)
#define INDEX_GLUE_APP_INSPECTION_ENABLE 3

#define ADDR_GLUE_APPLIED ADDRESS(162)
#define INDEX_GLUE_APPLIED 4

#define ADDR_ROBOT_COMPLETE_CYCLE_WITHOUT_FAULT ADDRESS(162)
#define INDEX_ROBOT_COMPLETE_CYCLE_WITHOUT_FAULT 5

#define ADDR_DISPENSE_UNIT_COMPLETE_WITHOUT_FAULT ADDRESS(162)
#define INDEX_DISPENSE_UNIT_COMPLETE_WITHOUT_FAULT 6

#define ADDR_ROTARY_UNIT_CYCLE_WITHOUT_FAULT ADDRESS(162)
#define INDEX_ROTARY_UNIT_CYCLE_WITHOUT_FAULT 7

#define ADDR_ADHESIVE_PROCESS_COMPLETE_SUMMARY ADDRESS(163)
#define INDEX_ADHESIVE_PROCESS_COMPLETE_SUMMARY 0

#define ADDR_GLASS_DELETED ADDRESS(163)
#define INDEX_GLASS_DELETED 1

#define ADDR_PRIMER_INSPECTION_ENABLED ADDRESS(163)
#define INDEX_PRIMER_INSPECTION_ENABLED 2

#define ADDR_PRIMERING_PROCESS_ENABLED ADDRESS(163)
#define INDEX_PRIMERING_PROCESS_ENABLED 3

#define ADDR_APPLICATION_RATIO_A ADDRESS(164)
#define APPLICATION_RATIO SIZEOF_REAL

#define ADDR_APPLICATION_RATIO_B ADDRESS(168)
#define APPLICATION_RATIO_B SIZEOF_REAL

#define ADDR_PISTOL_TEMPERATURE_DURING_APPLICAITON ADDRESS(172)
#define PISTOL_TEMPERATURE_DURING_APPLICATION SIZEOF_REAL

#define ADDR_A_POT_TEMPERATURE_DURING_APPLICATION ADDRESS(176)
#define A_POT_TEMPERATURE_DURING_APPLICATION SIZEOF_REAL

#define ADDR_APPLIED_GLUE_AMOUNT_A ADDRESS(180)
#define APPLIED_GLUE_AMOUNT_A SIZEOF_REAL

#define ADDR_APPLIED_GLUE_AMOUNT_B ADDRESS(184)
#define APPLIED_GLUE_AMOUNT_B SIZEOF_REAL

#define ADDR_AMBIENT_HUMIDITY ADDRESS(188)
#define AMBIENT_HUMIDITY SIZEOF_REAL

#define ADDR_AMBIENT_TEMPERATURE ADDRESS(192)
#define AMBIENT_TEMPERATURE SIZEOF_REAL

#define ADDR_METRALIGHT_ZONES_STATUS ADDRESS(196)
#define METRALIGHT_ZONE1_STATUS_OFFSET 0
#define METRALIGHT_ZONE2_STATUS_OFFSET 1
#define METRALIGHT_ZONE3_STATUS_OFFSET 2
#define METRALIGHT_ZONE4_STATUS_OFFSET 3
#define METRALIGHT_ZONE5_STATUS_OFFSET 4
#define METRALIGHT_ZONE6_STATUS_OFFSET 5
#define METRALIGHT_ZONE7_STATUS_OFFSET 6
#define METRALIGHT_ZONE8_STATUS_OFFSET 7
#define METRALIGHT_ZONE9_STATUS_OFFSET 8
#define METRALIGHT_ZONE10_STATUS_OFFSET 9
#define METRALIGHT_ZONE11_STATUS_OFFSET 10
#define METRALIGHT_ZONE12_STATUS_OFFSET 11
#define METRALIGHT_ZONES_STATUS (SIZEOF_BYTE*12)

#define ADDR_BARREL_A_BATCH_NUMBER ADDRESS(208)
#define CHARACTERS_BARREL_A_BATCH_NUMBER 16
#define BARREL_A_BATCH_NUMBER SIZEOF_STRING(CHARACTERS_BARREL_A_BATCH_NUMBER)

#define ADDR_BARREL_A_SERIAL_NUMBER ADDRESS(226)
#define CHARACTERS_BARREL_A_SERIAL_NUMBER 16
#define BARREL_A_SERIAL_NUMBER SIZEOF_STRING(CHARACTERS_BARREL_A_SERIAL_NUMBER)

#define ADDR_BARREL_A_EXPIRATION_YEAR ADDRESS(244)
#define BARREL_A_EXPIRATION_YEAR SIZEOF_UINT

#define ADDR_BARREL_A_EXPIRATION_MONTH ADDRESS(246)
#define BARREL_A_EXPIRATION_MONTH SIZEOF_BYTE

#define ADDR_BARREL_A_EXPIRATION ADDRESS(247)
#define INDEX_BARREL_A_EXPIRATION 0

#define ADDR_BARREL_B_BATCH_NUMBER ADDRESS(248)
#define CHARACTERS_BARREL_B_BATCH_NUMBER 16
#define BARREL_B_BATCH_NUMBER SIZEOF_STRING(CHARACTERS_BARREL_A_BATCH_NUMBER)

#define ADDR_BARREL_B_SERIAL_NUMBER ADDRESS(266)
#define CHARACTERS_BARREL_B_SERIAL_NUMBER 16
#define BARREL_B_SERIAL_NUMBER SIZEOF_STRING(CHARACTERS_BARREL_A_SERIAL_NUMBER)

#define ADDR_BARREL_B_EXPIRATION_YEAR ADDRESS(284)
#define BARREL_B_EXPIRATION_YEAR SIZEOF_UINT

#define ADDR_BARREL_B_EXPIRATION_MONTH ADDRESS(286)
#define BARREL_B_EXPIRATION_MONTH SIZEOF_BYTE

#define ADDR_BARREL_B_EXPIRATION ADDRESS(287)
#define INDEX_BARREL_B_EXPIRATION 0









#endif
