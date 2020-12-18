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


#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif



#include <s7lib.h>
#include "include/controler.h"
#include "include/config.h"


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
		while(true)
  		{
    	   	uint8_t * buffer =  s7lib_read(s7lib_ref, 0, DB_SIZE);

    	   	if(buffer != NULL)
    	   	{
    	   		buffer = controler_check_store_request(buffer, path, CSV_NAME, CSV_SEPARATOR);
    	   		s7lib_write(s7lib_ref, buffer, 0, 69);
    	   		free(buffer);
    	   	 	printf("%s\n", (char*) buffer);
    	   	}
    	   	else
    	   	{
    	   		printf("Can't read from PLC\n");
    	   	}

			fflush(stdout);
    	   	sleep(1);
    	}
	}
	else
	{
		printf("CSV path is not valid!\n");
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
  	}

  return 0;
}



