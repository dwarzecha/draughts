#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
TODO
- finish fill men algorithm
- if_append
- if_move_possible
- append to selector
- move men algorithm (check for conditions, etc.)
- choosing game type?
- [FIX] why initial position of selector + 1 is the same as +2?
        (the board starts printing at 0)
- [CODE] comments
- [CODE] check_for_errors non universal, but we'll see close to the end of project
- [CODE] separate headers file?
- [FRONTEND] make set_men function more scalable?
*/

/*
#ifdef _WIN32
    #define clear_screen system("cls")
#elif __APPLE__
    // printf ("\33c\e[3J");
    #define clear_screen system("clear") // fake, only prints a bunch of \n
#endif
*/

typedef struct game_parameters
{
	bool end_of_game;
    int board_dimensions;
    int number_of_pieces;
    int number_of_games;
}game_parameters;

typedef struct sizeof_board
{
    // square dimensions

    int square_width;
    int square_height;

    // distance from the screen border

    int border_x;
    int border_y;

    // size

    int rows;
    int columns;
}sizeof_board;

typedef struct selector
{
    // position
    // i - vertical axis, j - horizontal axis

    int i;
    int j;

    //dimensions

    int width;
    int height;
}selector;

typedef struct piece
{
	bool alive;
	bool white;
	bool king;

	int pos_x;
	int pos_y;

	bool if_append;
}piece;

void fill_board(sizeof_board b1, char board[b1.rows][b1.columns]);
void print_board(sizeof_board b1, char board[b1.rows][b1.columns]);
void controls(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1, game_parameters g1, piece p1[g1.number_of_pieces]);
void set_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector s1);
void clear_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1);
void create_men(sizeof_board b1, game_parameters g1, piece p1[g1.number_of_pieces]);
void fill_men(sizeof_board b1, char board[b1.rows][b1.columns], game_parameters g1, piece p1[g1.number_of_pieces]);
// void ask_for_dimensions(game_parameters *g1); // just for debugging properties
void check_for_errors(game_parameters g1);
void append(sizeof_board b1, selector *s1, game_parameters g1, piece p1[g1.number_of_pieces]);

int main()
{
	// initial parameters

    game_parameters g1 = {false, 10, 30, 1};
    check_for_errors(g1); // check if g1.number_of_pieces won't be too big for this g1.board_dimensions
    // ask_for_dimensions(&g1);


    // defining game objects

    sizeof_board b1 = {10, 5, 0, 0, b1.square_height*g1.board_dimensions+b1.border_y+1, b1.square_width*g1.board_dimensions+b1.border_x+1}; // define board size with default dimensions 10x10
    selector s1 = {b1.border_y+1, b1.border_x+1}; // define selector starting position
    char board[b1.rows][b1.columns]; // create an array for board
    piece p1[g1.number_of_pieces]; // create an array of structures for pieces 


    // filling game objects with initial content

    fill_board(b1, board); // fill board array with graphic symbols
    create_men(b1, g1, p1);


    // gameplay

    while(!g1.end_of_game)
    {
        printf("\33c\e[3J"); // clear the console, replace with clear_screen; below if necessary
        //clear_screen; // use if running in standard windows terminals, unsafe (system())
        fill_men(b1, board, g1, p1);
        set_selector(b1, board, s1); // input the selector symbols into the board array
        print_board(b1, board); // print the board array
        printf("%d %d", s1.i, s1.j); // debugging line
        
        for (int i=0; i<g1.number_of_pieces; i++)
        {
        	printf("%d ", p1[i].if_append);
        }
        
        controls(b1, board, &s1, g1, p1); // change selector position
    }
    return 0;
}

