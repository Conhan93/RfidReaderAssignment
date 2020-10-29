#include "admin.h"



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