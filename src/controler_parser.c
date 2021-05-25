/*
** funkce které pomocí knihovny S7 vyčtou z bytového pole přečteného z PLC požadovaná data
** funkce je vždy pojmenována podle hodnot, které vyčítá, takto jsou pojmenována i v PLC
** k tomuto se využívá knihova s7lib
*/

#include "include/controler_parser.h"
#include "include/mapping.h"

static char * cp_get_flag(uint8_t *, uint32_t, uint8_t, char *, char * buffer);
static int * cp_get_time(uint8_t *, int);
static char * cp_get_time_string(uint8_t *, uint32_t);
static bool cp_is_primer_application_enable(uint8_t * buffer);
static bool cp_is_primer_inspection_enable(uint8_t * buffer);
static double prepare_real(uint8_t * buffer, int address);
static char * cp_get_metralight_zone_status(uint8_t * buffer, uint8_t zone_index);
static bool cp_is_glue_application_inspection_enable(uint8_t * buffer);

/************************************* public interface ******************************/
char * cp_get_vehicle_number(uint8_t * buffer)
{
	char * vehicle_number = malloc(sizeof(char)*(VEHICLE_NUMBER+1));
	memcpy(vehicle_number, (char*) (buffer+ADDR_VEHICLE_NUMBER), VEHICLE_NUMBER);
	vehicle_number[VEHICLE_NUMBER] = 0;

	return vehicle_number;
}

char * cp_get_job_number(uint8_t * buffer)
{
	char * job_number = malloc(sizeof(char) * (SIZE_JOB_NUMBER+1));

	memcpy(job_number, buffer+ADDR_JOB_NUMBER, SIZE_JOB_NUMBER);
	job_number[SIZE_JOB_NUMBER] = 0;

	return job_number;
}

char * cp_get_rear_window_type(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, ADDR_REAR_WINDOW_TYPE, CHARACTERS_REAR_WINDOW_TYPE);
}

char * cp_get_vehicle_model(uint8_t * buffer)
{
	switch(buffer[ADDR_VEHICLE_MODEL])
	{
		case T7_VEHICLE_MODEL_CONSTANT:
			return c_string_init("T7");

		case ID_BUZZ_VEHICLE_MODEL_CONSTANT:
			return c_string_init("ID.BUZZ");

		default:
			return c_string_init("NaN");
	}
}

char * cp_get_window_id(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_dword(buffer, ADDR_WINDOW_ID));
}

char * cp_get_time_from_primer_application(uint8_t * buffer)
{
	if(cp_is_primer_application_enable(buffer) == true)
		return cp_get_time_string(buffer, ADDR_PRIMER_APPLICATION_TIME);

	return c_string_init("NaN");
}

char * cp_get_primer_detection_with_vision_ok(uint8_t * buffer)
{
	if(cp_is_primer_inspection_enable(buffer) == true)
		return cp_get_flag(buffer, ADDR_PRIMER_INSPECTION_RESULT, INDEX_PRIMER_INSPECTION_RESULT, "true", "false");

	return c_string_init("NaN");
}

char * cp_get_primer_check_pass_fail_area(uint8_t * buffer)
{
	if(cp_is_primer_inspection_enable(buffer) == true)
	{
		return c_string_format("%s-%s-%s-%s",
													((buffer[ADDR_BEAD_CHECK_AREA] & 0b1) ? "true" : "false"),
													((buffer[ADDR_BEAD_CHECK_AREA] & 0b10) ? "true" : "false"),
													((buffer[ADDR_BEAD_CHECK_AREA] & 0b100) ? "true" : "false"),
													((buffer[ADDR_BEAD_CHECK_AREA] & 0b1000) ? "true" : "false"));
	}

	return c_string_init("NaN");
}

char * cp_get_primer_curring_rack(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, ADDR_DRAWER_INDEX));
}

char * cp_get_time_primer_flashoff_complete(uint8_t * buffer)
{
	if(cp_is_primer_application_enable(buffer) == true)
	return cp_get_time_string(buffer, ADDR_PRIMER_FLUSHOFF_TIME);

	return c_string_init("NaN");
}

char * cp_get_interval_from_primering_util_gluing(uint8_t * buffer)
{
	if(cp_is_primer_application_enable(buffer) == true)
	{
		int * primering_time = cp_get_time(buffer, ADDR_PRIMER_APPLICATION_TIME);
		int * gluing_time = cp_get_time(buffer, ADDR_GLUE_APPLICATION_START_TIME);

		uint64_t interval = cp_time_to_seconds(gluing_time) - cp_time_to_seconds(primering_time);

		free(primering_time);
		free(gluing_time);

		return c_string_format("%ld", interval);
	}

	return c_string_init("NaN");
}

char * cp_get_time_from_last_dispense(uint8_t * buffer)
{
	return cp_get_time_string(buffer, ADDR_TIME_SINCE_LAST_DISPENCSE);
}