void fill_board(sizeof_board b1, char board[b1.rows][b1.columns])
{
    // fill the board

	// fill the first b1.border_y rows with spaces
    for(int i=0; i<b1.border_y; i++)
    {
    	for(int j=0; j<b1.columns; j++)
    	{
    		board[i][j] = ' ';
    	}
    }

    for(int i=b1.border_y; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y
    {
    	for(int j=0; j<b1.border_x; j++)
    	{
    		board[i][j] = ' '; // fill the first b1.border_x cells with spaces
    	}

        for(int j=b1.border_x; j<b1.columns; j++) // fill the row with dashes
        {
            board[i][j] = '-';
        }
    }

    for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y
    {
        for(int j=i; j<i+b1.square_height-1; j++) // go b1.square_height-1 rows at a time
        {
        	for(int k=0; k<b1.border_x; k++)
    		{
    			board[j][k] = ' '; // fill the first b1.border_x cells with spaces
    		}

            for(int k=b1.border_x; k<b1.columns; k+=b1.square_width) // fill every b1.square_width cell starting from b1.border_x with vertical line
            {
                board[j][k] = '|';
            }
        }
    }

    for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y+1
    {
        for(int j=i; j<i+b1.square_height-1; j++) // go b1.square_height-1 rows at a time
        {
			for(int k=0; k<b1.border_x; k++)
    		{
    			board[j][k] = ' '; // fill the first b1.border_x cells with spaces
    		}

            for(int k=b1.border_x+1; k<b1.columns; k+=b1.square_width) // set j on every b1.square_width cell starting from b1.border_x+1
            {          
                for(int l=k; l<k+b1.square_width-1; l++) // fill b1.square_width-1 consecutive cells with space
                {
                    board[j][l] = ' ';
                }
            }
        }
    }

}

void print_board(sizeof_board b1, char board[b1.rows][b1.columns])
{
    // print the board

    for(int i=0; i<b1.rows; i++)
    {
        for(int j=0; j<b1.columns; j++)
        {
            printf("%c", board[i][j]);
        }

        printf("\n");
    }

}

void controls(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1, game_parameters g1, piece p1[g1.number_of_pieces])
{
    char key = 0; // buffer for key pressed

    while(1) // always until break
    {
        key = getchar(); // get the ASCII value of the key pressed

        if(key==100) // if d was pressed
        {
            if(s1->j == b1.columns - b1.square_width) // doing nothing if the selector is on the right border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->j = s1->j + b1.square_width; // move b1.square_height spaces to the right
                break;
            }
        }
        else if(key == 97) // if a was pressed
        {
            if(s1->j == b1.border_x+1) // doing nothing if the selector is on the left border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->j = s1->j - b1.square_width; // move b1.square_height spaces to the left
                break;
            }
        }
        else if(key == 119) // if w was pressed
        {
            if(s1->i == b1.border_y+1) // doing nothing if the selector is on the upper border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->i = s1->i - b1.square_height; // move b1.square_height spaces up
                break;
            }
        }
        else if(key == 115) // if s was pressed
        {
            if(s1->i == b1.rows - b1.square_height) // doing nothing if the selector is on the lower border
            {

            }
            else
            {
                clear_selector(b1, board, s1); // clear previous selector
                s1->i = s1->i + b1.square_height; // move b1.square_height spaces down
                break;
            }
        }
        else if(key == 'e')
		{
			append(b1, s1, g1, p1);
			break;
		}
        else if(key == 'x') // debugging line
        {
        	printf("\33c\e[3J"); // clear the console
        	exit(0);
        }
    }
}

void set_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector s1)
{
    board[s1.i][s1.j] = '|'; // set upper left corner
    board[s1.i][s1.j+b1.square_width-2] = '|'; // set upper right corner b1.square_width-2 spaces to the right
    board[s1.i+b1.square_height-2][s1.j] = '|'; // set bottom left corner b1.square_height-2 spaces down
    board[s1.i+b1.square_height-2][s1.j+b1.square_width-2] = '|'; // set bottom right corner b1.square_width-2 spaces to the right, b1.square_height-2 spaces down
}

void clear_selector(sizeof_board b1, char board[b1.rows][b1.columns], selector *s1)
{
    board[s1->i][s1->j] = ' '; // clear upper left corner
    board[s1->i][s1->j+b1.square_width-2] = ' '; // clear upper right corner x spaces to the right (now x = 3)
    board[s1->i+b1.square_height-2][s1->j] = ' '; // clear bottom left corner x spaces down (now x = 1)
    board[s1->i+b1.square_height-2][s1->j+b1.square_width-2] = ' '; // clear bottom right corner x spaces to the right, y spaces down (now  x = 3, y = 1)
}

void create_men(sizeof_board b1, game_parameters g1, piece p1[g1.number_of_pieces])
{
	for(int i=0; i<g1.number_of_pieces; i++)
	{
		p1[i].alive = true;
		p1[i].king = false;
		p1[i].if_append = false;

		if(i<g1.number_of_pieces/2)
		{
			p1[i].white = false; 
		}
		else
		{
			p1[i].white = true; 
		}

		if(g1.board_dimensions*i<g1.number_of_pieces)
		{
			for(int j=0; j<g1.board_dimensions; j++)
			{
				p1[j+g1.board_dimensions*i].pos_x = b1.border_x+1+j*b1.square_width;
				p1[j+g1.board_dimensions*i].pos_y = b1.border_y+1+i*b1.square_height;
			}
		}
	}
}

