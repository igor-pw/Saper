#include "h_files/board.h"
#include "h_files/mouse_click.h"

static int cols = 9;
static int rows = 9;
static int bombs = 10;

void set_variables(int mode)
{
	printf("DZIALAM\n");
	
	printf("%d\n", mode);

	if(mode == 2)
	{
		printf("JESTEM W SRODKU\n");

		cols = 16;
		rows = 16;
		bombs = 40;

		printf("%d %d %d\n", cols, rows, bombs);

	}

	switch(mode)
	{	
		case 1:
			cols = 9;
			rows = 9;
			bombs = 10;
		break;

		case 2:
			cols = 16;
			rows = 16;
			bombs = 40;
		break;

		case 3:
			cols = 30;
			rows = 16;
			bombs = 99;
		break;
	}

	return;
}

cell_t **init_cells(GtkWidget *grid)
{
        //inicjacja pierwszego wymiaru dwuwymiarowej tablicy wskaznikow do struktur
        cell_t **cells = malloc(sizeof cells * cols);

        for(int i = 0; i < cols; i++)
        {
                //inicjacja drugiego wymiaru dwuwymiarowej tablicy
                cells[i] = malloc(sizeof cells * rows);

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
                        gtk_grid_attach(GTK_GRID(grid), cells[i][j]->button, i, j, 1, 1);

                        //przypisanie przyciskowi funkcji
                        g_signal_connect(cells[i][j]->button, "button-press-event", G_CALLBACK(on_button_click), cells[i][j]);
                }
        }

        return cells;
}

void add_bombs(cell_t **cells)
{
        int x;
        int y;

        for(int i = 0; i < bombs; i++)
        {
                x = rand() % cols;
                y = rand() % rows;

                if(!cells[x][y]->bomb && !cells[x][y]->revealed)
                        cells[x][y]->bomb = true;

                else
                        i--;
        }

        return;
}

void count_connections(cell_t **cells)
{
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

void create_connections(cell_t **cells)
{
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

void count_bombs(cell_t **cells)
{
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

void game_over(cell_t **cells)
{
	for(int i = 0; i < cols; i++)
	{
		for(int j = 0; j < rows; j++)
		{
			cells[i][j]->revealed = true;

			if(cells[i][j]->bomb)
			{
				gtk_button_set_label(GTK_BUTTON(cells[i][j]->button), "💣");
				//sleep(0.5);
			}
		}
	}

}

void win(cell_t **cells)
{
	for(int i = 0; i < cols; i++)
	{	
		for(int j = 0; j < rows; j++)
		{
			if(!cells[i][j]->revealed && !cells[i][j]->bomb)
				return;
		}

	}

	for(int i = 0; i < cols; i++)
	{
		for(int j = 0; j < rows; j++)
		{
			if(cells[i][j]->bomb)
			{
				cells[i][j]->revealed = true;
				gtk_button_set_label(GTK_BUTTON(cells[i][j]->button), "💣");
			}
		}
	}

	//free_memory(cells);

	//gtk_main_quit();

}

void free_memory(cell_t **cells)
{
	for(int i = 0; i < cols; i++)
	{
		for(int j = 0; j < rows; j++)
		{
			free(cells[i][j]->neighbour);
			free(cells[i][j]);
		}

		free(cells[i]);
		
	}

	free(cells);
}
