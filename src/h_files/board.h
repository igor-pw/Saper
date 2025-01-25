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
        bool corner;
        bool edge;
        int bombs;
        GtkWidget *button;
        int connections;
        struct cell **neighbour;
} cell_o, *cell_t;

typedef struct GameData
{
	GtkWidget *stack;
	GtkWidget *grid;
	cell_t **cells;
	int cols;
	int rows;
	int bombs;
} *gd_t;

gd_t init_GameData(GtkWidget *stack, GtkWidget *grid);
void set_variables(int mode, gd_t gama_data);
cell_t **init_cells(gd_t game_data);
void add_bombs(gd_t game_data);
void count_connections(gd_t game_data);
void create_connections(gd_t gama_data);
void count_bombs(gd_t game_data);
void game_over(gd_t game_data);
void win(gd_t game_data);
void free_memory(gd_t game_data);

#endif
