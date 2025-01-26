#include <time.h>
#include <gtk/gtk.h>
#include "h_files/board.h"
#include "h_files/mouse_click.h"
#include "h_files/menu.h"

//funkcja obslugujaca zdarzenie kliknicia przycisku
int main(int argc, char *argv[]) 
{
    	//inicjalizacja GTK
    	gtk_init(&argc, &argv);

	srand(time(NULL));

    	//utworzenie głównego okna
    	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(window), "Saper");
    	gtk_window_set_default_size(GTK_WINDOW(window), 450, 400);
    	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

	//utworzenie stosu dla widokow programu
	GtkWidget *stack = gtk_stack_new();
	gtk_container_add(GTK_CONTAINER(window), stack);

	//utworzenie widoku gry
	GtkWidget *game = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	//utworzenie siatki z informacjami o grze
	GtkWidget *game_info = gtk_grid_new();
	GtkWidget *timer_label = gtk_label_new("0");
	GtkWidget *points_label = gtk_label_new("0");
	GtkWidget *flags_label = gtk_label_new("0");

	//przypisanie informacji o grze do siatki
	gtk_grid_attach(GTK_GRID(game_info), timer_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(game_info), points_label, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(game_info), flags_label, 2, 0, 1, 1);

	//przypisanie siatki do widoku gry
	gtk_box_pack_start(GTK_BOX(game), game_info, TRUE, TRUE, 0);
	
	//wysrodkowanie siatki z informacjami o grze
	gtk_widget_set_halign(game_info, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(game_info, GTK_ALIGN_CENTER);
	gtk_grid_set_column_spacing(GTK_GRID(game_info), 50);

	//utworzenie siatki planszy
	GtkWidget *board = gtk_grid_new();
	gtk_box_pack_start(GTK_BOX(game), board, TRUE, TRUE, 0);
	
	//wysrodkowanie planszy
	gtk_widget_set_halign(board, GTK_ALIGN_CENTER); 
	gtk_widget_set_valign(board, GTK_ALIGN_CENTER); 
	
	//zainicjowanie danych gry
	gd_t game_data = init_GameData(window, stack, board, timer_label, points_label, flags_label);
	get_game_data(game_data);

	//utworzenie widoku menu
	GtkWidget *menu = create_menu_view(game_data);

	//dodanie widoku menu do stosu
	gtk_stack_add_named(GTK_STACK(stack), game, "game_view");
	
	//animacja przejsc miedzy widokami 
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	
	//obsluga wyjscia z programu
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //wyswietlenie wszystkich elementow
    gtk_widget_show_all(window);

    //uruchomienie glownej petli GTK
    gtk_main();

    return 0;
}

