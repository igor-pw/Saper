#include <time.h>
#include <gtk/gtk.h>
#include "h_files/board.h"
#include "h_files/mouse_click.h"

static void on_game_start(GtkButton *button, gpointer stack, cell_t **cells)
{	
	//set_variables(2);
	
        //inicjacja komorek
	
	gtk_stack_set_visible_child_name(GTK_STACK(stack), "Game");
}

// Funkcja obsługująca zdarzenie kliknięcia przycisku
int main(int argc, char *argv[]) 
{
    	//inicjalizacja GTK
    	gtk_init(&argc, &argv);

	srand(time(NULL));

    	//tworzenie głównego okna
    	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	gtk_window_set_title(GTK_WINDOW(window), "Saper");
    	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 
	GtkWidget *stack = gtk_stack_new();
	gtk_container_add(GTK_CONTAINER(window), stack);

	//menu
	GtkWidget *menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    	gtk_stack_add_named(GTK_STACK(stack), menu_box, "menu_view");
	gtk_widget_set_halign(menu_box, GTK_ALIGN_CENTER);
	//gtk_widget_set_valign(menu_box, GTK_ALIGN_CENTER);

	GtkWidget *start_button = gtk_button_new_with_label("Easy Mode");
    	g_signal_connect(start_button, "clicked", G_CALLBACK(on_game_start), stack);
    	gtk_box_pack_start(GTK_BOX(menu_box), start_button, FALSE, FALSE, 100);
    	gtk_widget_set_size_request(start_button, 150, 50);
	
	//tworzenie widgetow 
   	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_stack_add_named(GTK_STACK(stack), box, "Game");
	GtkWidget *grid = gtk_grid_new();
	gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
	//gtk_container_add(GTK_CONTAINER(window), box);

	//wysrodkowanie planszy
	gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); 
	gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); 
	
	cell_t **cells;

	//inicjacja komorek
	cells = init_cells(grid);
	get_p_cells(cells);
	
	gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Wyświetlenie wszystkich elementów
    gtk_widget_show_all(window);

    // Uruchomienie głównej pętli GTK
    gtk_main();

    return 0;
}

