#include "state.h"



// for device_com
typedef enum
{
	COMMAND_INDEX,
	OPENDOOR,
	CLEARCARDS,
	ADDCARD
};

// file handling -- files.c
void save_to_file(STATE* SYSTEM_STATE);
bool load_file(STATE* SYSTEM_STATE);
int get_file_size(FILE* file_ptr);

// device communication -- device_com.c
bool remote_open_door(STATE* SYSTEM_STATE);
void create_message(unsigned char* message_string, Card* card);
bool clear_cards(STATE* SYSTEM_STATE);

// card and list functions -- admin.c
bool valid_id(char* new_id);
void expand_list(STATE* SYSTEM_STATE);
Card create_card(char* new_id);
void format_date_string(time_t date_added, char* buffer, int buffersize);
Card* get_card(Card* card_list, int nr_cards, char* search_term);
void add_card(STATE* SYSTEM_STATE, char* new_card_id);

// state initialization -- admin.c
void initialize_state(STATE* SYSTEM_STATE);
