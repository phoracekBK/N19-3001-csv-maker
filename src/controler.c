#include "include/controler.h"
#include "include/controler_parser.h"
#include "include/csv_io.h"

static bool controler_write_to_file(uint8_t *, char *, char);
static bool controler_store_csv_line(uint8_t *, char *, char*, char);
static char * controler_compose_csv_unit_header(char);
static char * controler_compose_csv_description_header(char);
static char * controler_compose_csv_line(uint8_t *, char);

uint8_t * controler_check_store_request(uint8_t * buffer, char * csv_path, char * csv_name, char separator)
{
    if(s7lib_parser_read_bool(buffer, 0, 0) == true)
    {
        if(controler_store_csv_line(buffer, csv_path, csv_name, separator) == true)
          return s7lib_parser_write_bool(buffer, 0, 0, false);
    }

    return buffer;
}


/******************************* static functions **********************************/

static bool controler_write_to_file(uint8_t * buffer, char * csv_path, char separator)
{
  if(buffer != NULL)
  {
	char * csv_line = controler_compose_csv_line(buffer, separator);

    if(access(csv_path, F_OK) != -1)
	{
    	bool return_value = csv_io_write_to_file(csv_path, "\n%s", csv_line);
		//printf("%s\n", csv_line);

		free(csv_line);

		return return_value;
	}
    else
	{
		char * csv_description_header = controler_compose_csv_description_header(separator);
		char * csv_unit_header = controler_compose_csv_unit_header(separator);

		//printf("%s\n%s\n%s\n", csv_description_header, csv_unit_header, csv_line);

      	bool return_value = csv_io_write_to_file(csv_path, "%s\n%s\n%s", csv_description_header, csv_unit_header, csv_line);

		free(csv_description_header);
		free(csv_unit_header);
		free(csv_line);

		return return_value;
	}
  }

  return false;
}

static char * controler_compose_csv_description_header(char separator)
{
	char * line = NULL;

	line = c_string_add_constant(line, "Job number");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Vehicle number");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Rear window type");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Vehicle model");
	line = c_string_add_character(line, separator);	
	line = c_string_add_constant(line, "Window id");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Time from primer application");
    line = c_string_add_character(line, separator);
	line = c_string_add_constant(line,	"Primer detection with vision");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Bead check pass/fail area");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Primer curring rack");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Time stamp when primer flashoff complete");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Time interval from primer application util glue application finish");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Time from last dispense");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Timestamp of glue bead application");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Robot_finish time glue bead application");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Time interval from glue bead application util glass taken out from output station");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Barrel expiration ok A");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Adhesive batch id A");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Adhesive serial A");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Material dispensed A");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Pistol temperature min");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Pistol temperature actual");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Pistol temperature max");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Pot A temperature min");
    line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Pot A temperature actual");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Pot A temperature max");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Barrel expire ok B");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Adhesive batch id B");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Adhesive serial B");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Material dispensed B");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Application ratio");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Mix tube life");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Robot completed cycle without fault");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Dispense unit completed cycle without fault");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Rotary unit completed cycle without fault");
	line = c_string_add_character(line, separator);
    line = c_string_add_constant(line, "Adhesive application process complete summery");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "Bead check laser sensor");

	return line;	
}

static char * controler_compose_csv_unit_header(char separator)
{
	char * line = NULL;

	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);	
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
    line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "s");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "s");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "ml");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "°C");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "°C");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "°C");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "°C");
    line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "°C");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "°C");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "ml");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "min");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
    line = c_string_add_constant(line, "");
	line = c_string_add_character(line, separator);
	line = c_string_add_constant(line, "");

	return line;

}

static char * controler_compose_csv_line(uint8_t * buffer, char separator)
{
	char * line = NULL;

	line = c_string_add(line, cp_get_job_number(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_vehicle_number(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_rear_window_type(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_vehicle_model(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_window_id(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_time_from_primer_application(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_primer_detection_with_vision_ok(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_bead_check_pass_fail_area(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_primer_curring_rack(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_time_primer_flashoff_complete(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_interval_from_primering_util_gluing(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_time_from_last_dispense(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_timestamp_of_glue_bead_application(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_robot_finish_time_glue_bead_application(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_interval_from_glue_application_util_output(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_barrel_expire_ok_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_adhesive_batch_id_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_adhesive_serial_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_material_dispensed_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line,cp_get_pistol_temperature_min_value(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_pistol_temperature(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_pistol_temperature_max_value(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_pot_temperature_min_value_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_pot_temperature_actual_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_pot_temperature_max_value_a(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_barrel_expire_ok_b(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_adhesive_serial_b(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_adhesive_serial_b(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_material_dispensed_b(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_application_ratio(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_mixer_tube_life(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_robot_completed_cycle_without_fault(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_dispense_unit_completed_cycle_without_fault(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_rotary_unit_completed_cycle_without_fault(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_adhesive_application_complete_summary(buffer));
	line = c_string_add_character(line, separator);
	line = c_string_add(line, cp_get_bead_check_laser_sensor(buffer));

  	return line;
}

static bool controler_store_csv_line(uint8_t * buffer, char * csv_path, char * csv_name, char separator)
{
	char * path = csv_io_generate_csv_name(time(NULL), csv_path, csv_name);
	bool return_val = controler_write_to_file(buffer, path, separator);

	free(path);

	return return_val;
}
