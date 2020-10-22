#include <stdio.h>
#include "serial.h"


#define STRLEN 30

void comms(char* message);

int main(void)
{
    char message[STRLEN];


    printf("Enter message: ");
    scanf("%s",message);


    comms(message);

}
void comms(char* message)
{
    SERIALPORT port = SerialInit("COM3");

    if(!SerialIsConnected(port))
    {
        printf("\nport not connected");
        return;
    }
    char buf[512];
	SerialReadPort(port, buf,512);
    printf("\n%s",buf);
}
