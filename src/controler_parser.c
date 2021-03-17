#include "include/controler_parser.h"

static char * cp_get_flag(uint8_t *, uint32_t, uint8_t, char *, char * buffer);
static int * cp_get_time(uint8_t *, int);
static char * cp_get_time_string(uint8_t *, uint32_t);
static bool cp_get_primer_application_enable(uint8_t * buffer);
static bool cp_get_primer_inspection_enable(uint8_t * buffer);



/************************************* public interface ******************************/
char * cp_get_vehicle_number(uint8_t * buffer)
{
	char * vehicle_number = malloc(sizeof(char)*14);
	memcpy(vehicle_number, (char*) (buffer+27), 13);
	vehicle_number[13] = 0;

	return vehicle_number;
}

char * cp_get_job_number(uint8_t * buffer)
{
	char * job_number = malloc(sizeof(char) * 11);

	memcpy(job_number, buffer+1, 10);
	job_number[10] = 0;

	return job_number;
}

char * cp_get_rear_window_type(uint8_t * buffer)
{
  return s7lib_parser_read_string(buffer, 41, 18);
}

char * cp_get_vehicle_model(uint8_t * buffer)
{
	if(buffer[61] == T7_VEHICLE_MODEL_CONSTANT)
  		return c_string_init("T7");
	else if(buffer[61] == ID_BUZZ_VEHICLE_MODEL_CONSTANT)
		return c_string_init("ID.BUZZ");
	else 
		return c_string_init("NA");
}

char * cp_get_window_id(uint8_t * buffer)
{
  return c_string_format("%d", s7lib_parser_read_dword(buffer, 63));
}

char * cp_get_time_from_primer_application(uint8_t * buffer)
{
	if(cp_get_primer_application_enable(buffer) == true)
		return cp_get_time_string(buffer, 73);

	return c_string_init("NA");
}

char * cp_get_primer_detection_with_vision_ok(uint8_t * buffer)
{
	if(cp_get_primer_inspection_enable(buffer) == true)
		return cp_get_flag(buffer, 161, 1, "PASS", "FAIL");

	return c_string_init("NA");
}

char * cp_get_bead_check_pass_fail_area(uint8_t * buffer)
{
	if(cp_get_primer_inspection_enable(buffer) == true)
	{
		return c_string_format("%s-%s-%s-%s", ((buffer[145] & 0b1) ? "PASS" : "FAIL"),
												((buffer[145] & 0b10) ? "PASS" : "FAIL"),
												((buffer[145] & 0b100) ? "PASS" : "FAIL"),
												((buffer[145] & 0b1000) ? "PASS" : "FAIL"));
	}

	return c_string_init("NA");
}

char * cp_get_primer_curring_rack(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, 147));
}

char * cp_get_time_primer_flashoff_complete(uint8_t * buffer)
{
	if(cp_get_primer_application_enable(buffer) == true)
		return cp_get_time_string(buffer, 85);

	return c_string_init("NA");
}

char * cp_get_interval_from_primering_util_gluing(uint8_t * buffer)
{
	if(cp_get_primer_application_enable(buffer) == true)
	{
		int * primering_time = cp_get_time(buffer, 73);
		int * gluing_time = cp_get_time(buffer, 97);

		uint64_t interval = cp_time_to_seconds(gluing_time) - cp_time_to_seconds(primering_time);

		free(primering_time);
		free(gluing_time);

		return c_string_format("%ld", interval);
	}	

	return c_string_init("NA");
}

char * cp_get_time_from_last_dispense(uint8_t * buffer)
{
	return cp_get_time_string(buffer, 133);
}

char * cp_get_timestamp_of_glue_bead_application(uint8_t * buffer)
{
	return cp_get_time_string(buffer, 97);
}

char * cp_get_robot_finish_time_glue_bead_application(uint8_t * buffer)
{

	return cp_get_time_string(buffer, 109);
}

