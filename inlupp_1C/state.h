#ifndef STATE_INFO_INCLUDED
#define STATE_INFO_INCLUDED

#include <time.h>
#include <string.h>
#include "serial.h"


#define STRLEN 30
#define BUFFERSIZE 40

typedef struct
{
    char ID[STRLEN];
    bool access;
    time_t date_added;
} Card;


typedef struct
{
    Card* card_list;
    int nr_cards;
    SERIALPORT port;
} STATE;

#endif