#include "admin.h"

bool remote_open_door(STATE* SYSTEM_STATE)
{
    // sends open door command to device


    char message[2];

    // add command to message
    message[COMMAND_INDEX] = OPENDOOR;
    // close message
    message[COMMAND_INDEX + 1] = '\0';
    
    // sends command string to device
    return SerialWritePort(
        SYSTEM_STATE->port,
        message,
        sizeof(message)
    );
}
void create_message(unsigned char message_string[], Card* card)
{
    // formats message with add card command and card ID
    

    unsigned char id_byte = 0;
    
    // copy of card ID so original is untouched by strtok
    char ID_copy[STRLEN];
    strcpy(ID_copy, card->ID);

    // places command at front of message string
    message_string[COMMAND_INDEX] = ADDCARD;

    // converts and places uid tag onto message
    char delim[] = ".";
    char* digit = strtok(ID_copy, delim);

    int index = 1;
    while (digit)
    {
        id_byte = atoi(digit);
        message_string[index] = (unsigned char)id_byte;
        digit = strtok(NULL, delim);
        index++;
    }
}
bool clear_cards(STATE* SYSTEM_STATE)
{
    // clears stored cards on device

    char message[2];
    
    // add command to message
    message[COMMAND_INDEX] = CLEARCARDS;
    // close message
    message[COMMAND_INDEX + 1] = '\0';
    // send string
    return SerialWritePort(
        SYSTEM_STATE->port,
        message,
        sizeof(message)
    );
}