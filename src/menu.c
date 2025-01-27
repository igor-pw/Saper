#include <gtk/gtk.h>
#include "h_files/mouse_click.h"
#include "h_files/board.h"
#include "h_files/menu.h"
#include "h_files/after_game.h"

void on_easy_mode(GtkButton *button, gpointer game_data)
{       
	gd_t p_game_data = (gd_t)game_data;
        set_variables(1, p_game_data);

        //inicjacja komorek
        p_game_data->cells = init_board(p_game_data);
	start_timer(p_game_data);

	gtk_window_resize(GTK_WINDOW(p_game_data->window), p_game_data->cols*60, p_game_data->rows*50);
        gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");

	set_first_click_true();
}

void on_medium_mode(GtkButton *button, gpointer game_data)
{
	gd_t p_game_data = (gd_t)game_data;
	set_variables(2, p_game_data);

	p_game_data->cells = init_board(p_game_data);
	start_timer(p_game_data);

	gtk_window_resize(GTK_WINDOW(p_game_data->window), p_game_data->cols*60, p_game_data->rows*50);
	gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");
	
	set_first_click_true();
}

void on_hard_mode(GtkButton *button, gpointer game_data)
{
	gd_t p_game_data = (gd_t)game_data;
	set_variables(3, p_game_data);

	p_game_data->cells = init_board(p_game_data);
	start_timer(p_game_data);

	gtk_window_resize(GTK_WINDOW(p_game_data->window), p_game_data->cols*60, p_game_data->rows*50);
	gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");

	set_first_click_true();
}

void on_custom_mode(GtkButton *button, gpointer game_data)
{
	gd_t p_game_data = (gd_t)game_data;

	const char *c_rows = gtk_entry_get_text(p_game_data->rows_entry);
	const char *c_cols = gtk_entry_get_text(p_game_data->cols_entry);
	const char *c_bombs = gtk_entry_get_text(p_game_data->bombs_entry);

	int rows = atoi(c_rows);
	int cols = atoi(c_cols);
	int bombs = atoi(c_bombs);

	//p_game_data->mode = 0;

	if(rows >= 9 && cols >= 9 && bombs >= 10 && bombs < rows*cols)
	{
		if((cols >= 9 && cols < 16) || (rows >= 9 && rows < 16) || (bombs >= 10 && bombs < 40))
			p_game_data->mode = 1;

		else if((cols >= 16 && cols < 30) || (rows == 16) || (bombs >= 40 && bombs < 99))
                        p_game_data->mode = 2;

		else if((cols >= 30) || (rows > 16) || (bombs >= 99))
                        p_game_data->mode = 3;

		p_game_data->rows = rows;
		p_game_data->cols = cols;
		p_game_data->bombs = bombs;
		p_game_data->flags = bombs;

		p_game_data->cells = init_board(p_game_data);
		update_flags_label(p_game_data);
		start_timer(p_game_data);

		gtk_window_resize(GTK_WINDOW(p_game_data->window), cols*60, rows*50);	
		gtk_stack_set_visible_child_name(GTK_STACK(p_game_data->stack), "game_view");

		set_first_click_true();
	}

	else
		return;

}

void on_file_button(GtkWidget *file_button, gpointer data)
{
	set_board_loaded_true();
	gd_t game_data = (gd_t)data;

	FILE *load_board = fopen(gtk_entry_get_text(game_data->file_entry), "r");

	if(load_board == NULL)
		return;

	int rows, cols, bombs;

	fscanf(load_board, "%d %d %d", &cols, &rows, &bombs);

	game_data->rows = rows;
	game_data->cols = cols;
	game_data->bombs = bombs;

	game_data->cells = init_board(game_data);

	int x, y;

	for(int i = 0; i < bombs; i++)
	{
		fscanf(load_board, "%d %d", &y, &x);
		game_data->cells[x-1][y-1]->bomb = true;
	}

	count_connections(game_data);
    	create_connections(game_data);
     	count_bombs(game_data);

	game_data->mode = 1;

	game_data->flags = bombs;

	gtk_window_resize(GTK_WINDOW(game_data->window), cols*60, rows*50);
	gtk_stack_set_visible_child_name(GTK_STACK(game_data->stack), "game_view");

	char move;
	int correct_moves = 0;
	bool end = false;

	while(fscanf(load_board, "\n%c %d %d", &move, &y, &x) == 3)
	{
		switch(move)
		{
			case 'r':
			on_left_click(game_data->cells[x-1][y-1]);
			
			if(game_data->cells[x-1][y-1]->bomb && !end && !game_data->cells[x-1][y-1]->flagged)
			{
				end = true;
				board_loaded_lost(game_data, correct_moves);
			}

			if(game_data->revealed == game_data->cols*game_data->rows - game_data->bombs && !end)
        		{
                		end = true;
                		board_loaded_won(game_data, correct_moves);
        		}

			else
				correct_moves++;			
			break;

			case 'f':
			on_right_click(game_data->cells[x-1][y-1]);
			break;
		}
	}

	if(!end)
		board_loaded_unresolved(game_data, correct_moves);

	fclose(load_board);
}

