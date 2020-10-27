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
    char cmds[3][STRLEN];
} STATE;
typedef enum
{
    ADDCARD,
    OPENDOOR,
    CLEARCARDS
} COMMANDS;

void initialize_state(STATE* SYSTEM_STATE);