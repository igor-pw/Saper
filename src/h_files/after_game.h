#include "board.h"

void on_play_again(GtkWidget *play_again, gpointer data);
void show_scoreboard();
void on_back_to_menu(GtkWidget *back_to_menu, gpointer data);
void on_save_player_name(GtkWidget *save_player_name, gpointer data);
void save_player_name(GtkWidget *game_over, gd_t game_data);
void add_to_scoreboard(gd_t game_data, const char *player_name);
void game_lost(gd_t game_data);
void game_won(gd_t game_data);
