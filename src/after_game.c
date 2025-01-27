#include "h_files/board.h"
#include "h_files/after_game.h"
#include "h_files/mouse_click.h"
#include <string.h>
#include <stdio.h>

static GtkWidget *after_game_window = NULL;
static GtkWidget *p_game_over = NULL;
static GtkWidget *p_player_name = NULL;

void on_play_again(GtkWidget *play_again, gpointer data)
{
	set_end_false();
	gd_t game_data = (gd_t)data;

	reset_board(game_data);
	set_first_click_true();
	gtk_label_set_text(GTK_LABEL(game_data->points_label), "0");

	gtk_label_set_text(GTK_LABEL(game_data->timer_label), "0");
	start_timer(game_data);

	char flags[3];
	game_data->flags = game_data->bombs;
	sprintf(flags, "%d", game_data->flags);

	gtk_label_set_text(GTK_LABEL(game_data->flags_label), flags);

	game_data->cells = init_board(game_data);
	
	gtk_widget_destroy(after_game_window);

}

void show_scoreboard()
{
	FILE *scoreboard = fopen("scoreboard/easy_mode.txt", "r");
	int score;
	char player_name[32];

	char buf[50];

	for(int i = 1; i <= 5; i++)
	{
		if(fscanf(scoreboard, "%s %d", player_name, &score) == 2)
		{
			sprintf(buf, "%d. %s %d", i, player_name, score);

			GtkWidget *player_score = gtk_label_new(buf);
			gtk_box_pack_start(GTK_BOX(p_game_over), player_score, FALSE, FALSE, 5);
			gtk_widget_set_halign(player_score, GTK_ALIGN_CENTER);
			gtk_widget_set_valign(player_score, GTK_ALIGN_CENTER);
			gtk_widget_show(player_score);
		}
	}

	fclose(scoreboard);	
}

void on_back_to_menu(GtkWidget *back_to_menu, gpointer data)
{
	set_end_false();
	gd_t game_data = (gd_t)data;

	reset_board(game_data);
	set_first_click_true();
	gtk_label_set_text(GTK_LABEL(game_data->points_label), "0");

	gtk_label_set_text(GTK_LABEL(game_data->timer_label), "0");

	char flags[3];
        game_data->flags = game_data->bombs;
        sprintf(flags, "%d", game_data->flags);

	gtk_label_set_text(GTK_LABEL(game_data->flags_label), flags);

	gtk_widget_destroy(after_game_window);

	gtk_window_set_resizable(GTK_WINDOW(game_data->window), FALSE);
	gtk_stack_set_visible_child_name(GTK_STACK(game_data->stack), "menu_view");
	gtk_window_resize(GTK_WINDOW(game_data->window), 450, 400);
}

void on_save_player_name(GtkWidget *save_player, gpointer data)
{
	gd_t game_data = (gd_t)data;

	const char *player_name = gtk_entry_get_text(GTK_ENTRY(p_player_name));

	if(player_name[0] == '\0')
		player_name = "Anonymous";

	add_to_scoreboard(game_data, player_name);	

	gtk_widget_destroy(p_player_name);
	gtk_widget_destroy(save_player);

	GtkWidget *scoreboard  = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(scoreboard), "<span font='20'>Scoreboard</span>");

	gtk_box_pack_start(GTK_BOX(p_game_over), scoreboard, FALSE, FALSE, 25);
	gtk_label_set_justify(GTK_LABEL(scoreboard), GTK_JUSTIFY_CENTER);
	gtk_widget_set_size_request(scoreboard, 200, 50);

	show_scoreboard();	

	GtkWidget *play_again = gtk_button_new_with_label("Play again");
	g_signal_connect(play_again, "clicked", G_CALLBACK(on_play_again), game_data);
	gtk_box_pack_start(GTK_BOX(p_game_over), play_again, FALSE, FALSE, 10);
	gtk_widget_set_size_request(play_again, 150, 30);
	gtk_widget_set_halign(play_again, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(play_again, GTK_ALIGN_CENTER);

	gtk_widget_show(play_again);

	GtkWidget *back_to_menu = gtk_button_new_with_label("Back to menu");
	g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu), game_data);
	gtk_box_pack_start(GTK_BOX(p_game_over), back_to_menu, FALSE, FALSE, 0);
	gtk_widget_set_size_request(back_to_menu, 150, 30);
        gtk_widget_set_halign(back_to_menu, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(back_to_menu, GTK_ALIGN_CENTER);

        gtk_widget_show(back_to_menu);

}

