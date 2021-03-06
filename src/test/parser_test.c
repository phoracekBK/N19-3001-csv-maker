#include "parser_test.h"

#include <stdbool.h>
#include <s7lib_parser.h>

#include "../include/controler_parser.h"
#include "../include/config.h"
#include "../include/csv_io.h"



static void cp_get_vehicle_number_test();
static void cp_get_rear_window_type_test();
static void cp_get_vehicle_model_test();
static void cp_get_window_id_test();
static void cp_get_time_from_primer_application_test();
static void cp_get_time_from_primer_application_process_off_test();
static void cp_get_primer_detection_with_vision_ok_test();
static void cp_get_primer_detection_with_vision_ok_control_off_test();
static void cp_get_bead_check_pass_fail_area_test();
static void cp_get_bead_check_pass_fail_area_process_off_test();
static void cp_get_primer_curring_rack_test();
static void cp_get_time_primer_flashoff_complete_test();
static void cp_get_time_primer_flashoff_complete_process_off_test();
static void cp_get_interval_from_primering_util_gluing_test();		
static void cp_get_interval_from_primering_util_gluing_process_off_test();
static void cp_get_time_from_last_dispense_test();
static void cp_get_timestamp_of_glue_bead_application_test();
static void cp_get_robot_finish_time_glue_bead_application_test();
static void cp_get_interval_from_glue_application_util_output_test();
static void cp_get_barrel_expire_ok_a_test();
static void cp_get_adhesive_batch_id_a_test();
static void cp_get_adhesive_serial_a_test();
static void cp_get_material_dispensed_a_test();
static void cp_get_pistol_temperature_min_value_test();
static void cp_get_pistol_temperature_test();
static void cp_get_pistol_temperature_max_value_test();
static void cp_get_pot_temperature_min_value_a_test();
static void cp_get_pot_temperature_actual_a_test();
static void cp_get_pot_temperature_max_value_a_test();
static void cp_get_barrel_expire_ok_b_test();
static void cp_get_adhesive_batch_b_test();
static void cp_get_adhesive_serial_b_test();
static void cp_get_material_dispensed_b_test();
static void cp_get_application_ratio_test();
static void cp_get_mixer_tube_life_test();
static void cp_get_robot_completed_cycle_without_fault_test();
static void cp_get_dispense_unit_completed_cycle_without_fault_test();
static void cp_get_rotary_unit_completed_cycle_without_fault_test();
static void cp_get_adhesive_application_complete_summary_test();
static void cp_get_bead_check_laser_sensor_test();
static void cp_get_humidity_test();
static void cp_get_ambient_temperature_test();
static void csv_io_generate_csv_name_test();
static uint8_t * ut_get_buffer();



