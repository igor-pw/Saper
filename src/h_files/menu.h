#include <gtk/gtk.h>

void on_easy_mode(GtkButton *button, gpointer game_data);
void on_medium_mode(GtkButton *button, gpointer game_data);
void on_hard_mode(GtkButton *button, gpointer game_data);
void on_custom_mode(GtkButton *button, gpointer game_data);
void on_file_button(GtkWidget *file_button, gpointer data);
GtkWidget *create_menu_view(gd_t game_data);
