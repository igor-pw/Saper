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

void set_variables(int mode);
cell_t **init_cells(GtkWidget *grid);
void add_bombs(cell_t **cells);
void count_connections(cell_t **cells);
void create_connections(cell_t **cells);
void count_bombs(cell_t **cells);
void game_over(cell_t **cells);
void win(cell_t **cells);
void free_memory(cell_t **cells);

#endif
