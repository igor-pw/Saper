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

    	//tworzenie głównego okna
    	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(window), "Saper");
    	gtk_window_set_default_size(GTK_WINDOW(window), 500, 200);
    	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	GtkWidget *stack = gtk_stack_new();
	gtk_container_add(GTK_CONTAINER(window), stack);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *grid = gtk_grid_new();

	gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
	
	gd_t game_data = init_GameData(stack, grid);

	get_game_data(game_data);

	GtkWidget *menu = create_menu_view(game_data);
	//gtk_stack_set_visible_child_name(GTK_STACK(game_data->stack), "menu_view");

	//wysrodkowanie planszy
	gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); 
	gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); 
   	
	gtk_stack_add_named(GTK_STACK(stack), box, "game_view");
	
	//inicjacja komorek
	
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //wyswietlenie wszystkich elementow
    gtk_widget_show_all(window);

    //uruchomienie glownej petli GTK
    gtk_main();

    return 0;
}

