#include "board.h"
#include <gtk/gtk.h>

void get_p_cells(cell_t **cells);
gboolean on_button_click(GtkWidget *widget, GdkEventButton *event, gpointer data);
void on_left_click(cell_t cell);
void on_right_click(cell_t cell);
