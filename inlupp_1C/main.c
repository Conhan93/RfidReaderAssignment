#include <stdio.h>
#include <time.h>
#include "serial.h"
#include "safeinput.h"
#include "state.h"

#define BUFFERSIZE 40






void format_date_string(time_t date_added, char* buffer, int buffersize);
void list_cards(STATE* SYSTEM_STATE);
void save_to_file(STATE* SYSTEM_STATE);
Card* search_id(STATE* SYSTEM_STATE);
Card* get_card(Card* card_list, int nr_cards, char* search_term);
void change_card_access(STATE* SYSTEM_STATE);
int admin_menu();
void add_card(STATE* SYSTEM_STATE);
bool valid_id(char* new_id);
void expand_list(STATE* SYSTEM_STATE);
Card create_card(char* new_id);

void remote_open_door(STATE* SYSTEM_STATE);
void send_card(STATE* SYSTEM_STATE);
void create_message(char* message_string, Card* card, char* commands[]);
bool clear_cards(STATE* SYSTEM_STATE);

bool load_file(STATE* SYSTEM_STATE);

int main()
{

    STATE SYSTEM_STATE;

    initialize_state(&SYSTEM_STATE);

    printf("\n%s", load_file(&SYSTEM_STATE) ? "files loaded" : "unable to load files");



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
            add_card(&SYSTEM_STATE);
            break;
        case 4:
            change_card_access(&SYSTEM_STATE);
            break;
        case 5:
            save_to_file(&SYSTEM_STATE);
            break;
        case 6:
            send_card(&SYSTEM_STATE);
            break;
        case 7:
            clear_cards(&SYSTEM_STATE);
            break;
        case 9:
            SerialClose(SYSTEM_STATE.port);
            return 0;

        default:
            printf("\nError!");
            break;
        }
    }
}
int admin_menu()
{
    /*  Prints main menu and gets menu input    */

    int selection = 0;

    printf("\n\n\n****ADMIN MENU****\n");
    printf("\n1.Remote Open Door"
        "\n2. List all cards in system"
        "\n3. Add card to list"
        "\n4. Add/remove access"
        "\n5. Save cards to file"
        "\n6. Send a card to device"
        "\n7. Clear all cards on device"
        "\n9. Exit\n");

    GetInputInt(NULL, &selection);

    return selection;
}
void list_cards(STATE* SYSTEM_STATE)
{
    // displays all cards in system.
    char date_string[STRLEN];

    printf("\nID\t\t\tACCESS\t\t\tDATE ADDED\n"
    "------------------------------------------------------------");

    for (int index = 0; index < SYSTEM_STATE->nr_cards; index++)
    {
        format_date_string(
            SYSTEM_STATE->card_list[index].date_added,
            date_string,
            sizeof(date_string)
        );
        
        printf("\n%-20s\t%-20s\t%s",
            SYSTEM_STATE->card_list[index].ID,
            SYSTEM_STATE->card_list[index].access ? "Access" : "No Access",
            date_string
        );
    }
}
void change_card_access(STATE* SYSTEM_STATE)
{
    Card* active_card = NULL;

    // fetch card, exits if null
    if ((active_card = search_id(SYSTEM_STATE)) == NULL) return;

    int new_access = 0;

    printf("\nThis card has %s, enter 1 for access, 2 for no access",
        active_card->access ? "access" : "no access");
    // sets new access for card
    if (GetInputInt(NULL, &new_access))
    {
        if (new_access == 0 || new_access == 1)
            active_card->access = new_access;
        else printf("\nInvalid input, exiting");
    }

}
Card* search_id(STATE* SYSTEM_STATE)
{
    /*  Interface for card ID search    */


    Card* active_card = NULL;
    char search_term[STRLEN];

    while (active_card == NULL)
    {
        if (GetInput("Enter card ID: ", search_term, sizeof(search_term)) == INPUT_RESULT_OK)
        {
            if (!strcmp(search_term, "exit")) return NULL;

            active_card = get_card(
                SYSTEM_STATE->card_list,
                SYSTEM_STATE->nr_cards,
                search_term
            );
        }
    }
    return active_card;
}
Card* get_card(Card* card_list, int nr_cards, char* search_term)
{
    /*   Loops through card list comparing ID:s and returns card with matching ID   */

    Card* active_card;

    for (int index = 0; index < nr_cards; index++)
    {
        if (!strcmp(search_term, card_list[index].ID))
            return active_card = &card_list[index];
    }
    // returns NULL if card is not found
    return NULL;
}
void format_date_string(time_t date_added, char* buffer, int buffersize)
{
    // Formats buffer string with the time stored in date_added
    struct tm* date_ptr;

    date_ptr = localtime(&date_added);

    strftime(buffer, buffersize, "%Y-%m-%d", date_ptr);
}
void add_card(STATE* SYSTEM_STATE)
{
    char new_card_id[STRLEN];
    do
    {
        printf("\nValid card in form x.x.x.x.x where x is an integer between 0 and 257\n");

        GetInput("Ange kortnr: ", new_card_id, sizeof(new_card_id));
        if (valid_id(new_card_id))
        {
            expand_list(SYSTEM_STATE);
            SYSTEM_STATE->card_list[SYSTEM_STATE->nr_cards] = create_card(new_card_id);
            SYSTEM_STATE->nr_cards++;
            return;
        }
        else printf("\nInvalid ID");

    } while (true);
}
bool valid_id(char* new_id)
{
    // checks if new card id input is valid or not
    char temp[STRLEN], delim[] = ".";
    char* digit;
    int counter = 0;

    strcpy(temp, new_id);

    digit = strtok(temp, delim);
    while (digit != NULL && counter == 5)
    {
        if (atoi(digit) > 257 || atoi(digit) < 0) return false;

        else digit = strtok(NULL, delim);

        counter++;
    }
    return true;
}
void expand_list(STATE* SYSTEM_STATE)
{
    
    if (SYSTEM_STATE->nr_cards <= 0)
    
    SYSTEM_STATE->card_list = malloc(sizeof(Card));

    else    
        SYSTEM_STATE->card_list = realloc
        (
            SYSTEM_STATE->card_list,
            sizeof(Card) * (SYSTEM_STATE->nr_cards + 1)
        );
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
void remote_open_door(STATE* SYSTEM_STATE)
{

    SerialWritePort(
        SYSTEM_STATE->port,
        SYSTEM_STATE->cmds[OPENDOOR],
        BUFFERSIZE
    );
}
void send_card(STATE* SYSTEM_STATE)
{
    /*    Sequence for adding a card to device.     */

    Card* active_card = NULL;
    char message[BUFFERSIZE] = "";

    // fetch card
    if ((active_card = search_id(SYSTEM_STATE)) == NULL) return;
    
    // compose message to device
    create_message(message,
        active_card,
        SYSTEM_STATE->cmds[ADDCARD]
    );

    printf("\nSent this: %s", message);
    return;
    // send message to device
    SerialWritePort(
        SYSTEM_STATE->port,
        message,
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
void save_to_file(STATE* SYSTEM_STATE)
{
    char filename[] = "cards.bin";

    FILE* file_ptr;

    file_ptr = fopen(filename, "wb");
    if (file_ptr)
    {
        fwrite(
            SYSTEM_STATE->card_list,
            sizeof(Card),
            SYSTEM_STATE->nr_cards,
            file_ptr
        );
    }

    fclose(file_ptr);
}
int get_file_size(FILE* file_ptr)
{
    int start = ftell(file_ptr);

    fseek(file_ptr, 0L, SEEK_END);

    int size = ftell(file_ptr);

    fseek(file_ptr, start, SEEK_SET);

    return size;
}
bool load_file(STATE* SYSTEM_STATE)
{
    char filename[] = "cards.bin";
    int filesize = 0;

    FILE* file_ptr;

    file_ptr = fopen(filename, "rb");

    if (file_ptr == NULL)
    {
        fclose(file_ptr);
        return false;
    }

    filesize = get_file_size(file_ptr);

    SYSTEM_STATE->nr_cards = filesize / sizeof(Card);
    expand_list(SYSTEM_STATE);

    fread(SYSTEM_STATE->card_list,
        sizeof(Card),
        SYSTEM_STATE->nr_cards,
        file_ptr
    );

    fclose(file_ptr);

    return true;
}
