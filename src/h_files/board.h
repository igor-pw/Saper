#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct cell
{
        bool revealed;
        bool bomb;
        bool flagged;
        int bombs;
        int connections;
        GtkWidget *button;
        struct cell **neighbour;
} cell_o, *cell_t;

typedef struct GameData
{
	GtkWidget *window;
	GtkWidget *stack;
	GtkWidget *grid;
	GtkWidget *points_label;
	GtkWidget *flags_label;
	GtkWidget *timer_label;
	GtkEntry *rows_entry;
	GtkEntry *cols_entry;
	GtkEntry *bombs_entry;
	cell_t **cells;
	int mode;
	int flags;
	int revealed;
	int cols;
	int rows;
	int bombs;
} *gd_t;

gd_t init_GameData(GtkWidget *window, GtkWidget *stack, GtkWidget *board, GtkWidget *timer_label, GtkWidget *points_label, GtkWidget *flags_label);
void set_variables(int mode, gd_t gama_data);
void update_points_label(gd_t game_data);
void update_flags_label(gd_t game_data);
cell_t **init_board(gd_t game_data);
void add_bombs(gd_t game_data);
void count_connections(gd_t game_data);
void create_connections(gd_t gama_data);
void count_bombs(gd_t game_data);
void game_over(gd_t game_data);
void win(gd_t game_data);
void reset_board(gd_t game_data);

#endif