void save_player_name(GtkWidget *game_over, gd_t game_data)
{
	
	GtkWidget *player_name = gtk_entry_new();
	gtk_entry_set_width_chars(GTK_ENTRY(player_name), 32);
	gtk_entry_set_placeholder_text(GTK_ENTRY(player_name), "Anonymous");
	gtk_box_pack_start(GTK_BOX(game_over), player_name, FALSE, FALSE, 0);
	gtk_widget_set_halign(player_name, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(player_name, GTK_ALIGN_CENTER);

	GtkWidget *save_player_name = gtk_button_new_with_label("Add name");
	g_signal_connect(save_player_name, "clicked", G_CALLBACK(on_save_player_name), game_data);
	gtk_box_pack_start(GTK_BOX(game_over), save_player_name, FALSE, FALSE, 10);
	gtk_widget_set_halign(save_player_name, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(save_player_name, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request(save_player_name, 150, 30);

	p_player_name = player_name;

}

void add_to_scoreboard(gd_t game_data, const char *player_name)
{
	FILE *scoreboard = fopen("scoreboard/easy_mode.txt", "r+");
	FILE *new_scoreboard = fopen("scoreboard/new_scoreboard.txt", "w");
	char new_name[32];

	sprintf(new_name, "%s", player_name);

	char name[32];
	int score;
	//int line = 0;
	bool replaced = false;

	while(fscanf(scoreboard, "%s %d", name, &score) == 2)
	{
		//line++;
       
            	if(!replaced && score < atoi(gtk_label_get_text(GTK_LABEL(game_data->points_label)))) 
		{
			replaced = true;
                	int new_score = atoi(gtk_label_get_text(GTK_LABEL(game_data->points_label)));
			char new_data[50];

			sprintf(new_data, "%s %d\n", new_name, new_score);
			
			fputs(new_data, new_scoreboard);
            	}

      		char buf[50];
		sprintf(buf, "%s %d\n", name, score);
		fputs(buf, new_scoreboard);
		
        }

	if(!replaced)
	{
		int new_score = atoi(gtk_label_get_text(GTK_LABEL(game_data->points_label)));
		char new_data[50];

      	        sprintf(new_data, "%s %d\n", new_name, new_score);

              	fputs(new_data, new_scoreboard);

	}

	fclose(scoreboard);
	fclose(new_scoreboard);

	remove("scoreboard/easy_mode.txt");

	rename("scoreboard/new_scoreboard.txt", "scoreboard/easy_mode.txt");

}

void game_lost(gd_t game_data)
{
 	GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(new_window), "");
        gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 350);

	GtkWidget *game_lost = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(new_window), game_lost);

	p_game_over = game_lost;

	char label[50] = "You lost\nYour score: "; 

	strcat(label, gtk_label_get_text(GTK_LABEL(game_data->points_label)));

        GtkWidget *final_points = gtk_label_new(label);
	gtk_box_pack_start(GTK_BOX(game_lost), final_points, FALSE, FALSE, 20);
	gtk_label_set_justify(GTK_LABEL(final_points), GTK_JUSTIFY_CENTER);
	gtk_widget_set_size_request(final_points, 200, 50);

	save_player_name(game_lost, game_data);

        gtk_window_set_transient_for(GTK_WINDOW(new_window), GTK_WINDOW(game_data->window));
        gtk_window_set_modal(GTK_WINDOW(new_window), FALSE); 
	after_game_window = new_window;
        gtk_widget_show_all(new_window);

}

void game_won(gd_t game_data)
{
	GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(new_window), "");
        gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 350);

        GtkWidget *game_won = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(new_window), game_won);

	p_game_over = game_won;

	char label[50] = "You won\nYour score: ";

        strcat(label, gtk_label_get_text(GTK_LABEL(game_data->points_label)));

        GtkWidget *final_points = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(game_won), final_points, FALSE, FALSE, 20);
        gtk_label_set_justify(GTK_LABEL(final_points), GTK_JUSTIFY_CENTER);
        gtk_widget_set_size_request(final_points, 200, 50);

	save_player_name(game_won, game_data);

        gtk_window_set_transient_for(GTK_WINDOW(new_window), GTK_WINDOW(game_data->window));
        gtk_window_set_modal(GTK_WINDOW(new_window), FALSE);

        after_game_window = new_window;	
        gtk_widget_show_all(new_window);
}

