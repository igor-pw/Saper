#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <gtk/gtk.h>

typedef struct cell
{
	bool revealed;
	bool bomb;
	bool flagged;
	int bombs;
	GtkWidget *button;	
} cell_o, *cell_t;


void on_left_click(cell_t cell)
{
	if(cell->revealed || cell->flagged)
		return;

	cell->revealed = true;

	if(cell->bomb)
		gtk_button_set_label(GTK_BUTTON(cell->button), "B");

	else
		gtk_button_set_label(GTK_BUTTON(cell->button), ":)");
	
	gtk_widget_set_sensitive(cell->button, FALSE);
}

void on_right_click(cell_t cell)
{		
	if(!cell->flagged)
	{
		cell->flagged = true;
		gtk_button_set_label(GTK_BUTTON(cell->button), "F");
	}

	else
	{
		cell->flagged = false;
		gtk_button_set_label(GTK_BUTTON(cell->button), "");
	}
}

gboolean on_button_click(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	cell_t cell = (cell_t)data;

	if(event->button == GDK_BUTTON_PRIMARY)
		on_left_click(cell);
	else if(event->button == GDK_BUTTON_SECONDARY)
		on_right_click(cell);

	return TRUE;
}
cell_t **init_cells(GtkWidget *grid, int size)
{
	//inicjacja pierwszego wymiaru dwuwymiarowej tablicy wskaznikow do struktur
	cell_t **cells = malloc(sizeof cells * size*size);	
	
	for(int i = 0; i < size; i++)
	{
		//inicjacja drugiego wymiaru dwuwymiarowej tablicy
		cells[i] = malloc(sizeof cells * size);

		for(int j = 0; j < size; j++)
		{	
			//inicjacja struktur
			cells[i][j] = malloc(sizeof **cells);
			cells[i][j]->revealed = false;
			cells[i][j]->bomb = false;
			cells[i][j]->flagged = false;
			cells[i][j]->bombs = 0;

			//inicjacja przycisku
			cells[i][j]->button = gtk_button_new_with_label("");

			//przypisanie komorki do siatki
			gtk_grid_attach(GTK_GRID(grid), cells[i][j]->button, i, j, 1, 1);

			//przypisanie przyciskowi funkcji
			g_signal_connect(cells[i][j]->button, "button-press-event", G_CALLBACK(on_button_click), cells[i][j]);
		}
	}

	return cells;
}

void add_bombs(cell_t **cells, int size, int n)
{
	int x;
	int y;

	for(int i = 0; i < n; i++)
	{
			x = rand() % size;
			y = rand() % size;
			
			if(!cells[x][y]->bomb)
				cells[x][y]->bomb = true;

			else
				i--;

	}

	return;
}


// Funkcja obsługująca zdarzenie kliknięcia przycisku
int main(int argc, char *argv[]) {
    // Inicjalizacja GTK
    gtk_init(&argc, &argv);

    // Tworzenie głównego okna
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Przykładowe GUI w GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    //gtk_container_set_border_width(GTK_CONTAINER(window), 10);


    // Tworzenie głównego kontenera
   	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box);


	GtkWidget *grid = gtk_grid_new();
	gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);

	cell_t **cells;

	int size = 10;
	int n = 9;
	cells = init_cells(grid, size);
	add_bombs(cells, size, n);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Wyświetlenie wszystkich elementów
    gtk_widget_show_all(window);

    // Uruchomienie głównej pętli GTK
    gtk_main();

    return 0;
}