GtkWidget *create_menu_view(gd_t game_data)
{
	GtkWidget *menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_widget_set_halign(menu, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(menu, GTK_ALIGN_CENTER);

	gtk_widget_set_vexpand(menu, FALSE);

	GtkWidget *easy_mode = gtk_button_new();
	gtk_widget_set_vexpand(easy_mode, FALSE);
	
	GtkWidget *easy_mode_label = gtk_label_new("Easy\n9x9  10 mines");
	gtk_label_set_justify(GTK_LABEL(easy_mode_label), GTK_JUSTIFY_CENTER);
	gtk_label_set_xalign(GTK_LABEL(easy_mode_label), 0.5);
	gtk_label_set_yalign(GTK_LABEL(easy_mode_label), 0.0);

	gtk_container_add(GTK_CONTAINER(easy_mode), easy_mode_label);
	gtk_widget_show(easy_mode_label);	

        g_signal_connect(easy_mode, "clicked", G_CALLBACK(on_easy_mode), game_data);
        gtk_box_pack_start(GTK_BOX(menu), easy_mode, FALSE, FALSE, 30);
        gtk_widget_set_size_request(easy_mode, 200, 50);

	GtkWidget *medium_mode = gtk_button_new();
	gtk_widget_set_vexpand(medium_mode, FALSE);

	GtkWidget *medium_mode_label = gtk_label_new("Medium\n16x16  40 mines");
	gtk_label_set_justify(GTK_LABEL(medium_mode_label), GTK_JUSTIFY_CENTER);
	gtk_label_set_xalign(GTK_LABEL(medium_mode_label), 0.5);
	gtk_label_set_yalign(GTK_LABEL(medium_mode_label), 0.0);

	gtk_container_add(GTK_CONTAINER(medium_mode), medium_mode_label);
	gtk_widget_show(medium_mode_label);

	g_signal_connect(medium_mode, "clicked", G_CALLBACK(on_medium_mode), game_data);
	gtk_box_pack_start(GTK_BOX(menu), medium_mode, FALSE, FALSE, 0);
	gtk_widget_set_size_request(medium_mode, 200, 50);

	GtkWidget *hard_mode = gtk_button_new();
	gtk_widget_set_vexpand(hard_mode, FALSE);

	GtkWidget *hard_mode_label = gtk_label_new("Hard\n16x30  99 mines");
	gtk_label_set_justify(GTK_LABEL(hard_mode_label), GTK_JUSTIFY_CENTER);
	gtk_label_set_xalign(GTK_LABEL(hard_mode_label), 0.5);
	gtk_label_set_yalign(GTK_LABEL(hard_mode_label), 0.0);

	gtk_container_add(GTK_CONTAINER(hard_mode), hard_mode_label);
	gtk_widget_show(hard_mode_label);
	
	g_signal_connect(hard_mode, "clicked", G_CALLBACK(on_hard_mode), game_data);
	gtk_box_pack_start(GTK_BOX(menu), hard_mode, FALSE, FALSE, 30);	
	gtk_widget_set_size_request(hard_mode, 200, 50);
        
	GtkWidget *custom_mode = gtk_button_new_with_label("Custom");
	g_signal_connect(custom_mode, "clicked", G_CALLBACK(on_custom_mode), game_data);
	gtk_box_pack_start(GTK_BOX(menu), custom_mode, FALSE, FALSE, 0);
	gtk_widget_set_size_request(custom_mode, 200, 50);
	
	GtkWidget *entry_grid = gtk_grid_new();
	
	gtk_widget_set_halign(entry_grid, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(entry_grid, GTK_ALIGN_CENTER);
	
	gtk_grid_set_column_spacing(GTK_GRID(entry_grid), 5);
	gtk_widget_set_vexpand(entry_grid, FALSE);
	gtk_widget_set_size_request(entry_grid, 50, 30);

	GtkWidget *rows_entry = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(rows_entry), 10);  // Szerokość na podstawie 3 znaków
	gtk_entry_set_placeholder_text(GTK_ENTRY(rows_entry), "rows");

	GtkWidget *cols_entry = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(cols_entry), 10);
	gtk_entry_set_placeholder_text(GTK_ENTRY(cols_entry), "cols");

	GtkWidget *bombs_entry = gtk_entry_new();
        gtk_entry_set_width_chars(GTK_ENTRY(bombs_entry), 10);
	gtk_entry_set_placeholder_text(GTK_ENTRY(bombs_entry), "mines");

	gtk_grid_attach(GTK_GRID(entry_grid), rows_entry, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(entry_grid), cols_entry, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(entry_grid), bombs_entry, 2, 0, 1, 1);
	
	gtk_box_pack_start(GTK_BOX(menu), entry_grid, FALSE, FALSE, 20);

	GtkWidget *file_entry = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(file_entry), 30);
	gtk_entry_set_placeholder_text(GTK_ENTRY(file_entry), "file path");
	gtk_box_pack_start(GTK_BOX(menu), file_entry, FALSE, FALSE, 5);
	
	GtkWidget *file_button = gtk_button_new_with_label("Load board");
	g_signal_connect(file_button, "clicked", G_CALLBACK(on_file_button), game_data);
	gtk_box_pack_start(GTK_BOX(menu), file_button, FALSE, FALSE, 0);
	gtk_widget_set_vexpand(medium_mode, FALSE);
        gtk_widget_set_size_request(custom_mode, 200, 50);

	game_data->rows_entry = (GtkEntry *)rows_entry;
	game_data->cols_entry = (GtkEntry *)cols_entry;
	game_data->bombs_entry = (GtkEntry* )bombs_entry;
	game_data->file_entry = (GtkEntry* )file_entry;

	gtk_stack_add_named(GTK_STACK(game_data->stack), menu, "menu_view");
	gtk_stack_set_visible_child_name(GTK_STACK(game_data->stack), "menu_view");

	return menu;
}

