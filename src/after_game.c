#include "h_files/board.h"
#include "h_files/after_game.h"
#include "h_files/mouse_click.h"
#include <string.h>

static GtkWidget *game_lost_window = NULL;

void on_play_again(GtkWidget *play_again, gpointer data)
{
	gd_t game_data = (gd_t)data;

	reset_board(game_data);
	set_first_click_true();
	gtk_label_set_text(GTK_LABEL(game_data->points_label), "0");

	char flags[3];
	game_data->flags = game_data->bombs;
	sprintf(flags, "%d", game_data->flags);

	gtk_label_set_text(GTK_LABEL(game_data->flags_label), flags);

	game_data->cells = init_board(game_data);
	
	gtk_widget_destroy(game_lost_window);

}

void game_lost(gd_t game_data)
{
 	GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(new_window), "");
        gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 300);

	GtkWidget *game_lost = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(new_window), game_lost);
	
	char label[50] = "You lost\nYour score: "; 

	strcat(label, gtk_label_get_text(GTK_LABEL(game_data->points_label)));

        GtkWidget *final_points = gtk_label_new(label);
	gtk_box_pack_start(GTK_BOX(game_lost), final_points, FALSE, FALSE, 25);
	gtk_label_set_justify(GTK_LABEL(final_points), GTK_JUSTIFY_CENTER);
	gtk_widget_set_size_request(final_points, 200, 50);

	GtkWidget *play_again = gtk_button_new_with_label("Play again");
	g_signal_connect(play_again, "clicked", G_CALLBACK(on_play_again), game_data);
	gtk_box_pack_start(GTK_BOX(game_lost), play_again, FALSE, FALSE, 0);
	gtk_widget_set_size_request(play_again, 200, 50);
	gtk_widget_set_halign(play_again, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(play_again, GTK_ALIGN_CENTER);

        gtk_window_set_transient_for(GTK_WINDOW(new_window), GTK_WINDOW(game_data->window));
        gtk_window_set_modal(GTK_WINDOW(new_window), FALSE); // Sprawia, że okno nadrzędne jest niedostępne

	game_lost_window = new_window;
        gtk_widget_show_all(new_window);


}


