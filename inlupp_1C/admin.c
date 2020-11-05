#include "admin.h"

void initialize_state(STATE* SYSTEM_STATE)
{
    // System state initialization

    SYSTEM_STATE->card_list = NULL;
    SYSTEM_STATE->nr_cards = 0;
    // initialize port
    SYSTEM_STATE->port = SerialInit("COM3");

}


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
        if (atoi(digit) > 255 || atoi(digit) <= 0) return false;

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