void board_loaded_won(gd_t game_data, int correct_moves)
{
	GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(new_window), "");
        gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 200);

        GtkWidget *game_won = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(new_window), game_won);

        char label[50] = "You won\nYour score: ";

        strcat(label, gtk_label_get_text(GTK_LABEL(game_data->points_label)));

        GtkWidget *final_points = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(game_won), final_points, FALSE, FALSE, 20);
        gtk_label_set_justify(GTK_LABEL(final_points), GTK_JUSTIFY_CENTER);
        gtk_widget_set_size_request(final_points, 200, 50);

	char moves[50];

	sprintf(moves, "Correct moves: %d", correct_moves);

	GtkWidget *moves_info = gtk_label_new(moves);
	gtk_box_pack_start(GTK_BOX(game_won), moves_info, FALSE, FALSE, 0);
        gtk_label_set_justify(GTK_LABEL(moves_info), GTK_JUSTIFY_CENTER);
        gtk_widget_set_size_request(moves_info, 150, 50);
	
	GtkWidget *back_to_menu = gtk_button_new_with_label("Back to menu");
        g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu), game_data);
        gtk_widget_set_valign(back_to_menu, GTK_ALIGN_CENTER);
	gtk_widget_set_halign(back_to_menu, GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(game_won), back_to_menu, FALSE, FALSE, 0);
        gtk_widget_set_size_request(back_to_menu, 150, 30);

	after_game_window = new_window;
		
	gtk_window_set_transient_for(GTK_WINDOW(new_window), GTK_WINDOW(game_data->window));
        gtk_window_set_modal(GTK_WINDOW(new_window), FALSE);
	gtk_widget_show_all(new_window);
}

void board_loaded_lost(gd_t game_data, int correct_moves)
{
        GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(new_window), "");
        gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 200);

        GtkWidget *game_lost = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(new_window), game_lost);

        char label[50] = "You lost\nYour score: ";

        strcat(label, gtk_label_get_text(GTK_LABEL(game_data->points_label)));

        GtkWidget *final_points = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(game_lost), final_points, FALSE, FALSE, 20);
        gtk_label_set_justify(GTK_LABEL(final_points), GTK_JUSTIFY_CENTER);
	gtk_widget_set_size_request(final_points, 200, 50);

	char moves[50];

        sprintf(moves, "Correct moves: %d", correct_moves);

        GtkWidget *moves_info = gtk_label_new(moves);
        gtk_box_pack_start(GTK_BOX(game_lost), moves_info, FALSE, FALSE, 0);
        gtk_label_set_justify(GTK_LABEL(moves_info), GTK_JUSTIFY_CENTER);
        gtk_widget_set_size_request(moves_info, 200, 50);

	GtkWidget *back_to_menu = gtk_button_new_with_label("Back to menu");
        g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu), game_data);
        gtk_widget_set_valign(back_to_menu, GTK_ALIGN_CENTER);
        gtk_widget_set_halign(back_to_menu, GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(game_lost), back_to_menu, FALSE, FALSE, 0);
        gtk_widget_set_size_request(back_to_menu, 150, 30);

	after_game_window = new_window;

        gtk_window_set_transient_for(GTK_WINDOW(new_window), GTK_WINDOW(game_data->window));
        gtk_window_set_modal(GTK_WINDOW(new_window), FALSE);

        gtk_widget_show_all(new_window);
}

void board_loaded_unresolved(gd_t game_data, int correct_moves)
{
        GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(new_window), "");
        gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 200);

        GtkWidget *game_unresolved = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(new_window), game_unresolved);

        char label[50] = "Game unresolved\nYour score: ";

        strcat(label, gtk_label_get_text(GTK_LABEL(game_data->points_label)));

        GtkWidget *final_points = gtk_label_new(label);
        gtk_box_pack_start(GTK_BOX(game_unresolved), final_points, FALSE, FALSE, 20);
        gtk_label_set_justify(GTK_LABEL(final_points), GTK_JUSTIFY_CENTER);
        gtk_widget_set_size_request(final_points, 200, 50);

	char moves[50];

        sprintf(moves, "Correct moves: %d", correct_moves);

        GtkWidget *moves_info = gtk_label_new(moves);
        gtk_box_pack_start(GTK_BOX(game_unresolved), moves_info, FALSE, FALSE, 0);
        gtk_label_set_justify(GTK_LABEL(moves_info), GTK_JUSTIFY_CENTER);
        gtk_widget_set_size_request(moves_info, 200, 50);

	GtkWidget *back_to_menu = gtk_button_new_with_label("Back to menu");
	g_signal_connect(back_to_menu, "clicked", G_CALLBACK(on_back_to_menu), game_data);
	gtk_widget_set_valign(back_to_menu, GTK_ALIGN_CENTER);
        gtk_widget_set_halign(back_to_menu, GTK_ALIGN_CENTER);
	gtk_box_pack_start(GTK_BOX(game_unresolved), back_to_menu, FALSE, FALSE, 0);
        gtk_widget_set_size_request(back_to_menu, 150, 30);

	after_game_window = new_window;

        gtk_window_set_transient_for(GTK_WINDOW(new_window), GTK_WINDOW(game_data->window));
        gtk_window_set_modal(GTK_WINDOW(new_window), FALSE);

	gtk_widget_show_all(new_window);
}

