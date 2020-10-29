#include "admin.h"




bool valid_id(char* new_id)
{
    // checks if new card id input is valid or not
    char temp[STRLEN], delim[] = ".";
    char* digit;
    int counter = 0;

    strcpy(temp, new_id);

    digit = strtok(temp, delim);
    while (digit != NULL)
    {
        if (atoi(digit) > 257 || atoi(digit) < 0) return false;

        else digit = strtok(NULL, delim);

        counter++;
    }
    return counter == 5 ? true : false;
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
void remote_open_door(STATE* SYSTEM_STATE)
{
    /*
    *   Opens door on device.
    */
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
void save_to_file(STATE* SYSTEM_STATE)
{
    /*  Stores card list in file, creates file if one doesn't exist     */

    char filename[] = "cards.bin";

    FILE* file_ptr;
    // open in write binary
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
    // returns the size of the file.
    int start = ftell(file_ptr);

    fseek(file_ptr, 0L, SEEK_END);

    int size = ftell(file_ptr);

    fseek(file_ptr, start, SEEK_SET);

    return size;
}
bool load_file(STATE* SYSTEM_STATE)
{
    /* Loads cards from files if available  */


    char filename[] = "cards.bin";
    int filesize = 0;

    FILE* file_ptr;
    // open read binary
    file_ptr = fopen(filename, "rb");

    if (file_ptr == NULL)
    {
        fclose(file_ptr);
        return false;
    }
    // get size of file
    filesize = get_file_size(file_ptr);

    // get amount of cards in file and expand list to accommodate
    SYSTEM_STATE->nr_cards = filesize / sizeof(Card);
    expand_list(SYSTEM_STATE);

    // get cards
    fread(SYSTEM_STATE->card_list,
        sizeof(Card),
        SYSTEM_STATE->nr_cards,
        file_ptr
    );

    fclose(file_ptr);

    return true;
}