#include "h_files/mouse_click.h"
#include "h_files/board.h"

static bool first_click = true;
static cell_t **p_cells = NULL;

void get_p_cells(cell_t **cells)
{
	p_cells = cells;
	return;
}

gboolean on_button_click(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
        cell_t cell = (cell_t)data;

        if(first_click)
        {
		first_click = false;
                
		add_bombs(p_cells);
                count_connections(p_cells);
                create_connections(p_cells);
                count_bombs(p_cells);

		on_left_click(cell);
                cell->revealed = true;
        }

        if(event->button == GDK_BUTTON_PRIMARY)
                on_left_click(cell);
        else if(event->button == GDK_BUTTON_SECONDARY)
                on_right_click(cell);

        return TRUE;
}

void on_left_click(cell_t cell)
{
        if(first_click)
                first_click = false;

        if(cell->flagged || cell->revealed) 
 		return;

        cell->revealed = true;
        gtk_widget_set_sensitive(cell->button, FALSE);

        if(cell->bomb)
	{
	        gtk_button_set_label(GTK_BUTTON(cell->button), "💣");
		game_over(p_cells);
	}
        else
        {
                if(cell->bombs > 0)
                {
                        char number[2];
                        snprintf(number, sizeof(number), "%d", cell->bombs);
                        gtk_button_set_label(GTK_BUTTON(cell->button), number);
                }

                if(cell->bombs == 0)
                {
                        for(int i = 0; i < cell->connections; i++)
                                if(!cell->neighbour[i]->revealed)
                                        on_left_click(cell->neighbour[i]);
                }

		win(p_cells);
        }

        return;
}


void on_right_click(cell_t cell)
{
	if(cell->revealed)
		return;

        if(!cell->flagged)
        {
                cell->flagged = true;
                gtk_button_set_label(GTK_BUTTON(cell->button), "🚩");
        }

        else
        {
                cell->flagged = false;
                gtk_button_set_label(GTK_BUTTON(cell->button), "");
        }
}