char * cp_get_timestamp_of_glue_bead_application(uint8_t * buffer)
{
	return cp_get_time_string(buffer, ADDR_GLUE_APPLICATION_START_TIME);
}

char * cp_get_robot_finish_time_glue_bead_application(uint8_t * buffer)
{
	return cp_get_time_string(buffer, ADDR_GLUE_APPLICATION_END_TIME);
}

char * cp_get_interval_from_glue_application_util_output(uint8_t * buffer)
{
	int * time_from_start_glue_application = cp_get_time(buffer, ADDR_GLUE_APPLICATION_START_TIME);
	int * assembly_time = cp_get_time(buffer, ADDR_ASSEMBLY_TIME);

	uint64_t interval = cp_time_to_seconds(assembly_time) - cp_time_to_seconds(time_from_start_glue_application);

	free(time_from_start_glue_application);
	free(assembly_time);

	return c_string_format("%ld", interval);
}

char * cp_get_barrel_expire_ok_a(uint8_t * buffer)
{
	return cp_get_flag(buffer, ADDR_BARREL_A_EXPIRATION, INDEX_BARREL_A_EXPIRATION, "false", "true");
}

char * cp_get_adhesive_batch_id_a(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, ADDR_BARREL_A_BATCH_NUMBER, CHARACTERS_BARREL_A_BATCH_NUMBER);
}

char * cp_get_adhesive_serial_a(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, ADDR_BARREL_A_SERIAL_NUMBER, CHARACTERS_BARREL_A_SERIAL_NUMBER);
}

char * cp_get_material_dispensed_a(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, ADDR_APPLIED_GLUE_AMOUNT_A)*10.0)/10.0);
}

char * cp_get_pistol_temperature_min_value(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, ADDR_PISTOL_TEMPERATURE_UPPER_LIMIT));
}

char * cp_get_pistol_temperature(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, ADDR_PISTOL_TEMPERATURE_DURING_APPLICAITON)*10.0)/10.0);
}

char * cp_get_pistol_temperature_max_value(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, ADDR_PISTOL_TEMPERATURE_UPPER_LIMIT));
}

char * cp_get_pot_temperature_min_value_a(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, ADDR_PISTOL_TEMPERATURE_LOWER_LIMIT));
}

char * cp_get_pot_temperature_actual_a(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, ADDR_A_POT_TEMPERATURE_DURING_APPLICATION)*10.0)/10.0);
}

char * cp_get_pot_temperature_max_value_a(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, ADDR_A_POT_TEMPERATURE_UPPER_LIMIT));
}

char * cp_get_barrel_expire_ok_b(uint8_t * buffer)
{
	return cp_get_flag(buffer, ADDR_BARREL_B_EXPIRATION, INDEX_BARREL_B_EXPIRATION, "false", "true");
}

char * cp_get_adhesive_batch_b(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, ADDR_BARREL_B_BATCH_NUMBER, CHARACTERS_BARREL_B_BATCH_NUMBER);
}

char * cp_get_adhesive_serial_b(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, ADDR_BARREL_B_SERIAL_NUMBER, CHARACTERS_BARREL_B_SERIAL_NUMBER);
}

char * cp_get_material_dispensed_b(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, ADDR_APPLIED_GLUE_AMOUNT_B)*10.0)/10.0);
}

char * cp_get_application_ratio(uint8_t * buffer)
{
	return c_string_format("%.01f:%.01f",
												prepare_real(buffer, ADDR_APPLICATION_RATIO_A),
												prepare_real(buffer, ADDR_APPLICATION_RATIO_B));
}

char * cp_get_mixer_tube_life(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_dword(buffer, ADDR_MIXER_TUBE_LIFE));
}

char * cp_get_robot_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, ADDR_ROBOT_COMPLETE_CYCLE_WITHOUT_FAULT, INDEX_ROBOT_COMPLETE_CYCLE_WITHOUT_FAULT, "true", "false");
}

char * cp_get_dispense_unit_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, ADDR_DISPENSE_UNIT_COMPLETE_WITHOUT_FAULT, INDEX_DISPENSE_UNIT_COMPLETE_WITHOUT_FAULT, "true", "false");
}

char * cp_get_rotary_unit_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, ADDR_ROTARY_UNIT_CYCLE_WITHOUT_FAULT, INDEX_ROTARY_UNIT_CYCLE_WITHOUT_FAULT, "true", "false");
}

char * cp_get_adhesive_application_complete_summary(uint8_t * buffer)
{
	return cp_get_flag(buffer, ADDR_ADHESIVE_PROCESS_COMPLETE_SUMMARY, INDEX_ADHESIVE_PROCESS_COMPLETE_SUMMARY, "true", "false");
}