void unit_test_run()
{

	 cp_get_vehicle_number_test();
	 cp_get_rear_window_type_test();
	 cp_get_vehicle_model_test();
	 cp_get_window_id_test();
	 cp_get_time_from_primer_application_test();
	 cp_get_time_from_primer_application_process_off_test();
	 cp_get_primer_detection_with_vision_ok_test();
	 cp_get_primer_detection_with_vision_ok_control_off_test();
	 cp_get_bead_check_pass_fail_area_test();	
	 cp_get_bead_check_pass_fail_area_process_off_test();
	 cp_get_primer_curring_rack_test();
	 cp_get_time_primer_flashoff_complete_test();
	 cp_get_time_primer_flashoff_complete_process_off_test();
	 cp_get_interval_from_primering_util_gluing_test();		
	 cp_get_interval_from_primering_util_gluing_process_off_test();
	 cp_get_time_from_last_dispense_test();
	 cp_get_timestamp_of_glue_bead_application_test();
	 cp_get_robot_finish_time_glue_bead_application_test();
	 cp_get_interval_from_glue_application_util_output_test();
	 cp_get_barrel_expire_ok_a_test();
	 cp_get_adhesive_batch_id_a_test();
	 cp_get_adhesive_serial_a_test();
	 cp_get_material_dispensed_a_test();
	 cp_get_pistol_temperature_min_value_test();
	 cp_get_pistol_temperature_test();
	 cp_get_pistol_temperature_max_value_test();
	 cp_get_pot_temperature_min_value_a_test();
	 cp_get_pot_temperature_actual_a_test();
	 cp_get_pot_temperature_max_value_a_test();
	 cp_get_barrel_expire_ok_b_test();
	 cp_get_adhesive_batch_b_test();
	 cp_get_adhesive_serial_b_test();
	 cp_get_material_dispensed_b_test();
	 cp_get_application_ratio_test();
	 cp_get_mixer_tube_life_test();
	 cp_get_robot_completed_cycle_without_fault_test();
	 cp_get_dispense_unit_completed_cycle_without_fault_test();
	 cp_get_rotary_unit_completed_cycle_without_fault_test();
	 cp_get_adhesive_application_complete_summary_test();
	 cp_get_bead_check_laser_sensor_test();
	 cp_get_humidity_test();
	 cp_get_ambient_temperature_test();
	 csv_io_generate_csv_name_test();
}

static uint8_t * ut_get_buffer()
{
  uint8_t * buffer = malloc(sizeof(char)*DB_SIZE);
  memset(buffer, 0, DB_SIZE);

  return buffer;
}

static uint8_t * ut_set_time_date(uint8_t * buffer, uint32_t index, uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	s7lib_parser_write_int(buffer, index, year);
	buffer[index+2] = month;
	buffer[index+3] = day;
	buffer[index+5] = hour;
	buffer[index+6] = minute;
	buffer[index+7] = second;

	return buffer;
}

static uint8_t * ut_get_buffer_with_time_date(uint32_t index, uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	uint8_t * buffer = ut_get_buffer();
	return ut_set_time_date(buffer, index, year, month, day, hour, minute, second);
}