char * cp_get_interval_from_glue_application_util_output(uint8_t * buffer)
{
	int * time_from_start_glue_application = cp_get_time(buffer,97);
	int * assembly_time = cp_get_time(buffer, 121);

	uint64_t interval = cp_time_to_seconds(assembly_time) - cp_time_to_seconds(time_from_start_glue_application);

	free(time_from_start_glue_application);
	free(assembly_time);

	return c_string_format("%ld", interval);
}

char * cp_get_barrel_expire_ok_a(uint8_t * buffer)
{
	return cp_get_flag(buffer, 234,0, "nok", "ok");
}

char * cp_get_adhesive_batch_id_a(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 195, 16);
}

char * cp_get_adhesive_serial_a(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 213, 16);
}

char * cp_get_material_dispensed_a(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 179)*10.0)/10.0);
}

char * cp_get_pistol_temperature_min_value(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, 149));
}

char * cp_get_pistol_temperature(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 171)*10.0)/10.0);
}

char * cp_get_pistol_temperature_max_value(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, 151));
}

char * cp_get_pot_temperature_min_value_a(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, 153));
}

char * cp_get_pot_temperature_actual_a(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 175)*10.0)/10.0);
}

char * cp_get_pot_temperature_max_value_a(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_int(buffer, 155));
}

char * cp_get_barrel_expire_ok_b(uint8_t * buffer)
{
	return cp_get_flag(buffer, 274, 0, "nok", "ok");
}

char * cp_get_adhesive_batch_b(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 235, 16);
}

char * cp_get_adhesive_serial_b(uint8_t * buffer)
{
	return s7lib_parser_read_string(buffer, 253, 16);
}

char * cp_get_material_dispensed_b(uint8_t * buffer)
{
	return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 183)*10.0)/10.0);
}

char * cp_get_application_ratio(uint8_t * buffer)
{
	return c_string_format("%.01f:%.01f", floor(s7lib_parser_read_real(buffer, 163)*10.0)/10.0, floor(s7lib_parser_read_real(buffer, 167)*10.0)/10.0);
}

char * cp_get_mixer_tube_life(uint8_t * buffer)
{
	return c_string_format("%d", s7lib_parser_read_dword(buffer, 157));
}

char * cp_get_robot_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, 161, 5, "true", "false");
}

char * cp_get_dispense_unit_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, 161, 6, "true", "false");
}

char * cp_get_rotary_unit_completed_cycle_without_fault(uint8_t * buffer)
{
	return cp_get_flag(buffer, 161, 7, "true", "false");
}

char * cp_get_adhesive_application_complete_summary(uint8_t * buffer)
{
	return cp_get_flag(buffer, 162, 0, "true", "false");
}

bool cp_get_glue_application_inspection_enable(uint8_t * buffer)
{
	return s7lib_parser_read_bool(buffer, 161, 3);
}

char * cp_get_bead_check_laser_sensor(uint8_t * buffer)
{
	if(cp_get_glue_application_inspection_enable(buffer) == true)
		return cp_get_flag(buffer, 161, 2, "PASS", "FAIL");

	return c_string_init("NA");
}

char * cp_get_humidity(uint8_t * buffer)
{
  return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 187)*10.0)/10.0);
}

char * cp_get_ambient_temperature(uint8_t * buffer)
{
  return c_string_format("%.01f", floor(s7lib_parser_read_real(buffer, 191)*10.0)/10.0);
}


/***************************** private interface ****************************/

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
    return ((uint64_t)(((((time[0]>0) ? time[0]-1970 : 0))*31556925.216) + (time[1]*30.4368499*86400) + (time[2]*86400) + (time[3]*3600) + (time[4]*60) + time[5]));
}

static char * cp_get_flag(uint8_t * buffer, uint32_t byte_index, uint8_t bit_index, char * option_ok, char * option_nok)
{
  if(s7lib_parser_read_bool(buffer, byte_index, bit_index) == true)
    return c_string_init(option_ok);
  else
    return c_string_init(option_nok);
}

static bool cp_get_primer_application_enable(uint8_t * buffer)
{
	return s7lib_parser_read_bool(buffer, 163, 2);
}

static bool cp_get_primer_inspection_enable(uint8_t * buffer)
{
	return s7lib_parser_read_bool(buffer, 163,3);
}
