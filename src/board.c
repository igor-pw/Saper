#include "h_files/board.h"
#include "h_files/mouse_click.h"
#include "h_files/after_game.h"

static int time_value = 0;
static bool end = false;

gd_t init_GameData(GtkWidget *window, GtkWidget *stack, GtkWidget *board, GtkWidget *timer_label, GtkWidget *points_label, GtkWidget *flags_label)
{
	gd_t game_data = malloc(sizeof *game_data);

	game_data->flags_label = flags_label;
	game_data->points_label = points_label;
	game_data->timer_label = timer_label;
	game_data->cells = NULL;
	game_data->grid = board;
	game_data->stack = stack;
	game_data->window = window;
	return game_data;
}

void set_end_false()
{
	end = false;
	return;
}

gboolean update_timer(gpointer data) {
    
	gd_t game_data = (gd_t)data;
	
	time_value++;

    	char buffer[50];
    	sprintf(buffer, "%d", time_value);
    	gtk_label_set_text(GTK_LABEL(game_data->timer_label), buffer);

    	if (time_value >= 999 || end)
	{
        	time_value = 0;
		return FALSE; 
	}
   	return TRUE;
}

void start_timer(gd_t game_data) 
{
   	g_timeout_add(1000, update_timer, game_data);
}

void set_variables(int mode, gd_t game_data)
{
	switch(mode)
	{	
		case 1:
			game_data->mode = 1;
			game_data->cols = 9;
			game_data->rows = 9;
			game_data->bombs = 10;
			game_data->flags = 10;
		break;

		case 2:
			game_data->mode = 2;
			game_data->cols = 16;
			game_data->rows = 16;
			game_data->bombs = 40;
			game_data->flags = 40;
		break;

		case 3:
			game_data->mode = 3;
			game_data->cols = 30;
			game_data->rows = 16;
			game_data->bombs = 99;
			game_data->flags = 99;
		break;
	}

	update_flags_label(game_data);

	return;
}

void update_points_label(gd_t game_data)
{
	int current_points = 10*game_data->revealed*game_data->mode;

	char points[10];
	sprintf(points, "%d", current_points);

	gtk_label_set_text(GTK_LABEL(game_data->points_label), points);

	return;
}

void update_flags_label(gd_t game_data)
{
	char flags[3];
	sprintf(flags, "%d", game_data->flags);

	gtk_label_set_text(GTK_LABEL(game_data->flags_label), flags);
	
	return;
}

cell_t **init_board(gd_t game_data)
{
	end = false;
	int cols = game_data->cols;
	int rows = game_data->rows;
	int bombs = game_data->bombs;

	game_data->revealed = 0;

        //inicjacja pierwszego wymiaru dwuwymiarowej tablicy wskaznikow do struktur
        cell_t **cells = malloc(sizeof(cell_t*) * cols);

        for(int i = 0; i < cols; i++)
        {
                //inicjacja drugiego wymiaru dwuwymiarowej tablicy
                cells[i] = malloc(sizeof(cell_t) * rows);

                for(int j = 0; j < rows; j++)
                {
                        //inicjacja struktur
                        cells[i][j] = malloc(sizeof ***cells);
                        cells[i][j]->revealed = false;
                        cells[i][j]->bomb = false;
                        cells[i][j]->flagged = false;
                        cells[i][j]->bombs = 0;

                        //inicjacja przycisku
                        cells[i][j]->button = gtk_button_new_with_label("");

			//przypisanie komorki do siatki
                        gtk_grid_attach(GTK_GRID(game_data->grid), cells[i][j]->button, i, j, 1, 1);

                        //przypisanie przyciskowi funkcji
                        g_signal_connect(cells[i][j]->button, "button-press-event", G_CALLBACK(on_button_click), cells[i][j]);
                }
        }

	gtk_widget_show_all(game_data->grid);

        return cells;
}

void add_bombs(gd_t game_data)
{
        int x;
        int y;


        for(int i = 0; i < game_data->bombs; i++)
        {
                x = rand() % game_data->cols;
                y = rand() % game_data->rows;

                if(!game_data->cells[x][y]->bomb && !game_data->cells[x][y]->revealed)
                        game_data->cells[x][y]->bomb = true;

                else
                        i--;
        }

        return;
}

void count_connections(gd_t game_data)
{
	int cols = game_data->cols;
	int rows = game_data->rows;

	cell_t **cells = game_data->cells;

        for(int i = 0; i < cols; i++)
        {
                for(int j = 0; j < rows; j++)
                {
                        if(i-1 < 0 || i+1 > cols-1 || j-1 < 0 || j+1 > rows-1)
                        {
                                if((i == 0 && j == 0) || (i == 0 && j == rows-1) || (i == cols-1 && j == 0) || (i == cols-1 && j == rows-1))
                                        cells[i][j]->connections = 3;

                                else
                                        cells[i][j]->connections = 5;
                        }

                        else
                                cells[i][j]->connections = 8;
                }
        }
}