static void cp_get_vehicle_number_test()
{
	printf("cp_get_vehicle_number: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_string(buffer, 2, "asdfqwe123456", 13);

	char * vehicle_number = cp_get_vehicle_number(buffer);

	if(strcmp(vehicle_number, "asdfqwe123456") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(vehicle_number);
	free(buffer);
}

static void cp_get_rear_window_type_test()
{
	printf("cp_get_rear_window_type: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_string(buffer, 18, "asdfghjklqwer12345", 18);

	char * rear_window_type = cp_get_rear_window_type(buffer);

	if(strcmp(rear_window_type, "asdfghjklqwer12345") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(rear_window_type);
	free(buffer);
}

static void cp_get_vehicle_model_test()
{
	printf("cp_get_vehicle_model: ");

	uint8_t * buffer = ut_get_buffer();
	buffer[38] = '7';

	char * vehicle_model = cp_get_vehicle_model(buffer);

	if(strcmp(vehicle_model, "7") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(vehicle_model);
	free(buffer);
}

static void cp_get_window_id_test()
{
	printf("cp_get_window_id: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_dword(buffer, 40, 4567);

	char * window_id = cp_get_window_id(buffer);

	if(strcmp(window_id, "4567") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(window_id);
	free(buffer);
}

static void cp_get_time_from_primer_application_test()
{
	printf("cp_get_time_from_primer_application: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(44, 2020, 6, 27, 23, 21, 51);
	s7lib_parser_write_bool(buffer, 122, 1, true);
	
	char * time_from_primer_application = cp_get_time_from_primer_application(buffer);
	
	if(strcmp(time_from_primer_application, "2020-06-27 23:21:51") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(time_from_primer_application);
	free(buffer);
}

static void cp_get_time_from_primer_application_process_off_test()
{
	printf("cp_get_time_from_primer_application_process_off: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(44, 2020, 6, 27, 23, 21, 51);

	char * time_from_primer_application = cp_get_time_from_primer_application(buffer);
	
	if(strcmp(time_from_primer_application, "NA") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(time_from_primer_application);
	free(buffer);
}

static void cp_get_primer_detection_with_vision_ok_test()
{
	printf("cp_get_primer_detection_with_vision_ok: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 122, 0, true);
	s7lib_parser_write_bool(buffer, 108, 1, true);
	
	char * primer_detection_with_vision_ok = cp_get_primer_detection_with_vision_ok(buffer);

	if(strcmp(primer_detection_with_vision_ok, "PASS") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(primer_detection_with_vision_ok);
	free(buffer);
}

static void cp_get_primer_detection_with_vision_ok_control_off_test()
{
	printf("cp_get_primer_detection_with_vision_ok_control_off: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 108, 1, true);


	char * primer_detection_with_vision_ok = cp_get_primer_detection_with_vision_ok(buffer);

	if(strcmp(primer_detection_with_vision_ok, "NA") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(primer_detection_with_vision_ok);

	free(buffer);
}

static void cp_get_bead_check_pass_fail_area_test()
{
	printf("cp_get_beat_check_pass_fail_area: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 106, 0, true);
	s7lib_parser_write_bool(buffer, 106, 1, false);
	s7lib_parser_write_bool(buffer, 106, 2, true);
	s7lib_parser_write_bool(buffer, 106, 3, false);
	s7lib_parser_write_bool(buffer, 122, 0, true);

	char * bead_check_pass_fail_area = cp_get_bead_check_pass_fail_area(buffer);

	if(strcmp(bead_check_pass_fail_area, "PASS-FAIL-PASS-FAIL") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(bead_check_pass_fail_area);
	free(buffer);
}

static void cp_get_bead_check_pass_fail_area_process_off_test()
{
	printf("cp_get_beat_check_pass_fail_area_process_off: ");

	uint8_t * buffer = ut_get_buffer();
	buffer = s7lib_parser_write_bool(buffer, 106, 0, true);
	buffer = s7lib_parser_write_bool(buffer, 106, 1, false);
	buffer = s7lib_parser_write_bool(buffer, 106, 2, true);
	buffer = s7lib_parser_write_bool(buffer, 106, 3, false);
	
	char * bead_check_pass_fail_area = cp_get_bead_check_pass_fail_area(buffer);

	if(strcmp(bead_check_pass_fail_area, "NA") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(bead_check_pass_fail_area);
	free(buffer);
}

static void cp_get_primer_curring_rack_test()
{
	printf("cp_get_primer_curring_rack: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_int(buffer, 104, 5);

	char * primer_curring_rack = cp_get_primer_curring_rack(buffer);

	if(strcmp(primer_curring_rack, "5") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(primer_curring_rack);
	free(buffer);
}

static void cp_get_time_primer_flashoff_complete_test()
{
	printf("cp_get_time_primer_flash_off_complete: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(56, 2020, 6, 27, 12, 33, 53);
	s7lib_parser_write_bool(buffer, 122, 1, true);

	char * time_primer_flashoff_complete = cp_get_time_primer_flashoff_complete(buffer);

	if(strcmp(time_primer_flashoff_complete, "2020-06-27 12:33:53") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(time_primer_flashoff_complete);
	free(buffer);
}

static void cp_get_time_primer_flashoff_complete_process_off_test()
{
	printf("cp_get_time_primer_flash_off_complete_process_off: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(56, 2020, 6, 27, 12, 33, 53);

	char * time_primer_flashoff_complete = cp_get_time_primer_flashoff_complete(buffer);

	if(strcmp(time_primer_flashoff_complete, "NA") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(time_primer_flashoff_complete);
	free(buffer);
}


static void cp_get_interval_from_primering_util_gluing_test()
{
	printf("cp_get_interval_from_primering_util_gluing: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(44, 2020, 6, 27, 23, 21, 51);
	buffer = ut_set_time_date(buffer, 80, 2020, 6, 27, 23, 41, 51);
	s7lib_parser_write_bool(buffer, 122, 1, true);

	char * interval_from_primering_util_gluing = cp_get_interval_from_primering_util_gluing(buffer);

	if(strcmp(interval_from_primering_util_gluing, "1200") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(interval_from_primering_util_gluing);
	free(buffer);
}


static void cp_get_interval_from_primering_util_gluing_process_off_test()
{
	printf("cp_get_interval_from_primering_util_gluing_process_off: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(44, 2020, 6, 27, 23, 21, 51);
	buffer = ut_set_time_date(buffer, 80, 2020, 6, 27, 23, 41, 51);

	char * interval_from_primering_util_gluing = cp_get_interval_from_primering_util_gluing(buffer);

	if(strcmp(interval_from_primering_util_gluing, "NA") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(interval_from_primering_util_gluing);
	free(buffer);
}

static void cp_get_time_from_last_dispense_test()
{
	printf("cp_get_time_from_last_dispense: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(110, 2020, 11, 2,22,16,2);
	
	char * time_from_last_dispense = cp_get_time_from_last_dispense(buffer);

	if(strcmp(time_from_last_dispense, "2020-11-02 22:16:02") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(time_from_last_dispense);
	free(buffer);
}

static void cp_get_timestamp_of_glue_bead_application_test()
{
	printf("cp_get_timestamp_of_glue_bead_application: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(68, 2020,11,2,23,16,33);

	char * timestamp_of_flue_bead_application = cp_get_timestamp_of_glue_bead_application(buffer);

	if(strcmp(timestamp_of_flue_bead_application, "2020-11-02 23:16:33") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(timestamp_of_flue_bead_application);
	free(buffer);
}

static void cp_get_robot_finish_time_glue_bead_application_test()
{
	printf("cp_get_robot_finish_time_glue_bead_application: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(80, 2020, 5, 22, 12, 5, 23);
	
	char * robot_finish_time_glue_bead_application = cp_get_robot_finish_time_glue_bead_application(buffer);

	if(strcmp(robot_finish_time_glue_bead_application, "2020-05-22 12:05:23") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(robot_finish_time_glue_bead_application);
	free(buffer);
}

static void cp_get_interval_from_glue_application_util_output_test()
{
	printf("cp_get_interval_from_glue_application_util_output: ");

	uint8_t * buffer = ut_get_buffer_with_time_date(68, 2020, 12,2,12,34,44);
	buffer = ut_set_time_date(buffer, 92, 2020,12,2,12,40,44);

	char * interval_from_glue_application_util_output = cp_get_interval_from_glue_application_util_output(buffer);

	if(strcmp(interval_from_glue_application_util_output, "360") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(interval_from_glue_application_util_output);
	free(buffer);
}

static void cp_get_barrel_expire_ok_a_test()
{
	printf("cp_get_barrel_expiration_ok_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 207, 0, true);

	char * barrel_expire_ok_a = cp_get_barrel_expire_ok_a(buffer);

	if(strcmp(barrel_expire_ok_a, "nok") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(barrel_expire_ok_a);
	free(buffer);
}

static void cp_get_adhesive_batch_id_a_test()
{
	printf("cp_get_adhesive_batch_id_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_string(buffer, 168, "F113JC1UDA", 16);

	char * adhesive_batch_id_a = cp_get_adhesive_batch_id_a(buffer);

	if(strcmp(adhesive_batch_id_a, "F113JC1UDA") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(adhesive_batch_id_a);
	free(buffer);
}

static void cp_get_adhesive_serial_a_test()
{
	printf("cp_get_adhesive_serial_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_string(buffer, 186, "107472359", 16);

	char * adhesive_serial_a = cp_get_adhesive_serial_a(buffer); 

	if(strcmp(adhesive_serial_a, "107472359") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(adhesive_serial_a);
	free(buffer);
}

static void cp_get_material_dispensed_a_test()
{
	printf("cp_get_material_dispensed_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 152, 123.345);

	char * material_dispensed_a = cp_get_material_dispensed_a(buffer); 

	if(strcmp(material_dispensed_a, "123.3") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(material_dispensed_a);
	free(buffer);
}

static void cp_get_pistol_temperature_min_value_test()
{
	printf("cp_get_pistol_temperature_min_value: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_int(buffer, 142, 35);

	char * pistol_temperature_min_value = cp_get_pistol_temperature_min_value(buffer);

	if(strcmp(pistol_temperature_min_value, "35") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(pistol_temperature_min_value);
	free(buffer);
}

static void cp_get_pistol_temperature_test()
{
	printf("cp_get_pistol_temperature: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 138, 55.5);
	
	char * pistol_actual_temperature = cp_get_pistol_temperature(buffer);

	if(strcmp(pistol_actual_temperature, "55.5") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n"); 

	free(pistol_actual_temperature);
	free(buffer);
}

static void cp_get_pistol_temperature_max_value_test()
{
	printf("cp_get_pistol_temperature_max_value: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_int(buffer, 136, 65);
	
	char * pistol_temperature_max_value = cp_get_pistol_temperature_max_value(buffer);

	if(strcmp(pistol_temperature_max_value, "65") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(pistol_temperature_max_value);
	free(buffer);
}

static void cp_get_pot_temperature_min_value_a_test()
{
	printf("cp_get_pot_temperature_min_value_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_int(buffer, 150, 35);

	char * pot_temperature_min_value_a = cp_get_pot_temperature_min_value_a(buffer);

	if(strcmp(pot_temperature_min_value_a, "35") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(pot_temperature_min_value_a);
	free(buffer);
}

static void cp_get_pot_temperature_actual_a_test()
{
	printf("cp_get_pot_temperature_actual_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 146, 55.5);
	
	char * pot_temperature_actual_a = cp_get_pot_temperature_actual_a(buffer);

	if(strcmp(pot_temperature_actual_a, "55.5") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(pot_temperature_actual_a);
	free(buffer);
}

static void cp_get_pot_temperature_max_value_a_test()
{
	printf("cp_get_pot_temperature_max_value_a: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_int(buffer, 144, 65);

	char * pot_temperature_max_value_a = cp_get_pot_temperature_max_value_a(buffer);

	if(strcmp(pot_temperature_max_value_a, "65") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(pot_temperature_max_value_a);
	free(buffer);
}

static void cp_get_barrel_expire_ok_b_test()
{
	printf("cp_get_barrel_expire_ok_b: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 247, 0, true);

	char * barrel_expire_ok_b = cp_get_barrel_expire_ok_b(buffer);

	if(strcmp(barrel_expire_ok_b, "nok") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(barrel_expire_ok_b);
	free(buffer);
}

static void cp_get_adhesive_batch_b_test()
{
	printf("cp_get_adhesive_batch_b: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_string(buffer, 208, "YY00j6E802", 16);

	char * adhesive_batch_b = cp_get_adhesive_batch_b(buffer);

	if(strcmp(adhesive_batch_b, "YY00j6E802") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(adhesive_batch_b);
	free(buffer);
}

static void cp_get_adhesive_serial_b_test()
{
	printf("cp_get_adhesive_searial_b: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_string(buffer, 226, "107472359", 16);

	char * adhesive_serial_b = cp_get_adhesive_serial_b(buffer);

	if(strcmp(adhesive_serial_b, "107472359") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(adhesive_serial_b);
	free(buffer);
}

static void cp_get_material_dispensed_b_test()
{
	printf("cp_get_material_dispensed_b: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 156, 2.234);

	char * material_dispensed_b = cp_get_material_dispensed_b(buffer); 

	if(strcmp(material_dispensed_b, "2.2") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(material_dispensed_b);
	free(buffer);
}

static void cp_get_application_ratio_test()
{
	printf("cp_get_application_ratio: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 128, 100.0);
	s7lib_parser_write_real(buffer, 132, 3.0);
	
	char * application_ratio = cp_get_application_ratio(buffer);

	if(strcmp(application_ratio, "100.0:3.0") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(application_ratio);
	free(buffer);
}

static void cp_get_mixer_tube_life_test()
{
	printf("cp_get_mixer_tube_life: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_dword(buffer, 124, 3453);

	char * mixer_tube_life = cp_get_mixer_tube_life(buffer);

	if(strcmp(mixer_tube_life, "3453") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(mixer_tube_life);
	free(buffer);
}

static void cp_get_robot_completed_cycle_without_fault_test()
{
	printf("cp_get_robot_completed_cycle_without_fault: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 108, 4, true);
	
	char * robot_completed_cycle_without_fault = cp_get_robot_completed_cycle_without_fault(buffer);

	if(strcmp(robot_completed_cycle_without_fault, "true") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(robot_completed_cycle_without_fault);
	free(buffer);
}

static void cp_get_dispense_unit_completed_cycle_without_fault_test()
{
	printf("cp_get_dispense_unit_completed_cycle_without_fault: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 108, 5, true);

	char * dispense_unit_completed_cycle_without_fault = cp_get_dispense_unit_completed_cycle_without_fault(buffer);

	if(strcmp(dispense_unit_completed_cycle_without_fault, "true") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(dispense_unit_completed_cycle_without_fault);
	free(buffer);
}

static void cp_get_rotary_unit_completed_cycle_without_fault_test()
{
	printf("cp_get_rotary_unit_completed_cycle_wihtout_fault: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 108, 6, true);

	char * rotary_unit_completed_cycle_without_fault = cp_get_rotary_unit_completed_cycle_without_fault(buffer);

	if(strcmp(rotary_unit_completed_cycle_without_fault, "true") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(rotary_unit_completed_cycle_without_fault);
	free(buffer);
}

static void cp_get_adhesive_application_complete_summary_test()
{
	printf("cp_get_adhesive_application_complete_summary: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 108, 7, true);

	char * adhesive_application_complete_summary = cp_get_adhesive_application_complete_summary(buffer);

	if(strcmp(adhesive_application_complete_summary, "true") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(adhesive_application_complete_summary);
	free(buffer);
}

static void cp_get_bead_check_laser_sensor_test()
{
	printf("cp_get_bead_check_laser_sensor: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_bool(buffer, 108, 2, true);

	char * bead_check_laser_sensor = cp_get_bead_check_laser_sensor(buffer);

	if(strcmp(bead_check_laser_sensor, "PASS") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(bead_check_laser_sensor);
	free(buffer);
}

static void cp_get_humidity_test()
{
	printf("cp_get_humidity: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 160, 57.71);

	char * humidity = cp_get_humidity(buffer);

	if(strcmp(humidity, "57.7") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(humidity);
	free(buffer);
}

static void cp_get_ambient_temperature_test()
{
	printf("cp_get_ambient_temperature: ");

	uint8_t * buffer = ut_get_buffer();
	s7lib_parser_write_real(buffer, 164, 23.31);

	char * ambient_temperature = cp_get_ambient_temperature(buffer);

	if(strcmp(ambient_temperature, "23.3") == 0)
		printf("PASS\n");
	else
		printf("FAIL\n");

	free(ambient_temperature);
	free(buffer);
}



static void csv_io_generate_csv_name_test()
{
	printf("csv_io_generate_csv_name: ");

	char * name = csv_io_generate_csv_name(1591694883, ".", "InlineCubing");

	if(strcmp(name, "./InlineCubing-2020-06-09.csv") == 0)
		printf("PASS\n");	
	else
		printf("FAIL\n");

	free(name);
}