char * cp_get_bead_check_laser_sensor(uint8_t * buffer)
{
		return cp_get_flag(buffer, ADDR_ADHESIVE_PROCESS_COMPLETE_SUMMARY, INDEX_ADHESIVE_PROCESS_COMPLETE_SUMMARY, "true", "false");
}

char * cp_get_humidity(uint8_t * buffer)
{
	return c_string_format("%.01f",prepare_real(buffer, ADDR_AMBIENT_HUMIDITY));
}

char * cp_get_ambient_temperature(uint8_t * buffer)
{
	return c_string_format("%.01f", prepare_real(buffer, ADDR_AMBIENT_TEMPERATURE));
}

char * cp_get_metralight_result(uint8_t * buffer)
{
	if(cp_is_glue_application_inspection_enable(buffer) == true)
		return cp_get_flag(buffer, ADDR_GLUE_APPLICATION_RESULT, INDEX_GLUE_APPLICATION_RESULT, "true", "false");

	return c_string_init("NaN");
}

char * cp_get_metralight_zone1_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE1_STATUS_OFFSET);
}

char * cp_get_metralight_zone2_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE2_STATUS_OFFSET);
}

char * cp_get_metralight_zone3_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE3_STATUS_OFFSET);
}

char * cp_get_metralight_zone4_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE4_STATUS_OFFSET);
}

char * cp_get_metralight_zone5_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE5_STATUS_OFFSET);
}

char * cp_get_metralight_zone6_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE6_STATUS_OFFSET);
}

char * cp_get_metralight_zone7_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE7_STATUS_OFFSET);
}

char * cp_get_metralight_zone8_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE8_STATUS_OFFSET);
}

char * cp_get_metralight_zone9_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE9_STATUS_OFFSET);
}

char * cp_get_metralight_zone10_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE10_STATUS_OFFSET);
}

char * cp_get_metralight_zone11_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE11_STATUS_OFFSET);
}

char * cp_get_metralight_zone12_status(uint8_t * buffer)
{
	return cp_get_metralight_zone_status(buffer, METRALIGHT_ZONE12_STATUS_OFFSET);
}

/***************************** private interface ****************************/

static char * cp_get_metralight_zone_status(uint8_t * buffer, uint8_t zone_index)
{
	if(cp_is_glue_application_inspection_enable(buffer) == true)
	{
		switch(buffer[ADDR_METRALIGHT_ZONES_STATUS+zone_index])
		{
			case 0x10:
				return c_string_init("OK");
			case 0x20:
				return c_string_init("NOK");
			default:
				return c_string_init("ERR");
		}
	}

	return c_string_init("NaN");
}

static int * cp_get_time(uint8_t * buffer, int byte_index)
{
	int * time = malloc(sizeof(int)*6);

	//year
	time[0] = s7lib_parser_read_int(buffer, byte_index);

	//month
	time[1] = buffer[byte_index+2];

	// day
	time[2] = buffer[byte_index+3];

	//hour
	time[3] = buffer[byte_index+5];

	//minute
	time[4] = buffer[byte_index+6];

	//second
	time[5] = buffer[byte_index+7];

	return time;
}

static char * cp_get_time_string(uint8_t * buffer, uint32_t byte_index)
{
	int * time = cp_get_time(buffer, byte_index);
	char * time_string =  c_string_format("%d-%02d-%02d %02d:%02d:%02d", time[0], time[1], time[2], time[3], time[4], time[5]);

	free(time);

	return time_string;
}

uint64_t cp_time_to_seconds(int * time)
{
	return ((uint64_t)(((((time[0] > 0) ? time[0]-1970 : 0))*31556925.216) + (time[1]*30.4368499*86400) + (time[2]*86400) + (time[3]*3600) + (time[4]*60) + time[5]));
}

static char * cp_get_flag(uint8_t * buffer, uint32_t byte_index, uint8_t bit_index, char * option_ok, char * option_nok)
{
	if(s7lib_parser_read_bool(buffer, byte_index, bit_index) == true)
		return c_string_init(option_ok);
	else
		return c_string_init(option_nok);
}

static bool cp_is_glue_application_inspection_enable(uint8_t * buffer)
{
	return s7lib_parser_read_bool(buffer, ADDR_GLUE_APP_INSPECTION_ENABLE, INDEX_GLUE_APP_INSPECTION_ENABLE);
}

static bool cp_is_primer_application_enable(uint8_t * buffer)
{
	return s7lib_parser_read_bool(buffer, ADDR_PRIMERING_PROCESS_ENABLED, INDEX_PRIMERING_PROCESS_ENABLED);
}

static bool cp_is_primer_inspection_enable(uint8_t * buffer)
{
	return s7lib_parser_read_bool(buffer, ADDR_PRIMER_INSPECTION_ENABLED, INDEX_PRIMER_INSPECTION_ENABLED);
}

static double prepare_real(uint8_t * buffer, int address)
{
	return floor(s7lib_parser_read_real(buffer, address)*10.0)/10.0;
}
