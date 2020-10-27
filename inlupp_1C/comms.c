#include "comms.h"

/*
    File to hold functions communicating with device
*/


void remote_open_door(STATE* SYSTEM_STATE)
{

    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[OPENDOOR],
        BUFFERSIZE
    );
}
bool send_card(Card* card, SERIALPORT* port, char* commands[])
{
    SerialWritePort(
        *port,
        commands[ADDCARD],
        BUFFERSIZE
    );
}
bool clear_cards(STATE* SYSTEM_STATE)
{
    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[CLEARCARDS],
        BUFFERSIZE
    );
}