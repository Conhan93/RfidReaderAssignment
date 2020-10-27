#include "serial.h"
#include "state.h"

void remote_open_door(STATE* SYSTEM_STATE);
bool send_card(Card* card, SERIALPORT* port, char* commands[]);
bool clear_cards(STATE* SYSTEM_STATE);
