#include <gtk/gtk.h>
#include "h_files/mouse_click.h"
#include "h_files/board.h"
#include "h_files/menu.h"

void on_easy_mode(GtkButton *button, gpointer game_data)
{       
	gd_t p_game_data = (gd_t)game_data;
        set_variables(1, p_game_data);

        //inicjacja komorek
        p_game_data->cells = init_cells(p_game_data);

        gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");
}

void on_medium_mode(GtkButton *button, gpointer game_data)
{
	gd_t p_game_data = (gd_t)game_data;
	set_variables(2, p_game_data);

	p_game_data->cells = init_cells(p_game_data);

	gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");
}

void on_hard_mode(GtkButton *button, gpointer game_data)
{
	gd_t p_game_data = (gd_t)game_data;
	set_variables(3, p_game_data);

	p_game_data->cells = init_cells(p_game_data);

	gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");
}

GtkWidget *create_menu_view(gd_t game_data)
{
	//g_print("c_m_v Stack: %p\n", game_data->stack);

	GtkWidget *menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_halign(menu, GTK_ALIGN_CENTER);

	GtkWidget *easy_mode = gtk_button_new_with_label("Easy Mode");
        g_signal_connect(easy_mode, "clicked", G_CALLBACK(on_easy_mode), game_data);
        gtk_box_pack_start(GTK_BOX(menu), easy_mode, FALSE, FALSE, 50);
        gtk_widget_set_size_request(easy_mode, 200, 50);

	GtkWidget *medium_mode = gtk_button_new_with_label("Medium Mode");
	g_signal_connect(medium_mode, "clicked", G_CALLBACK(on_medium_mode), game_data);
	gtk_box_pack_start(GTK_BOX(menu), medium_mode, FALSE, FALSE, 0);
	gtk_widget_set_size_request(medium_mode, 200, 50);

        gtk_stack_add_named(GTK_STACK(game_data->stack), menu, "menu_view");
	gtk_stack_set_visible_child_name(GTK_STACK(game_data->stack), "menu_view");


	GtkWidget *hard_mode = gtk_button_new_with_label("Hard Mode");
	g_signal_connect(hard_mode, "clicked", G_CALLBACK(on_hard_mode), game_data);
	gtk_box_pack_start(GTK_BOX(menu), hard_mode, FALSE, FALSE, 50);
	gtk_widget_set_size_request(hard_mode, 200, 50);
	return menu;
}

