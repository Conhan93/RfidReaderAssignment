#include <stdio.h>
#include <time.h>
#include "serial.h"
#include "safeinput.h"

#define STRLEN 30
#define BUFFERSIZE 40

typedef struct
{
    char ID[STRLEN];
    bool access;
    time_t date_added;
} Card;


typedef struct
{
    Card* card_list;
    int nr_cards;
    SERIALPORT port;
    char cmds[3][STRLEN];
} STATE;
typedef enum
{
    ADDCARD,
    OPENDOOR,
    CLEARCARDS
} COMMANDS;


void comms(char* message);
void initialize_state(STATE* SYSTEM_STATE);
void format_date_string(time_t date_added, char* buffer, int buffersize);
void list_cards(STATE* SYSTEM_STATE);
void remote_open_door(STATE* SYSTEM_STATE);
Card* search_id(Card* card_list, int nr_cards, char* search_term);
void change_card_access(STATE* SYSTEM_STATE);

int main()
{

    STATE SYSTEM_STATE;

    initialize_state(&SYSTEM_STATE);



    int selection = 0;

    while (true)
    {
        selection = admin_menu();
        switch (selection)
        {
        case 1:
            remote_open_door(&SYSTEM_STATE);
            break;
        case 2:
            list_cards(&SYSTEM_STATE);
            break;
        case 3:
            break;
        case 4:
            change_card_access(&SYSTEM_STATE);
            break;

        default:
            printf("\nError!");
            break;
        }
    }
}
void comms(char* message)
{
    SERIALPORT port = SerialInit("COM3");
    char card[] = "222.180.106.195.195";

    if (!SerialIsConnected(port))
    {
        printf("\nNo connection");
        return;
    }
    if (!SerialWritePort(port, message, STRLEN))
    {
        printf("\nUnable to write to file");
        return;
    }
    SerialWritePort(port, card, 20);
    char buff[512];
    printf("\nstarting read port...");
    SerialReadToNewLine(port, buff, 512);

    printf("\nRecieved: %s", buff);
}
int admin_menu()
{
    int selection = 0;

    printf("\nAdmin menu");
    printf("\n1.Remote Open Door"
        "\n2. List all cards in system"
        "\n3. Add/remove access"
        "\n4. Exit\n");

    GetInputInt(NULL, selection);

    return selection;
}
void remote_open_door(STATE* SYSTEM_STATE)
{

    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[CLEARCARDS],
        BUFFERSIZE
    )
}
void initialize_state(STATE* SYSTEM_STATE)
{
    // System state initialization

    SYSTEM_STATE->card_list = NULL;
    SYSTEM_STATE->nr_cards = 0;
    // initialize port
    SYSTEM_STATE->port = SerialInit("COM3");

    // initialize commands for device
    stcpy(SYSTEM_STATE->cmds[0], "ADDCARD");
    strcpy(SYSTEM_STATE->cmds[1], "OPENDOOR");
    stcpy(SYSTEM_STATE->cmds[2], "CLEARALLCARDS");
}
void list_cards(STATE* SYSTEM_STATE)
{
    char date_string[STRLEN];
    for (int index = 0; index < SYSTEM_STATE->nr_cards; index++)
    {
        format_date_string(
            SYSTEM_STATE->card_list[index].date_added,
            date_string,
            sizeof(date_string)
        );

        printf("\n%s\t%s\t%s",
            SYSTEM_STATE->card_list[index].ID,
            SYSTEM_STATE->card_list[index].access ? "Access" : "No Access",
            date_string
        );
    }
}
void change_card_access(STATE* SYSTEM_STATE)
{
    Card* active_card = NULL;
    char search_term[STRLEN];

    while (active_card == NULL)
    {
        if (GetInput("Enter card ID: ", search_term, sizeof(search_term) == INPUT_RESULT_OK))
        {
            if (!strcmp(search_term, "exit")) return;

            active_card = search_id(
                SYSTEM_STATE->card_list,
                SYSTEM_STATE->nr_cards,
                search_term
            );
        }
    }
    int new_access = 0;
    printf("\nThis card has %s, enter 1 for access, 2 for no access",
        active_card->access ? "access" : "no access");
    if (GetInputInt(NULL, &new_access))
    {
        if (new_access == 0 || new_access == 1)
            active_card->access = new_access;
        else printf("\nInvalid input, exiting");
    }

}
Card* search_id(Card* card_list, int nr_cards, char* search_term)
{
    Card* active_card;

    for (int index = 0; index < nr_cards; index++)
    {
        if (!strcmp(search_term, card_list[index].ID))
            return active_card = &card_list[index];
    }
    return NULL;
}
void format_date_string(time_t date_added, char* buffer, int buffersize)
{
    struct tm* date_ptr;

    date_ptr = localtime(&date_added);

    strftime(buffer, buffersize, "%Y-%m-%d", date_ptr);
}
void add_card(STATE* SYSTEM_STATE)
{

}
Card create_card(char* new_id)
{
    // creates and initializes a new card
    Card new_card;

    strcpy(new_card.ID, new_id);
    new_card.access = false;
    new_card.date_added = time(NULL);

    return new_card;
}