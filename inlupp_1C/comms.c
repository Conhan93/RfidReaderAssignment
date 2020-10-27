#include "comms.h"

/*
    File to hold functions communicating with device
*/


void remote_open_door(STATE* SYSTEM_STATE)
{

    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[CLEARCARDS],
        BUFFERSIZE
    );
}