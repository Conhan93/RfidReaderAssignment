#include "admin.h"

void remote_open_door(STATE* SYSTEM_STATE)
{
    // opens door on device
    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[OPENDOOR],
        BUFFERSIZE
    );
}
void create_message(char* message_string, Card* card, char* command)
{
    // concatenates command and card ID
    strcat(message_string, command);
    strcat(message_string, card->ID);
}
bool clear_cards(STATE* SYSTEM_STATE)
{
    // clears stored cards on device
    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[CLEARCARDS],
        BUFFERSIZE
    );
}