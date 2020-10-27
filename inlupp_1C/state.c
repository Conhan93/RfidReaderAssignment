#include "state.h"


void initialize_state(STATE* SYSTEM_STATE)
{
    // System state initialization

    SYSTEM_STATE->card_list = NULL;
    SYSTEM_STATE->nr_cards = 0;
    // initialize port
    SYSTEM_STATE->port = SerialInit("COM3");

    // initialize commands for device communication
    strcpy(SYSTEM_STATE->cmds[0], "ADDCARD");
    strcpy(SYSTEM_STATE->cmds[1], "OPENDOOR");
    strcpy(SYSTEM_STATE->cmds[2], "CLEARALLCARDS");
}