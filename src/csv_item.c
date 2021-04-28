#include "include/csv_item.h"
#include <stdio.h>


csv_item * csv_item_new(char * name, char * unit, char * value)
{
    csv_item * self = malloc(sizeof(csv_item));


    self->name = name;
    self->unit = unit;
    self->value = value;

    return self;
}

char * csv_item_get_name(csv_item * self)
{
    if(self != NULL)
        return self->name;
    
    return "";
}

char * csv_item_get_unit(csv_item * self)
{
    if(self != NULL)
        return self->unit;

    return "";
}

char * csv_item_get_value(csv_item * self)
{
    if(self != NULL)
        return self->value;

    return "";
}

void csv_item_finalize_v2(csv_item * self)
{
    if(self != NULL)
    {
        if(self->value != NULL)
            free(self->value);

        if(self->unit != NULL)    
            free(self->unit);

        if(self->name != NULL)    
            free(self->name);

        free(self);
    }
}

void csv_item_finalize(void * self)
{
    csv_item_finalize_v2(self);
}