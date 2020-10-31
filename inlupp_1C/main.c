#include <stdio.h>
#include <time.h>
#include "safeinput.h"
#include "state.h"
#include "admin.h"


void list_cards(STATE* SYSTEM_STATE);
Card* search_id(STATE* SYSTEM_STATE);
void change_card_access(STATE* SYSTEM_STATE);
int admin_menu();
void add_card(STATE* SYSTEM_STATE);
void send_card(STATE* SYSTEM_STATE);

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
            if (remote_open_door(&SYSTEM_STATE)) printf("\nDoor Open");
            else printf("\nUnable to open door");
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
            if (clear_cards(&SYSTEM_STATE)) printf("\nCards cleared");
            else printf("\nUnable to clear cards");
            break;
        case 9:
            SerialClose(SYSTEM_STATE.port);
            return 0;

        default:
            printf("\nError! invalid input.");
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
    /*
        Handles changing card access value
    */
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
        if (GetInput("Enter card ID(type exit to quit): ", search_term, sizeof(search_term)) == INPUT_RESULT_OK)
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

void add_card(STATE* SYSTEM_STATE)
{ /*
        Handles sequence for adding a new card to card_list.
  */
    char new_card_id[STRLEN];
    do
 
    {
        printf("\nValid card in form x.x.x.x.x where x is an integer between 1 and 255\n");

        GetInput("Ange kortnr: ", new_card_id, sizeof(new_card_id));

        // if valid id and no duplicates exist
        if (valid_id(new_card_id) && get_card(SYSTEM_STATE->card_list,
                                     SYSTEM_STATE->nr_cards, new_card_id) == NULL )
        {
            expand_list(SYSTEM_STATE);
            SYSTEM_STATE->card_list[SYSTEM_STATE->nr_cards] = create_card(new_card_id);
            SYSTEM_STATE->nr_cards++;
            return;
        }
        else printf("\nInvalid ID");

    } while (true);
}

void send_card(STATE* SYSTEM_STATE)
{
    /*    Sequence for adding a card to device.     */

    Card* active_card = NULL;
    unsigned char message[BUFFERSIZE] = "";
    //bool access = false;

    do
    {
        // fetch card
        if ((active_card = search_id(SYSTEM_STATE)) == NULL) return;

        // check if card has access
        if (active_card->access) break;
        
        printf("\nInvalid card. No Access.");

    } while (true);
    
    
    // compose message to device
    create_message(message,
        active_card
    );

    
    // send message to device
    SerialWritePort(
        SYSTEM_STATE->port,
        message,
        BUFFERSIZE
    );
}


