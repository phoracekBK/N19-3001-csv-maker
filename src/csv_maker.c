/*
 ============================================================================
 Name        : csv_maker.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif



#include <s7lib.h>
#include "include/controler.h"
#include "include/config.h"



static char * time_string(const char * format)
{
	char * time_string = (char*) malloc(sizeof(char)*23);

    time_t my_time;
    struct tm* time_info;

    time(&my_time);
    time_info = localtime(&my_time);
    strftime(time_string, 23, format, time_info);

	return time_string;
}

static char * generate_log_name(char * csv_path, char * csv_name)
{
	time_t current_time;
	time(&current_time);
  	struct tm tm = * localtime(&current_time);
	return c_string_format("%s/%s-%d-%02d-%02d.log", csv_path, csv_name, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

static void save_log(char * format, ...)
{
	char * log_file_name = generate_log_name("log", "debug-log");

	FILE * log_file = fopen(log_file_name, "a");

	if(log_file != NULL)
	{
		va_list params;
		va_start(params, format);

		char * time_stamp = time_string("%d.%m.%y - %H:%M:%S");
		printf("%s - ", time_stamp);
		vprintf(format, params);
		printf("\n");

		fflush(stdout);

		if(log_file != NULL)
		{
			fprintf(log_file, "%s - ", time_stamp);
			vfprintf(log_file, format, params);
			fprintf(log_file, "\n");
			fflush(log_file);
			fclose(log_file);
		}

		free(log_file_name);
		free(time_stamp);

		va_end(params);		
	}
	else
	{
		printf("Can't open log file!\n");
		fflush(stdout);
	}
	
}

bool is_path_valid(char * path)
{
	if(access(path, F_OK) != 0)
	{
		if(errno == ENOENT || errno == ENOTDIR)
			return false;
	}

	return true;
}

char * resolve_path_for_csv(int argv, char ** argc, char * address_flag, char * default_path)
{
	if(argv > 1)
	{
		if(strcmp(argc[argv-1], address_flag) == 0)
			return argc[argv];
		else
			return resolve_path_for_csv(argv-1, argc, address_flag, default_path);
	}
	else
	{
		return default_path;
	}
}

bool is_debug_mode(int argv, char ** argc, char * debug_flag)
{
	if(argv > 1)
	{
		if(strcmp(argc[argv-1], debug_flag) == 0)
			return true;
		else
			return is_debug_mode(argv -1, argc, debug_flag);
	}
	else
	{
		return false;
	}
}

void main_loop(s7lib * s7lib_ref, char * path)
{
	if(is_path_valid(path) == true)
	{
		bool store_request_trigger = false;
		save_log("Entering to main loop");

		s7lib_write_bool(s7lib_ref, 0,1, false);
		s7lib_write_bool(s7lib_ref, 0,2, false);

		while(true)
  		{
			bool store_request = s7lib_read_bool(s7lib_ref, 0, 0);

    	   	if( store_request == true && store_request_trigger == false)
    	   	{
    	   		save_log("Saving csv record into %s", CSV_NAME);

    	   		uint8_t * buffer =  s7lib_read(s7lib_ref, 0, DB_SIZE);

    	   		if(buffer != NULL)
    	   		{
					if (controler_check_store_request(buffer, path, CSV_NAME, CSV_SEPARATOR) == true)
    	   				s7lib_write_bool(s7lib_ref, 0,1, true);
					else
						s7lib_write_bool(s7lib_ref, 0,2, true);

     	   			free(buffer);
    	   		}
    	   		else
    	   		{
    	   	    	save_log("Can't read from PLC\n");
    	   	    }
    	   	}
			else if(store_request == false && store_request_trigger == true)
			{
				s7lib_write_bool(s7lib_ref, 0,1, false);
				s7lib_write_bool(s7lib_ref, 0,2, false);
			}
			

			fflush(stdout);
    	   	sleep(0.5);

			store_request_trigger = store_request;
    	}
	}
	else
	{
		save_log("CSV path is not valid!\n");
	}	
}

void main_loop_silence_mode(s7lib * s7lib_ref, char * path)
{
	HWND var = GetConsoleWindow();

	ShowWindow(var, SW_HIDE);

	main_loop(s7lib_ref, path);

	ShowWindow(var, SW_SHOW);
}


int main(int argv, char ** argc)
{
  	s7lib * s7lib_ref = s7lib_new(IP_ADDRESS, RACK, SLOT, DB_INDEX);

	if(s7lib_ref != NULL)
  	{
		if(is_debug_mode(argv, argc, "-s") == true)
			main_loop_silence_mode(s7lib_ref, resolve_path_for_csv(argv, argc, "-a", CSV_PATH));
		else
			main_loop(s7lib_ref, resolve_path_for_csv(argv, argc, "-a", CSV_PATH));
 	}
  	else
  	{
    	  printf("S7lib initialization error!\n");
		  fflush(stdout);
  	}

  return 0;
}