void create_connections(gd_t game_data)
{
	int cols = game_data->cols;
	int rows = game_data->rows;
	
	cell_t **cells = game_data->cells;

        for(int i = 0; i < cols; i++)
        {
                for(int j = 0; j < rows; j++)
                {
                        cells[i][j]->neighbour = malloc(sizeof cells * cells[i][j]->connections);

                        if((i > 0 && j > 0 && i < cols-1 && j < rows-1))
                        {
                                cells[i][j]->neighbour[0] = cells[i-1][j-1];
                                cells[i][j]->neighbour[1] = cells[i][j-1];
                                cells[i][j]->neighbour[2] = cells[i+1][j-1];
                                cells[i][j]->neighbour[3] = cells[i-1][j];
                                cells[i][j]->neighbour[4] = cells[i-1][j+1];
                                cells[i][j]->neighbour[5] = cells[i][j+1];
                                cells[i][j]->neighbour[6] = cells[i+1][j+1];
                                cells[i][j]->neighbour[7] = cells[i+1][j];
                        }

                        else if(i-1 < 0)
                        {
                                if(j-1 < 0)
                                {
                                        cells[i][j]->neighbour[0] = cells[i+1][j+1];
                                        cells[i][j]->neighbour[1] = cells[i][j+1];
                                        cells[i][j]->neighbour[2] = cells[i+1][j];
                                }

                                else if(j+1 > rows-1)
                                {
                                        cells[i][j]->neighbour[0] = cells[i+1][j-1];
                                        cells[i][j]->neighbour[1] = cells[i][j-1];
                                        cells[i][j]->neighbour[2] = cells[i+1][j];
                                }

                                else
                                {
                                        cells[i][j]->neighbour[0] = cells[i+1][j+1];
                                        cells[i][j]->neighbour[1] = cells[i][j+1];
                                        cells[i][j]->neighbour[2] = cells[i+1][j-1];
                                        cells[i][j]->neighbour[3] = cells[i][j-1];
                                        cells[i][j]->neighbour[4] = cells[i+1][j];

                                }
                        }

                        else if(i+1 > cols-1)
                        {
                                if(j-1 < 0)
                                {
                                        cells[i][j]->neighbour[0] = cells[i-1][j+1];
                                        cells[i][j]->neighbour[1] = cells[i][j+1];
                                        cells[i][j]->neighbour[2] = cells[i-1][j];
                                }

                                else if(j+1 > rows-1)
                                {
                                        cells[i][j]->neighbour[0] = cells[i-1][j-1];
                                        cells[i][j]->neighbour[1] = cells[i][j-1];
                                        cells[i][j]->neighbour[2] = cells[i-1][j];
                                }

                                else
                                {
                                        cells[i][j]->neighbour[0] = cells[i-1][j+1];
                                        cells[i][j]->neighbour[1] = cells[i][j+1];
                                        cells[i][j]->neighbour[2] = cells[i-1][j-1];
                                        cells[i][j]->neighbour[3] = cells[i][j-1];
                                        cells[i][j]->neighbour[4] = cells[i-1][j];
                                }
                        }

                        else if(i-1 >= 0)
                        {
                                if(j-1 < 0)
                                {
                                        cells[i][j]->neighbour[0] = cells[i-1][j];
                                        cells[i][j]->neighbour[1] = cells[i-1][j+1];
                                        cells[i][j]->neighbour[2] = cells[i][j+1];
                                        cells[i][j]->neighbour[3] = cells[i+1][j+1];
                                        cells[i][j]->neighbour[4] = cells[i+1][j];


                                }

                                else if(j+1 > rows-1)
                                {
                                        cells[i][j]->neighbour[0] = cells[i-1][j];
                                        cells[i][j]->neighbour[1] = cells[i-1][j-1];
                                        cells[i][j]->neighbour[2] = cells[i][j-1];
                                        cells[i][j]->neighbour[3] = cells[i+1][j-1];
                                        cells[i][j]->neighbour[4] = cells[i+1][j];
                                }
                        }

                }
        }

        return;

}

void count_bombs(gd_t game_data)
{
	int cols = game_data->cols;
	int rows = game_data->rows;
	
	cell_t **cells = game_data->cells;

        for(int i = 0; i < cols; i++)
        {
                for(int j = 0; j < rows; j++)
                {
                        for(int k = 0; k < cells[i][j]->connections; k++)
                        {
                                if(cells[i][j]->neighbour[k]->bomb)
                                        cells[i][j]->bombs++;
                        }
                }

        }

        return;

}

void game_over(gd_t game_data)
{
	end = true;

	int cols = game_data->cols;
	int rows = game_data->rows;

	cell_t **cells = game_data->cells;

	for(int i = 0; i < cols; i++)
	{
		for(int j = 0; j < rows; j++)
		{
			cells[i][j]->revealed = true;

			if(cells[i][j]->bomb)
				gtk_button_set_label(GTK_BUTTON(cells[i][j]->button), "💣");
		}
	}
	
	game_lost(game_data);
}

void win(gd_t game_data)
{
	if(game_data->revealed == (game_data->cols*game_data->rows - game_data->bombs))
	{
		for(int i = 0; i < game_data->cols; i++)
		{
			for(int j = 0; j < game_data->rows; j++)
			{
				if(game_data->cells[i][j]->bomb)
				{
					game_data->cells[i][j]->revealed = true;
					gtk_button_set_label(GTK_BUTTON(game_data->cells[i][j]->button), "💣");
				}
			}
		}

	game_won(game_data);

	}
}

void reset_board(gd_t game_data)
{
	cell_t **cells = game_data->cells;

	for(int i = 0; i < game_data->cols; i++)
	{
		for(int j = 0; j < game_data->rows; j++)
		{
			free(cells[i][j]->neighbour);
			gtk_widget_destroy(cells[i][j]->button);
			free(cells[i][j]);
		}

		free(cells[i]);
		
	}

	free(cells);
}
