#include "board.h"
#include <gtk/gtk.h>

void set_first_click_true();
void get_game_data(gd_t game_data);
gboolean on_button_click(GtkWidget *widget, GdkEventButton *event, gpointer data);
void on_left_click(cell_t cell);
void on_right_click(cell_t cell);