void fill_men(sizeof_board b1, char board[b1.rows][b1.columns], game_parameters g1, piece p1[g1.number_of_pieces])
{
    for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y+1
    {
	    for(int j=b1.border_x+1; j<b1.columns; j+=b1.square_width) // set j on every b1.square_width cell starting on b1.border_x+1
	    {
	    	for(int k=0; k<g1.number_of_pieces; k++)
	    	{
	    		if(p1[k].pos_x == j && p1[k].pos_y == i)
	    		{
	    			if(p1[k].alive == 1)
	    			{
	    				if(p1[k].white == 1)
	    				{
	    					if(p1[k].king == 1)
	        				{
	        					for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
										board[i+l][j+m] = 'O';

	        							board[i+l][j] = ' ';
	        							board[i+l][j+b1.square_width-2] = ' ';

	        							board[i+l][j+1] = ' ';
	        							board[i+l][j+b1.square_width-3] = ' ';

	        							board[i+l][j+2] = ' ';
	        							board[i+l][j+b1.square_width-4] = ' ';
			        				}
			        			}

			        			for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
			        					board[i][j+m] = '|';

			        					board[i][j] = ' ';
		    							board[i][j+b1.square_width-2] = ' ';

		    							board[i][j+1] = ' ';
		    							board[i][j+b1.square_width-3] = ' ';
			        				}
			        			}

			        			for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
		    							board[i+b1.square_height-2][j+m] ='-';
		    							
		    							board[i+b1.square_height-2][j] = ' ';
		    							board[i+b1.square_height-2][j+b1.square_width-2] = ' ';

		    							board[i+b1.square_height-2][j+1] = ' ';
		    							board[i+b1.square_height-2][j+b1.square_width-3] = ' ';
			        				}
			        			}
			        		}
	        				else
	        				{
	        					for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
	        							board[i+l][j+m] = 'O';

	        							board[i+l][j] = ' ';
	        							board[i+l][j+b1.square_width-2] = ' ';

	        							board[i][j+1] = ' ';
	        							board[i][j+b1.square_width-3] = ' ';
	        							
	        							board[i+b1.square_height-2][j+1] =' ';
	        							board[i+b1.square_height-2][j+b1.square_width-3] =' ';
	        						}
	        					}
	        				}
	    				}
	    				else
	    				{
	    					if(p1[k].king == 1)
	        				{
	        					for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
	        							board[i+l][j+3] = 'O';
	        							board[i+l][j+b1.square_width-5] = 'O';

			        					board[i][j+m] = '|';

			        					board[i][j] = ' ';
		    							board[i][j+b1.square_width-2] = ' ';

		    							board[i][j+1] = ' ';
		    							board[i][j+b1.square_width-3] = ' ';
			        				}
			        			}

			        			for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
		    							board[i+b1.square_height-2][j+m] ='-';
		    							
		    							board[i+b1.square_height-2][j] = ' ';
		    							board[i+b1.square_height-2][j+b1.square_width-2] = ' ';

		    							board[i+b1.square_height-2][j+1] = ' ';
		    							board[i+b1.square_height-2][j+b1.square_width-3] = ' ';
			        				}
			        			}
	        				}
	        				else
	        				{
	        					for(int l=0; l<b1.square_height-1; l++)
	        					{
	        						for(int m=0; m<b1.square_width-1; m++)
	        						{
	        							board[i][j+m] = 'O';
	        							board[i+b1.square_height-2][j+m] ='O';

	        							board[i+l][j+1] = 'O';
	        							board[i+l][j+2] = 'O';
	        							board[i+l][j+b1.square_width-3] ='O';
	        							board[i+l][j+b1.square_width-4] ='O';

	        							board[i][j] = ' ';
	        							board[i][j+1] = ' ';

	        							board[i][j+b1.square_width-2] = ' ';
	        							board[i][j+b1.square_width-3] = ' ';

	        							board[i+b1.square_height-2][j] = ' ';
	        							board[i+b1.square_height-2][j+b1.square_width-2] = ' ';
	        							
	        							board[i+b1.square_height-2][j+1] =' ';
	        							board[i+b1.square_height-2][j+b1.square_width-3] =' ';
	        						}
	        					}
	        				}
	    				}
	    			}
	    		}
	    	}
	  	}
    }
}


/*
void ask_for_dimensions(game_parameters *g1)
{
    int f = FALSE;
    while(!f)
    {
        printf("What size of the board would you like to print? ");
        scanf("%d", &g1->board_dimensions);
        if(g1->board_dimensions<=0)
        {
            printf("\nInvalid value! Try once again\n\n");
            f = FALSE;
        }
        else
        {
            f = TRUE;
        }
   }
}
*/

void check_for_errors(game_parameters g1)
{
	if(g1.number_of_pieces>(g1.board_dimensions*g1.board_dimensions)/2)
    {
    	printf("ERROR: Too big number_of_pieces for this board_dimensions, please change variable\nThe program will now terminate.\nPress [Enter] to continue...");
    	getchar();
    	exit(0);
    }
    else if(g1.number_of_pieces<0)
    {
    	printf("ERROR: Too small number_of_pieces for this board_dimensions, please change variable.\nThe program will now terminate.\nPress [Enter] to continue...");
    	getchar();
    	exit(0);
    }
}

void append(sizeof_board b1, selector *s1, game_parameters g1, piece p1[g1.number_of_pieces])
{
	for(int i=b1.border_y+1; i<b1.rows; i+=b1.square_height) // every b1.square_height row starting from b1.border_y+1
    {
	    for(int j=b1.border_x+1; j<b1.columns; j+=b1.square_width) // set j on every b1.square_width cell starting on b1.border_x+1
	    {
	    	for(int k=0; k<g1.number_of_pieces; k++)
	    	{
	    		if(p1[k].pos_x == j && p1[k].pos_y == i)
	    		{
	    			if(p1[k].alive == 1)
	    			{
	    				if(p1[k].pos_x == s1->j && p1[k].pos_y == s1->i)
	    				{
	    					p1[k].if_append = true;
	    				}
	    			}
	    		}
	    	}
	    }
	}
}