#include "admin.h"

bool remote_open_door(STATE* SYSTEM_STATE)
{
    // sends open door command to device


    char command[2];

    // convert command to string
    //_itoa(OPENDOOR, command, 10);
    command[COMMAND_INDEX] = OPENDOOR;
    
    // sends command string to device
    return SerialWritePort(
        SYSTEM_STATE->port,
        command,
        sizeof(command)
    );
}
void create_message(unsigned char* message_string, Card* card)
{
    // concatenates command and card ID
    char command[2]; 
    char delim[] = ".";

    unsigned char id_byte = 0;
    unsigned char converted_tag[5];
    

    char ID_tag[STRLEN];
    strcpy(ID_tag, card->ID);

    // places command at front of message string
    message_string[COMMAND_INDEX] = ADDCARD;

    // converts and places uid tag into converted_tag string
    char* digit = strtok(ID_tag, delim);

    int index = 0;
    while (digit)
    {
        id_byte = atoi(digit);
        converted_tag[index] = id_byte;
        digit = strtok(NULL, delim);
        index++;
    }

    // adds card uid onto message string
    strcat(message_string, converted_tag);
    printf("\nmessage to send: %s", message_string);
    for (int index = 0; index < strlen(message_string); index++)
        printf("\nvalue %d: %d",index, message_string[index]);
}
bool clear_cards(STATE* SYSTEM_STATE)
{
    // clears stored cards on device

    char command[2];
    
    // convert command to string 
    //_itoa(CLEARCARDS, command, 10);
    command[COMMAND_INDEX] = CLEARCARDS;

    // send string
    return SerialWritePort(
        SYSTEM_STATE->port,
        command,
        sizeof(command)
    );
}