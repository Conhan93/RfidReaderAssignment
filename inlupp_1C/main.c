#include <stdio.h>
#include <time.h>
#include "safeinput.h"
#include "state.h"
#include "admin.h"


void list_cards(STATE* SYSTEM_STATE);
Card* search_id(STATE* SYSTEM_STATE);
void change_card_access(STATE* SYSTEM_STATE);
int admin_menu();

void send_card(STATE* SYSTEM_STATE);
void invalid_input();
bool add_new_card();


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
        "\n4. Add/remove access"
        "\n5. Save cards to file"
        "\n6. Send a card to device"
        "\n7. Clear all cards on device"
        "\n9. Exit\n");

    while (!GetInputInt(NULL, &selection)) invalid_input();

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
    char new_card_id[STRLEN];

    printf("\nValid card in form x.x.x.x.x where x is an integer between 1 and 255\n");

    while (GetInput("\nAnge kortnr: ", new_card_id, sizeof(new_card_id))) invalid_input();
    if (valid_id(new_card_id))
    {
        if ((active_card = get_card(SYSTEM_STATE->card_list, SYSTEM_STATE->nr_cards, new_card_id)) == NULL)
        {
            if (add_new_card())
                add_card(SYSTEM_STATE, new_card_id);
            return;
        }
        else
        {
            int new_access = 0;

            printf("\nThis card has %s, enter 1 for access, 2 for no access",
                active_card->access ? "access" : "no access");
            // sets new access for card
            if (GetInputInt(NULL, &new_access))
            {
                if (new_access == 0 || new_access == 1)
                    active_card->access = new_access;

            }
            else invalid_input();
        }
    }
    else printf("\nInvalid ID...");
    

    printf("\nExiting access...");

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
        else invalid_input();
    }
    return active_card;
}



void send_card(STATE* SYSTEM_STATE)
{
    /*    Sequence for adding a card to device.     */

    Card* active_card = NULL;
    unsigned char message[BUFFERSIZE] = "";
    

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
// flushes stdin and prints out invalid message
void invalid_input() { fflush(stdin); printf("\nInvalid input..."); }
bool add_new_card()
{
    char sel = 0;
    while (!GetInputChar("\nID not found, create new card"
        "with entered id?(y/n): ", &sel)) invalid_input();
    switch (sel)
    {
    case 'y' :
        return true;
    case 'n' : 
        return false;
    default:
        printf("\nInvalid input..");
        break;
    }
}

