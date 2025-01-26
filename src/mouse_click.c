#include "h_files/mouse_click.h"
#include "h_files/board.h"

static bool first_click = true;
static gd_t p_gd = NULL;

void set_first_click_true()
{
	first_click = true;
	return;
}

void get_game_data(gd_t game_data)
{
	p_gd = game_data;
	return;
}

gboolean on_button_click(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	cell_t cell = (cell_t)data;
	
	if(event->button == GDK_BUTTON_SECONDARY)
                on_right_click(cell);
        
	else if(first_click)
        {
		first_click = false;
		cell->revealed = true;
                
		add_bombs(p_gd);
                
		cell->revealed = false;

		count_connections(p_gd);
                create_connections(p_gd);
                count_bombs(p_gd);

		on_left_click(cell);
		cell->revealed = true;
        }

	else if(event->button == GDK_BUTTON_PRIMARY)
                on_left_click(cell);

        return TRUE;
}

void on_left_click(cell_t cell)
{
        if(first_click)
                first_click = false;

        if(cell->flagged || cell->revealed) 
 		return;

        cell->revealed = true;
	p_gd->revealed++;
        gtk_widget_set_sensitive(cell->button, FALSE);

        if(cell->bomb)
	{
	        gtk_button_set_label(GTK_BUTTON(cell->button), "💣");
		game_over(p_gd);
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
		
		if(!cell->bomb)
			update_points_label(p_gd);
		
		win(p_gd);
        }

        return;
}


void on_right_click(cell_t cell)
{
	if(cell->revealed)
		return;

        if(!cell->flagged)
        {
                p_gd->flags--;
		cell->flagged = true;
                gtk_button_set_label(GTK_BUTTON(cell->button), "🚩");
        }

        else
        {
		p_gd->flags++;
                cell->flagged = false;
                gtk_button_set_label(GTK_BUTTON(cell->button), "");
        }
	
	update_flags_label(p_gd);
}

