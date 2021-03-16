#ifndef _CSV_ITEM_H_
#define _CSV_ITEM_H_

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

struct _csv_item_;
typedef struct _csv_item_ csv_item;

struct _csv_item_
{
    char * name;
    char * unit;
    char * value;
};

csv_item * csv_item_new(char * name, char * unit, char * value);
char * csv_item_get_name(csv_item * self);
char * csv_item_get_unit(csv_item * self);
char * csv_item_get_value(csv_item * self);
void csv_item_finalize(void * self);



#endif