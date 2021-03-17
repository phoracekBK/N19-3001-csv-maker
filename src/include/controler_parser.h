#ifndef _controler_parser_get_H_
#define _controler_parser_get_H_

#include <stdint.h>
#include <math.h>
#include <c_string.h>
#include <s7lib_parser.h>
#include "config.h"

char * cp_get_job_number(uint8_t *);
char * cp_get_vehicle_number(uint8_t *);
char * cp_get_rear_window_type(uint8_t *);
char * cp_get_vehicle_model(uint8_t *);
char * cp_get_window_id(uint8_t *);
char * cp_get_time_from_primer_application(uint8_t *);
char * cp_get_primer_detection_with_vision_ok(uint8_t *);
char * cp_get_bead_check_pass_fail_area(uint8_t *);
char * cp_get_primer_curring_rack(uint8_t *);
char * cp_get_time_primer_flashoff_complete(uint8_t *);
char * cp_get_interval_from_primering_util_gluing(uint8_t *);		
char * cp_get_time_from_last_dispense(uint8_t *);
char * cp_get_timestamp_of_glue_bead_application(uint8_t *);
char * cp_get_robot_finish_time_glue_bead_application(uint8_t *);
char * cp_get_interval_from_glue_application_util_output(uint8_t *);
char * cp_get_barrel_expire_ok_a(uint8_t *);
char * cp_get_adhesive_batch_id_a(uint8_t *);
char * cp_get_adhesive_serial_a(uint8_t *);
char * cp_get_material_dispensed_a(uint8_t *);
char * cp_get_pistol_temperature_min_value(uint8_t *);
char * cp_get_pistol_temperature(uint8_t *);
char * cp_get_pistol_temperature_max_value(uint8_t *);
char * cp_get_pot_temperature_min_value_a(uint8_t *);
char * cp_get_pot_temperature_actual_a(uint8_t *);
char * cp_get_pot_temperature_max_value_a(uint8_t *);
char * cp_get_barrel_expire_ok_b(uint8_t *);
char * cp_get_adhesive_batch_b(uint8_t *);
char * cp_get_adhesive_serial_b(uint8_t *);
char * cp_get_material_dispensed_b(uint8_t *);
char * cp_get_application_ratio(uint8_t *);
char * cp_get_mixer_tube_life(uint8_t *);
char * cp_get_robot_completed_cycle_without_fault(uint8_t *);
char * cp_get_dispense_unit_completed_cycle_without_fault(uint8_t *);
char * cp_get_rotary_unit_completed_cycle_without_fault(uint8_t *);
char * cp_get_adhesive_application_complete_summary(uint8_t *);
char * cp_get_bead_check_laser_sensor(uint8_t *);
char * cp_get_humidity(uint8_t *);
char * cp_get_ambient_temperature(uint8_t *);
uint64_t cp_time_to_seconds(int *);

#endif
