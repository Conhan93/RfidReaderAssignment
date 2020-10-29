#include "state.h"

// file handling
void save_to_file(STATE* SYSTEM_STATE);
bool load_file(STATE* SYSTEM_STATE);
int get_file_size(FILE* file_ptr);

// device communication
void remote_open_door(STATE* SYSTEM_STATE);
void create_message(char* message_string, Card* card, char* command);
bool clear_cards(STATE* SYSTEM_STATE);

// card and list functions
bool valid_id(char* new_id);
void expand_list(STATE* SYSTEM_STATE);
Card create_card(char* new_id);
void format_date_string(time_t date_added, char* buffer, int buffersize);
Card* get_card(Card* card_list, int nr_cards, char* search_term);

// state initialization
void initialize_state(STATE* SYSTEM_STATE);
