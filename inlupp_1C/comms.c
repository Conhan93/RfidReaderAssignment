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
    char message[BUFFERSIZE];

    strcat(message, commands[ADDCARD]);
    strcat(message, ".");
    strcat(message, card->ID);

    SerialWritePort(
        *port,
        message,
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