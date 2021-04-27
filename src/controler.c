#include "include/controler.h"
#include "include/controler_parser.h"
#include "include/csv_io.h"
#include "include/csv_item.h"
#include "include/config.h"
#include <c_linked_list.h>

static bool controler_write_to_file(uint8_t *, char *, char);
static bool controler_store_csv_line(uint8_t *, char *, char*, char);
static c_linked_list * controler_compose_csv(uint8_t * buffer);
static char * controler_compose_csv_line(c_linked_list *, char *(*fce_ptr)(csv_item*), char);

bool controler_check_store_request(uint8_t * buffer, char * csv_path, char * csv_name, char separator)
{
    if(controler_store_csv_line(buffer, csv_path, csv_name, separator) == true)
          return true;

    return false;
}


/******************************* static functions **********************************/

static bool controler_write_to_file(uint8_t * buffer, char * csv_path, char separator)
{
  if(buffer != NULL)
  {
	c_linked_list * csv_struct = controler_compose_csv(buffer);

	char * csv_line = controler_compose_csv_line(csv_struct, csv_item_get_value, separator);

    if(access(csv_path, F_OK) != -1)
	{	
    	bool return_value = csv_io_write_to_file(csv_path, "\n%s", csv_line);

		free(csv_line);
		csv_struct = c_linked_list_clear_with_release(csv_struct, csv_item_finalize);

		return return_value;
	}
    else
	{
		char * csv_description_header = controler_compose_csv_line(csv_struct, csv_item_get_name, separator);
		char * csv_unit_header = controler_compose_csv_line(csv_struct, csv_item_get_unit, separator);

      	bool return_value = csv_io_write_to_file(csv_path, "%s\n%s\n%s", csv_description_header, csv_unit_header, csv_line);

		csv_struct = c_linked_list_clear_with_release(csv_struct, csv_item_finalize);
		free(csv_description_header);
		free(csv_unit_header);
		free(csv_line);

		return return_value;
	}
  }

  return false;
}

static c_linked_list * controler_add_csv_item(c_linked_list * csv_struct, char * name, char * unit, char * value)
{
	return c_linked_list_add_as_following(csv_struct, csv_item_new(name, unit, value));
}

static c_linked_list * controler_compose_csv(uint8_t * buffer)
{	
	c_linked_list * csv_struct = NULL;
	
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("JobNummer"), c_string_init(""), cp_get_job_number(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("AuftragsNr"), c_string_init(""), cp_get_vehicle_number(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("ScheibenType"), c_string_init(""), cp_get_rear_window_type(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("FahrzeugModell"), c_string_init(""), cp_get_vehicle_model(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("ScheibenNr"), c_string_init(""), cp_get_window_id(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TS_PrimerAuftrag"), c_string_init("Datum / Uhrzeit"), cp_get_time_from_primer_application(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("PrimerDetektiert"), c_string_init("true/false/NaN"), cp_get_primer_detection_with_vision_ok(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("Kleberaubenerkennung"), c_string_init("true/false/NaN"), cp_get_bead_check_pass_fail_area(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("Lagerfach"), c_string_init(""), cp_get_primer_curring_rack(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TS_PrimerAbgetrocknet"), c_string_init("Datum / Uhrzeit"), cp_get_time_primer_flashoff_complete(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TI_PrimerAufgebrachtBisKleberaupe"), c_string_init("s"), cp_get_interval_from_primering_util_gluing(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TS_LetzteSpuelungMischer"), c_string_init("Datum / Uhrzeit"), cp_get_time_from_last_dispense(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TS_KleberaupeStart"), c_string_init("Datum / Uhrzeit"), cp_get_timestamp_of_glue_bead_application(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TS_KleberaupeFertig"), c_string_init("Datum / Uhrzeit"), cp_get_robot_finish_time_glue_bead_application(buffer));	
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("TI_KleberaupeFertigBisScheibeEndnommen"), c_string_init("s"), cp_get_interval_from_glue_application_util_output(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_Unabgelaufen"), c_string_init("true/false/NaN"), cp_get_barrel_expire_ok_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_BatchId"), c_string_init(""), cp_get_adhesive_batch_id_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_SerienNr"), c_string_init(""), cp_get_adhesive_serial_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_Menge"), c_string_init("ml"), cp_get_material_dispensed_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("AppizierdueseTempMin"), c_string_init("°C"), cp_get_pistol_temperature_min_value(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("AppizierdueseTempAktuell"), c_string_init("°C"), cp_get_pistol_temperature(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("AppizierdueseTempMax"), c_string_init("°C"), cp_get_pistol_temperature_max_value(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_TempMin"), c_string_init("°C"), cp_get_pot_temperature_min_value_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_TempAktuell"), c_string_init("°C"), cp_get_pot_temperature_actual_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_TempMax"), c_string_init("°C"), cp_get_pot_temperature_max_value_a(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteB_Unabgelaufen"), c_string_init("true/false/NaN"), cp_get_barrel_expire_ok_b(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteB_BatchId"), c_string_init(""), cp_get_adhesive_batch_b(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteB_SerienNr"), c_string_init(""), cp_get_adhesive_serial_b(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KomponenteA_Menge"), c_string_init("ml"), cp_get_material_dispensed_b(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("MischungsverhaeltnisKomponenten"), c_string_init(""), cp_get_application_ratio(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("MischerrohrLebensdauerVerbleibend"), c_string_init("min"), cp_get_mixer_tube_life(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("RoboterZyklusOhneFehler"), c_string_init("true/false/NaN"), cp_get_robot_completed_cycle_without_fault(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("DosiereinheitOhneFehler"), c_string_init("true/false/NaN"), cp_get_dispense_unit_completed_cycle_without_fault(buffer));	
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("DrehtischOhneFehler"), c_string_init("true/false/NaN"), cp_get_rotary_unit_completed_cycle_without_fault(buffer));	
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KleberaupenauftragOhneFehler"), c_string_init("true/false/NaN"), cp_get_adhesive_application_complete_summary(buffer));
	csv_struct = controler_add_csv_item(csv_struct, c_string_init("KleberaupengeometrieOhneFehler"), c_string_init("true/false/NaN"), cp_get_bead_check_laser_sensor(buffer));	
		
	return c_linked_list_find_first(csv_struct);
}

static char * controler_compose_csv_line(c_linked_list* csv_struct, char *(*fce_ptr)(csv_item*), char separator)
{
	if(csv_struct != NULL)
		if(c_linked_list_is_last(csv_struct) == true)
			return c_string_concat(fce_ptr(c_linked_list_get_data(csv_struct)), controler_compose_csv_line(c_linked_list_next(csv_struct), fce_ptr, separator));
		else
			return c_string_concat(c_string_concat_character(fce_ptr(c_linked_list_get_data(csv_struct)), separator), controler_compose_csv_line(c_linked_list_next(csv_struct), fce_ptr, separator));
	else
		return NULL;	
}

static bool controler_store_csv_line(uint8_t * buffer, char * csv_path, char * csv_name, char separator)
{
	char * path = csv_io_generate_csv_name(time(NULL), csv_path, csv_name);
	bool return_val = controler_write_to_file(buffer, path, separator);

	free(path);

	return return_val;
}
