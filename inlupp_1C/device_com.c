#include "admin.h"

bool remote_open_door(STATE* SYSTEM_STATE)
{
    char command[2];

    itoa(OPENDOOR, command, 10);
    
    // opens door on device
    return SerialWritePort(
        SYSTEM_STATE->port,
        command,
        sizeof(command)
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

    char command[2];

    itoa(CLEARCARDS, command, 10);

    return SerialWritePort(
        SYSTEM_STATE->port,
        command,
        sizeof(command)
    );
}